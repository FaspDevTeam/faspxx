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
        this->rj.SetValues(len, 0.0);
        this->r0.SetValues(len, 0.0);
        this->pj.SetValues(len, 0.0);
        this->sj.SetValues(len, 0.0);
        this->ph.SetValues(len, 0.0);
        this->xh.SetValues(len, 0.0);
        this->qj.SetValues(len, 0.0);
        this->sh.SetValues(len, 0.0);
        this->yj.SetValues(len, 0.0);
        this->xmin.SetValues(len, 0.0);
        this->tmp.SetValues(len,0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    this->A = &_A;

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

    // local variables
    DBL n2b;
    INT stag = 1, moresteps = 1, maxmsteps = 1;
    INT flag, maxstagsteps, half_step = 0;
    DBL absres0 = 1e+20, absres = 1e+20, relres = 1e+20;
    DBL alpha, beta, omega, rho, rho1, rtv, tt;
    DBL normr, normr_act, normph, normx, imin;
    DBL norm_sh, norm_xh, normrmin, factor;

    // Output some info for debuging
    if (verbose > PRINT_NONE)
        std::cout<<"\nCalling BiCGstab solver (CSR) ...\n";

    // rj = b-A*u
    rj = b;
    n2b = rj.Norm2();

    flag = 1;
    xmin = x;
    imin = 0;

    numIter = 0;

    A->Apply(x, tmp);

    rj.AXPY(-1.0, tmp);

    normr = rj.Norm2();
    normr_act = normr;
    relres = normr / n2b;

    // if initial residual is small, no need to iterate!
    if (normr <= absTol || relres<relTol) {
        flag = 0;
        numIter = 0;
        goto FINISHED;
    }

    // output iteration information if needed
    PrintInfo(verbose, numIter, relres, n2b, 0.0);

    // shadow residual r0 = rj* := rj
    r0 = rj;
    normrmin = normr;

    rho = 1.0;
    omega = 1.0;
    stag = 0;
    alpha = 0.0;

    moresteps = 0;
    maxmsteps = 10;
    maxstagsteps = 3;

    // loop over maxit iterations (unless convergence or failure)
    while(numIter<=maxIter){
        numIter++;

        rho1 = rho;
        rho = r0.Dot(rj);

        if ((rho == 0.0) || (fabs(rho) >= DBL_MAX)) {
            flag = 4;
            goto FINISHED;
        }

        if (numIter == 1) {
            pj = rj;
        } else {
            beta = (rho / rho1) * (alpha / omega);

            if ((beta == 0) || (fabs(beta) > DBL_MAX)) {
                flag = 4;
                goto FINISHED;
            }

            // pj = rj + beta * (pj - omega * sj);
            pj.AXPY(-omega, sj);
            pj.AXPBY(beta, 1.0, rj);
        }

        pc->Apply(pj, ph);

        // sj = A*ph
        A->Apply(ph, sj);
        rtv = r0.Dot(sj);

        if ((rtv == 0.0) || (fabs(rtv) > DBL_MAX)) {
            flag = 4;
            goto FINISHED;
        }

        alpha = rho / rtv;

        if (fabs(alpha) > DBL_MAX) {
            flag = 4;
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            goto FINISHED;
        }

        normx = x.Norm2();
        normph = ph.Norm2();
        if (fabs(alpha) * normph < DBL_EPSILON * normx)
            stag = stag + 1;
        else
            stag = 0;

        // xh = x + alpha * ph;        // form the "half" iterate
        // qj = rj - alpha * sj;             // residual associated with xh
        xh.WAXPBY(1.0, x, alpha, ph);
        qj.WAXPBY(-alpha, sj, 1.0, rj);
        normr = qj.Norm2();
        normr_act = normr;

        // compute reduction factor of residual ||rj||
        absres = normr_act;
        factor = absres / absres0;
        PrintInfo(verbose, numIter, normr_act / n2b, absres, factor);

        // check for convergence
        if ((normr <= absTol) || (stag >= maxstagsteps) || moresteps) {
            qj = b;
            A->Apply(xh, tmp);
            qj.AXPY(-1.0, tmp);
            normr_act = qj.Norm2();

            if (normr_act <= absTol) {
                // x = xh;
                x = xh;
                flag = 0;
                imin = numIter - 0.5;
                half_step++;
                if (verbose >= PRINT_MORE)
                    printf("Flag = %d Stag = %d Itermin = %.1f Half_step = %d\n",
                           flag, stag, imin, half_step);
                goto FINISHED;
            } else {
                if ((stag >= maxstagsteps) && (moresteps == 0)) stag = 0;

                moresteps = moresteps + 1;
                if (moresteps >= maxmsteps) {
                    flag = 3;
                    x = xh;
                    goto FINISHED;
                }
            }
        }

        if (stag >= maxstagsteps) {
            flag = 3;
            goto FINISHED;
        }

        if (normr_act < normrmin) // update minimal norm quantities
        {
            normrmin = normr_act;
            xmin = xh;
            imin = numIter - 0.5;
            half_step++;
            if (verbose >= PRINT_MORE)
                std::cout<<"Flag = "<<flag<<"Stag = "<<stag<<"Itermin = "<<imin<<
                    "Half_step : "<<half_step<<std::endl;
        }

        pc->Apply(qj, sh);

        // yj = A*sh;
        A->Apply(sh, yj);
        // tt = yj' * yj;
        tt = yj.Dot(yj);
        if ((tt == 0) || (tt >= DBL_MAX)) {
            flag = 4;
            goto FINISHED;
        }

        // omega = (yj' * qj) / tt;
        omega = qj.Dot(yj) / tt;
        if (fabs(omega) > DBL_MAX) {
            flag = 4;
            goto FINISHED;
        }

        norm_sh = sh.Norm2();
        norm_xh = xh.Norm2();

        if (fabs(omega) * norm_sh < DBL_EPSILON * norm_xh)
            stag = stag + 1;
        else
            stag = 0;

        // x = xh + omega * sh;
        x.WAXPBY(1.0, xh, omega, sh);
        //rj = qj - omega * yj;
        rj.WAXPBY(1.0, qj, -omega, yj);
        //normr = norm(rj);
        normr = rj.Norm2();
        normr_act = normr;

        // check for convergence
        if ((normr <= absTol) || (stag >= maxstagsteps) || moresteps) {
            rj = b;
            A->Apply(x, tmp);
            rj.AXPY(-1.0, tmp);
            normr_act = rj.Norm2();
            if (normr_act <= absTol) {
                flag = 0;
                goto FINISHED;
            } else {
                if ((stag >= maxstagsteps) && (moresteps == 0)) stag = 0;
                moresteps = moresteps + 1;
                if (moresteps >= maxmsteps) {
                    flag = 3;
                    goto FINISHED;
                }
            }
        }

        // update minimal norm quantities
        if (normr_act < normrmin) {
            normrmin = normr_act;
            //fasp_darray_cp(m,x,xmin);
            xmin = x;
            imin = numIter;
        }

        if (stag >= maxstagsteps) {
            flag = 3;
            goto FINISHED;
        }

        if (verbose >= PRINT_MAX) RealRes(relres);

        absres0 = absres;
    }   // for iter = 1 : maxit

    FINISHED:  // finish iterative method
    // returned solution is first with minimal residual
    if (flag == 0)
        relres = normr_act / n2b;
    else {
        rj = b;
        A->Apply(xmin, tmp);
        rj.AXPY(-1.0, tmp);
        normr = rj.Norm2();

        if (normr <= normr_act) {
            x = xmin;
            numIter = imin;
            relres = normr / n2b;
        } else {
            numIter=numIter;
            relres = normr_act / n2b;
        }
    }

    if (verbose > PRINT_NONE) PrintFinal(verbose, numIter, maxIter, relres);

    if (verbose >= PRINT_MORE)
        std::cout<<"Flag = "<<flag<<"Stag = "<<stag<<"Itermin = "
        <<imin<<"Half_step = "<<half_step<<std::endl;

    norminf=rj.NormInf();
    norm2=rj.Norm2();

    if (numIter > maxIter)
        return ERROR_SOLVER_MAXIT;

    return FaspRetCode::SUCCESS;
}

/// clean preconditioner operator
void BiCGStab::CleanPCD() {
    if (pc == nullptr)
        pc = nullptr;
}

/// Release temporary memory
void BiCGStab::Clean() {
    VEC zero;
    rj = zero;
    r0 = zero;
    pj = zero;
    sj = zero;
    ph = zero;
    xh = zero;
    qj = zero;
    sh = zero;
    yj = zero;
    xmin = zero;
    tmp = zero;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/