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

/// solve by PCG
FaspRetCode BiCGStab::Solve(const VEC &b, VEC &x) {

    const unsigned MaxStag = 20;
    const INT len = b.GetSize();
    const DBL maxdiff = 1e-4 * relTol; // Stagnation tolerance
    const DBL solinftol = 1e-20; // Infinity norm tolerance

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    DBL resAbs = 1e+20, tmpAbs = 1e+20;
    DBL resRel = 1e+20, denAbs = 1e+20;
    DBL factor, tmpa, tmpb;
    DBL alphaj, omegaj, betaj;

    // Output iterative method info
    if (verbose > PRINT_NONE)
        std::cout<< "\nCalling BiCGStab solver ...\n";

    // r0 = b - A * x
    A->Apply(x, r0);
    r0.XPAY(-1.0, b);
    rj=r0;

    // Compute initial residual
    tmpAbs=rj.Norm2();
    denAbs=(1e-20>tmpAbs)?1e-20:tmpAbs;
    resRel=tmpAbs/denAbs;

    // If initial residual is already small, no need to iterate
    if(resRel<relTol || tmpAbs <absTol) goto FINISHED;

    // Prepare for the main loop
    // pj = r0
    pj=r0;
    PrintInfo(verbose,numIter,resRel,tmpAbs,0.0);
    tmpa=r0.Dot(rj);

    // Main BiCGStab loop
    while (numIter<maxIter) {
        numIter++; // iteration count

        // gj = P * pj
        pc->Apply(pj, gj);

        // sj = A * gj
        A->Apply(gj, sj);

        // alpha_{j} = (r0, rj)/(r0, sj)
        tmpb=r0.Dot(sj);
        if(fabs(tmpb)>1e-40) alphaj = tmpa / tmpb;
        else{
            FASPXX_WARNING("Divided by zero!") // Possible breakdown
            errorCode=FaspRetCode ::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        // qj = rj - alpha_{j} * sj
        qj.WAXPBY(1.0, rj, -alphaj, sj);

        // uj = P * qj
        pc->Apply(qj, uj);

        // yj = A * uj
        A->Apply(uj, yj);

        // omega_{j} = (qj, yj)/(yj, yj)
        omegaj = qj.Dot(yj) / yj.Dot(yj);

        // x_{j+1} = x_{j} + alpha_{j} * gj + omega_{j} * u_{j}
        tmp.WAXPBY(alphaj, gj, omegaj, uj);
        xj.AXPY(1.0, tmp);

        // r_{j+1} = qj - omegaj * yj
        tmp = rj;
        rj.WAXPBY(1.0, qj, -omegaj, yj);

        // Compute norm of residual
        resAbs=rj.Norm2();
        resRel=resAbs/denAbs;
        factor=resAbs/tmpAbs;

        // Output iteration information if needed
        PrintInfo(verbose,numIter,resRel,tmpAbs,factor);
#if 0
        if(factor>0.9) // Only Check when converge slowly
        {
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            DBL norminf=x.NormInf();
            if(norminf<solinftol){
                if(verbose>PRINT_MIN) FASPXX_WARNING("Iteration stopped -- "
                                                     "solution almost zero!");
                errorCode=FaspRetCode ::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference stagnated, try to restart
            DBL reldiff=fabs(alphaj)*pj.Norm2()/x.Norm2();
            if((stagStep<=MaxStag) && (reldiff<maxdiff)){
                if(verbose>PRINT_SOME){
                    DiffRes(reldiff,resRel);
                    FASPXX_WARNING("Iteration restarted -- stagnation!");
                }

                A->Apply(x, rj);
                rj.XPAY(-1.0, b);
                resAbs=rj.Norm2();
                resRel=resAbs/denAbs;
                if(verbose>PRINT_SOME) RealRes(resRel);

                if(resRel<relTol) break;
                else{
                    if(stagStep>=MaxStag){
                        if(verbose>PRINT_MIN) FASPXX_WARNING("Iteration stopped --"
                                                             " stagnation!");
                        errorCode=FaspRetCode ::ERROR_SOLVER_STAG;
                        break;
                    }
                    pj.SetValues(len,0.0);
                    ++stagStep;
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence
        if(resRel<relTol){
            // Compute true resdual r = b - Ax and update residual
            A->Apply(x,rj);
            rj.XPAY(-1.0,b);

            // Compute residual norms
            DBL updated_resRel=resRel;
            resAbs=rj.Norm2();
            resRel=resAbs/denAbs;

            // Check convergence
            if(resRel<relTol) break;

            if(verbose>=PRINT_MORE){
                Compres(updated_resRel);
                RealRes(resRel);
            }

            if(moreStep>=restart){
                if(verbose>PRINT_MIN) FASPXX_WARNING("The tolerance might be too "
                                                     "small!");
                errorCode=FaspRetCode ::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            pj.SetValues(len,0.0);
            ++moreStep;
        } // End of safe-guard check!
#endif
        // Save residual for next iteration
        tmpAbs=resAbs;

        // Compute betaj = alphaj/omegaj * (r0, r_{j+1}) / (r0, r_{j})
        // betaj = alphaj / omegaj * r0.Dot(rj) / r0.Dot(tmp);
        tmpb=r0.Dot(rj);
        betaj=tmpb/tmpa;
        tmpa=tmpb;

        // Compute p_{i+1} = r_{i+1} + betaj * (pj - omegaj *sj)
        pj.AXPY(-omegaj, sj);
        pj.AXPBY(betaj, 1.0, rj);
    } // End of main BiCGStab loop

    FINISHED: // Finish iterative method
    PrintFinal(verbose,numIter,maxIter,resRel);

    // Compute final residual norms
    norminf=rj.NormInf();
    norm2=rj.Norm2();

    return errorCode;
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