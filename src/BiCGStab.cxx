/**
 * a demo
 */

#include "BiCGStab.hxx"
#include <ios>
#include "PCD.hxx"


/// check and allocate memory
FaspRetCode
BiCGStab::Setup(const LOP &_A, const VEC &b, VEC &x) {
    if (x.GetSize() != _A.GetColSize() || b.GetSize() != _A.GetRowSize() ||
        _A.GetRowSize() != _A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len = b.GetSize();
    try {
        this->r0.SetValues(len, 0.0);
        this->rj.SetValues(len, 0.0);
        this->pj.SetValues(len, 0.0);
        this->gj.SetValues(len, 0.0);
        this->sj.SetValues(len, 0.0);
        this->qj.SetValues(len, 0.0);
        this->uj.SetValues(len, 0.0);
        this->yj.SetValues(len, 0.0);
        this->xj.SetValues(len, 0.0);
        this->tmp.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    this->A=&_A;

    /// identical preconditioner operator by default
    if (pc == nullptr) {
        IdentityLOP lop(len);
        pc = &lop;
    }

    return FaspRetCode::SUCCESS;
}

/// build preconditioner operator
void BiCGStab::SetPC(LOP *lop) {
    this->pc = lop;
}

/// solve by BiCGStab
FaspRetCode BiCGStab::Solve(const VEC &b, VEC &x) {

        const unsigned MaxStag = 20;
        const INT len = b.GetSize();
        const DBL maxdiff = 1e-4 * relTol; //Stagnation tolerance
        const DBL solinftol = 1e-20;

        // Local variables
        FaspRetCode errorCode = FaspRetCode::SUCCESS;
        unsigned stagStep = 1, moreStep = 1;
        DBL resAbs = 1e+20, tmpAbs = 1e+20;
        DBL resRel = 1e+20, denAbs = 1e+20;
        DBL alphaj, betaj, rjr0star, rjr0startmp, omegaj;
        DBL tmp12,factor;

        // Output iterative method info
        if (verbose > PRINT_NONE) std::cout << "\n Calling BiCGStab solver ...\n";

        // Initial iteration
        numIter = 0;

        // r0 = b - A * x_{0}
        A->Apply(x, this->tmp);
        this->rj.WAXPBY(1.0, b, -1.0, this->tmp);

        // Compute initial residual
        tmpAbs=this->rj.Norm2();
        denAbs=(1e-20>tmpAbs)?1e-20:tmpAbs;
        resRel=tmpAbs/denAbs;

        // If initial residual is already small, no need to iterate
        if(resRel <relTol || tmpAbs<absTol) goto FINISHED;

        // Prepare for the main loop
        PrintInfo(verbose,numIter,resRel,tmpAbs,0.0);

        // r0_{*} = r0
        this->r0 = this->rj;

        // p0 = r0
        this->pj = this->rj;

        while (true) {

            ++numIter;
            if (numIter > maxIter) {
                break;
            }

            // alpha_{j} = (rj,r0star)/(A * pj,r0star)
            rjr0star = this->rj.Dot(this->r0);


            A->Apply(this->pj, this->apj);

            tmp12 = this->apj.Dot(this->r0);
            if (fabs(tmp12) > 1e-40) alphaj = rjr0star / tmp12;
            else {
                FASPXX_WARNING("Divided by zero!"); // Possible breakdown
                errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
                goto FINISHED;
            }

            // sj = rj - alpha_{j} * A * p_{j}
            this->qj.WAXPBY(1.0, this->rj, -alphaj, this->apj);



            // omegaj = (A * sj,sj)/(A * sj,A * sj)
            A->Apply(this->qj, this->asj);
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
            PrintInfo(verbose, numIter, resRel, tmpAbs, factor);

            if (factor > 0.9) { // Only check when converge slowly
                // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
                DBL norminf = x.NormInf();
                if (norminf < solinftol) {
                    if (verbose > PRINT_MIN) FASPXX_WARNING(
                            "Iteration stopped -- solution almost zero!");
                    errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                    break;
                }

                // Check II: if relative difference stagnated, try to restart
                DBL reldiff = fabs(alphaj) * this->pj.Norm2() / x.Norm2();
                if ((stagStep <= MaxStag) && (reldiff < maxdiff)) {
                    if (verbose > PRINT_SOME) {
                        DiffRes(reldiff, resRel);
                        FASPXX_WARNING("Iteration restarted -- stagnation!");
                    }

                    A->Apply(x, this->rj);
                    this->rj.XPAY(-1.0, b);
                    resAbs = this->rj.Norm2();
                    resRel = resAbs / denAbs;

                    if (verbose > PRINT_SOME) RealRes(resRel);

                    if (resRel < relTol) break;
                    else {
                        if (stagStep >= MaxStag) {
                            if (verbose > PRINT_MIN) FASPXX_WARNING(
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
            if (resRel < relTol) {
                // Compute true residual r = b - Ax and update residual
                A->Apply(x, this->rj);
                this->rj.XPAY(-1.0, b);

                // Compute residual norms
                DBL updated_resRel = resRel;
                resAbs = rj.Norm2();
                resRel = resAbs / denAbs;

                // Check convergence
                if (resRel < relTol) break;

                if (verbose >= PRINT_MORE) {
                    Compres(updated_resRel);
                    RealRes(resRel);
                }

                if (moreStep >= restart) {
                    if (verbose > PRINT_MIN) FASPXX_WARNING(
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
            rjr0star = this->rj.Dot(this->r0);
            betaj = rjr0star / rjr0startmp * alphaj / omegaj;

            // p_{j+1} = r_{j+1} + betaj * (p_{j} - omegaj * A * p_{j})
            this->tmp.WAXPBY(1.0, this->pj, -omegaj, this->apj);
            this->pj.WAXPBY(1.0, this->rj, betaj, this->tmp);

        } // End of main PCG loop

        FINISHED: // Finish iterative method
        PrintFinal(verbose,numIter,maxIter,resRel);

        // Compute final residual norms
        norminf=rj.NormInf();
        norm2=rj.Norm2();

        return errorCode;
    }
}

/// clean preconditioner operator
void BiCGStab::CleanPCD() {
    if (pc == nullptr)
        pc = nullptr;
}

/// Release temporary memory
void BiCGStab::Clean() {
    VEC zero;
    r0 = zero;
    rj = zero;
    pj = zero;
    gj = zero;
    sj = zero;
    qj = zero;
    uj = zero;
    yj = zero;
    xj = zero;
    tmp = zero;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/