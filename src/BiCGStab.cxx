/**
 * a demo
 */

#include "BiCGStab.hxx"
#include <ios>
#include "PCG.hxx"
#include "Print.hxx"

/// check and allocate memory
FaspRetCode
BiCGStab::Setup(const LOP &A, const VEC &b, VEC &x, const IterParam &param) {
    if (x.GetSize() != A.GetColSize() || b.GetSize() != A.GetRowSize() ||
        A.GetRowSize() != A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len = b.GetSize();
    try {
        this->r0star.SetValues(len, 0.0);
        this->tmp.SetValues(len, 0.0);
        this->apj.SetValues(len, 0.0);
        this->asj.SetValues(len, 0.0);
        this->pj.SetValues(len, 0.0);
        this->rj.SetValues(len, 0.0);
        this->sj.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    /// identical preconditioner operator by default
    if (lop == nullptr)
        lop = new IdentityLOP(len);

    return FaspRetCode::SUCCESS;
}

/// build preconditioner operator
void BiCGStab::SetupPCD(LOP *lop) {
    this->lop = lop;
}

FaspRetCode BiCGStab::Solve(const LOP &A, const VEC &b, VEC &x, IterParam &param) {

    const unsigned MaxStag = 20;
    const INT len = b.GetSize();
    const DBL maxdiff = 1e-4 * param.relTol; //Stagnation tolerance
    const DBL solinftol = 1e-20;

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    DBL resAbs = 1e+20, tmpAbs = 1e+20;
    DBL resRel = 1e+20, denAbs = 1e+20;
    DBL alphaj, betaj, rjr0star, rjr0startmp, omegaj;
    DBL tmp12,factor;

    // Output iterative method info
    if (param.verbose > PRINT_NONE) std::cout << "\n Calling BiCGStab solver ...\n";

    // Initial iteration
    param.numIter = 0;

    // r0 = b - A * x_{0}
    A.Apply(x, this->tmp);
    this->rj.WAXPBY(1.0, b, -1.0, this->tmp);

    // Compute initial residual
    tmpAbs=this->rj.Norm2();
    denAbs=(1e-20>tmpAbs)?1e-20:tmpAbs;
    resRel=tmpAbs/denAbs;

    // If initial residual is already small, no need to iterate
    if(resRel <param.relTol || tmpAbs<param.absTol) goto FINISHED;

    // Prepare for the main loop
    PrintInfo(param.verbose,param.numIter,resRel,tmpAbs,0.0);

    // r0_{*} = r0
    this->r0star = this->rj;

    // p0 = r0
    this->pj = this->rj;

    while (true) {

        ++param.numIter;
        if (param.numIter > param.maxIter) {
            break;
        }

        // alpha_{j} = (rj,r0star)/(A * pj,r0star)
        rjr0star = this->rj.Dot(this->r0star);

        A.Apply(this->pj, this->apj);

        tmp12 = this->apj.Dot(this->r0star);
        if (fabs(tmp12) > 1e-40) alphaj = rjr0star / tmp12;
        else {
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        // sj = rj - alpha_{j} * A * p_{j}
        this->sj.WAXPBY(1.0, this->rj, -alphaj, this->apj);

        // omegaj = (A * sj,sj)/(A * sj,A * sj)
        A.Apply(this->sj, this->asj);
        omegaj = this->asj.Dot(this->sj) / this->asj.Dot(this->asj);

        // x_{j+1} = x_{j} + alpha_{j} * pj + omegaj * s_{j}
        this->tmp.WAXPBY(alphaj, this->pj, omegaj, this->sj);
        x.XPAY(1.0, this->tmp);

        // r_{j+1} = sj - omegaj * A * sj
        this->rj.WAXPBY(1.0, this->sj, -omegaj, this->asj);

        // Compute norm of residual
        resAbs = this->rj.Norm2();
        resRel = resAbs / denAbs;
        factor = resAbs / tmpAbs;

        // Output iteration information if needed
        PrintInfo(param.verbose, param.numIter, resRel, tmpAbs, factor);

        if (factor > 0.9) { // Only check when converge slowly
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            DBL norminf = x.NormInf();
            if (norminf < solinftol) {
                if (param.verbose > PRINT_MIN) FASPXX_WARNING(
                        "Iteration stopped -- solution almost zero!");
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference stagnated, try to restart
            DBL reldiff = fabs(alphaj) * this->pj.Norm2() / x.Norm2();
            if ((stagStep <= MaxStag) && (reldiff < maxdiff)) {
                if (param.verbose > PRINT_SOME) {
                    DiffRes(reldiff, resRel);
                    FASPXX_WARNING("Iteration restarted -- stagnation!");
                }

                A.Apply(x, this->rj);
                this->rj.XPAY(-1.0, b);
                resAbs = this->rj.Norm2();
                resRel = resAbs / denAbs;

                if (param.verbose > PRINT_SOME) RealRes(resRel);

                if (resRel < param.relTol) break;
                else {
                    if (stagStep >= MaxStag) {
                        if (param.verbose > PRINT_MIN) FASPXX_WARNING(
                                "Iteration stopped -- stagnation!");
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->pj.SetValues(len, 0.0);
                    ++stagStep;
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence
        if (resRel < param.relTol) {
            // Compute true residual r = b - Ax and update residual
            A.Apply(x, this->rj);
            this->rj.XPAY(-1.0, b);

            // Compute residual norms
            DBL updated_resRel = resRel;
            resAbs = rj.Norm2();
            resRel = resAbs / denAbs;

            // Check convergence
            if (resRel < param.relTol) break;

            if (param.verbose >= PRINT_MORE) {
                Compres(updated_resRel);
                RealRes(resRel);
            }

            if (moreStep >= param.restart) {
                if (param.verbose > PRINT_MIN) FASPXX_WARNING(
                        "The tolerence might be too small!");
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            this->pj.SetValues(len, 0.0);
            ++moreStep;
        } // End of safe-gurad check!

        // Save residual for next iteration
        tmpAbs = resAbs;

        // Compute betaj = (r_{j+1},r0^{*})/(r_{j},r0^{*}) *
        // \frac{alpha_{j}}{omega_{j}}
        rjr0startmp = rjr0star;
        rjr0star = this->rj.Dot(this->r0star);
        betaj = rjr0star / rjr0startmp * alphaj / omegaj;

        // p_{j+1} = r_{j+1} + betaj * (p_{j} - omegaj * A * p_{j})
        this->tmp.WAXPBY(1.0, this->pj, -omegaj, this->apj);
        this->pj.WAXPBY(1.0, this->rj, betaj, this->tmp);

    } // End of main PCG loop

    FINISHED: // Finish iterative method
    PrintFinal(param.verbose,param.numIter,param.maxIter,resRel);

    // Compute final residual norms
    param.normInf=rj.NormInf();
    param.norm2=rj.Norm2();

    return errorCode;
}

/// clean preconditioner operator
void BiCGStab::CleanPCD() {
    LOP *lop;
    this->lop = lop;
}

/// Release temporary memory
void BiCGStab::Clean() {
    VEC zero;
    this->sj = zero;
    this->rj = zero;
    this->pj = zero;
    this->asj = zero;
    this->apj = zero;
    this->tmp = zero;
    this->r0star = zero;
}
