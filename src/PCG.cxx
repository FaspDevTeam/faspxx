/**
 * a source file for PCG
 */

#include "PCG.hxx"

// Assign param to this->param
FaspRetCode PCG::SetUp(const MAT &A, const VEC &b, VEC &x, const IterParam &param) {
    if (x.GetSize() != A.GetColSize() || b.GetSize() != A.GetRowSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    return FaspRetCode::SUCCESS;
}

// Assign lop to *this
void PCG::SetUpPCD(const LOP &lop) {
    this->lop = lop;
    this->pcflag = 1;
}

// Start PCG
FaspRetCode PCG::Start(const MAT &A, const VEC &b, VEC &x, const StopType &type,
        IterParam &param) {

    const INT MaxStag = 20;
    const INT maxdiff = 1e-4 * param.relTol;// Stagnation tolerance
    const DBL solinftol = 1e-20;// Infinity norm tolerance

    // Local variables
    INT stag = 1, morestep = 1;
    DBL absres = 1e+20, abstmp = 1e+20;
    DBL relres = 1e+20, norm = 1e+20, normtmp = 1e+20;
    DBL reldiff, norminf, factor;
    DBL alpha, beta, tmpa, tmpb;
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Output some info for debuging
    if (param.outLvl > _PRINT_NONE)
        std::cout << "\nCalling PCG solver (CSR) ...\n";

    const INT len = b.GetSize();
    VEC rk(len), pk(len), zk(len), tmp(len);

    // rk = b - A * x
    A.MultVec(x, tmp);
    rk = b;
    rk -= tmp;

    if (pcflag == 1)
        ApplyPreconditioner(rk, zk, this->lop); // Apply preconditioner
    else
    zk = rk; // No preconditioner

    // Compute initial residuals
    switch (type) {
        case STOP_REL_RES:
            abstmp = rk.Norm2();
            normtmp = (1e-20 > abstmp) ? 1e-20 : abstmp;
            relres = abstmp / normtmp;
            break;
        case STOP_REL_PRECRES:
            abstmp = sqrt(rk.Dot(zk));
            normtmp = (1e-20 > abstmp) ? 1e-20 : abstmp;
            relres = abstmp / normtmp;
            break;
        case STOP_MOD_REL_RES:
            abstmp = rk.Norm2();
            norm = (1e-20 > x.Norm2()) ? 1e-20 : x.Norm2();
            relres = abstmp / norm;
            break;
        default:
            std::cout << "### ERROR: Unknown stopping type! [" << __FUNCTION__ << "]"
                      << std::endl;
            errorCode = FaspRetCode::ERROR_INPUT_PAR;
            goto FINISHED;
    }

    // If initial residual is small, no need to iterate
    if (relres < param.relTol || abstmp < param.absTol) goto FINISHED;

    // Output iteration information if needed
    PrintInfo(param.outLvl, type, param.numIter, relres, abstmp, 0.0);

    pk = zk;
    tmpa = zk.Dot(rk);

    // Main PCG loop
    while (param.numIter++ < param.maxIter) {
        // tmp = A * pk
        A.MultVec(pk, tmp);

        // alpha_k = (z_{k-1},r_{k-1})/(A*p_{k-1},p_{k-1})
        tmpb = tmp.Dot(pk);
        if (fabs(tmpb) > 1e-40)
            alpha = tmpa / tmpb;
        else {
            DivZero();// Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }
        // u_k = u_{k-1} + alpha_k*p_{k-1}
        x.Add(1.0, alpha, pk);

        // r_k = r_{k-1} - alpha_k*A*p_{k-1}
        rk.Add(1.0, -alpha, tmp);

        // Compute norm of residual
        switch (type) {
            case STOP_REL_RES:
                absres = rk.Norm2();
                relres = absres / normtmp;
                break;
            case STOP_REL_PRECRES:
                // z = B(r)
                if (this->pcflag == 1)
                    ApplyPreconditioner(rk, zk,
                                        this->lop); /* Apply preconditioner */
                else
                    zk = rk; /* No preconditioenr */

                absres = sqrt(fabs(zk.Dot(rk)));
                relres = absres / normtmp;
                break;
            case STOP_MOD_REL_RES:
                absres = rk.Norm2();
                relres = absres / norm;
                break;
        }
        // compute reduction factor of residual ||r||
        factor = absres / abstmp;

        // Output iteration information if needed
        PrintInfo(param.outLvl, type, param.numIter, relres, abstmp, factor);

        if (factor > 0.9) { // Only check when converge slowly

            // Check I : if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            norminf = x.NormInf();
            if (norminf <= solinftol) {
                if (param.outLvl > _PRINT_MIN) ZeroSol;
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if stagnated, try to restart
            norm = x.Norm2();

            // Compute relative difference
            reldiff = fabs(alpha) * pk.Norm2() / norm;

            if ((stag <= MaxStag) & (reldiff < maxdiff)) {

                if (param.outLvl >= _PRINT_MORE) {
                    DiffRes(reldiff, relres);
                    Restart();
                }

                A.MultVec(x, tmp);
                rk.Add(1.0, b, -1.0, tmp);

                switch (type) {
                    case STOP_REL_RES:
                        absres = rk.Norm2();
                        relres = absres / normtmp;
                        break;
                    case STOP_REL_PRECRES:
                        // z = B(r)
                        if (pcflag == 1)
                            ApplyPreconditioner(rk, zk,
                                                this->lop); // Apply preconditioner
                        else
                            zk = rk; // No preconditioner

                        absres = sqrt(fabs(zk.Dot(rk)));
                        relres = absres / normtmp;
                        break;
                    case STOP_MOD_REL_RES:
                        absres = rk.Norm2();
                        relres = absres / norm;
                        break;
                }

                if (param.outLvl >= _PRINT_MORE) RealRes(relres);

                if (relres < param.relTol)
                    break;
                else {
                    if (stag >= MaxStag) {
                        if (param.outLvl > _PRINT_MIN) Stagged();
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    pk.SetValues(pk.GetSize(), 0.0);
                    ++stag;
                }
            } // End of stagnation check!
        }// End of check I and II

        // Check III: prevent false convergence
        if (relres < param.relTol) {
            DBL updated_relres = relres;

            // Compute true residual r = b - Ax and update residual
            A.MultVec(x, tmp);

            rk.Add(1.0, b, -1.0, tmp);

            // Compute residual norms
            switch (type) {
                case STOP_REL_RES:
                    absres = rk.Norm2();
                    relres = absres / normtmp;
                    break;
                case STOP_REL_PRECRES:
                    // z = B(r)
                    if (pcflag == 1)
                        ApplyPreconditioner(rk, zk,
                                            this->lop); // Apply preconditioner
                    else
                        zk = rk; // No preconditioner

                    absres = sqrt(fabs(zk.Dot(rk)));
                    relres = absres / normtmp;
                    break;
                case STOP_MOD_REL_RES:
                    absres = rk.Norm2();
                    relres = absres / norm;
                    break;
            }

            // Check convergence
            if (relres < param.relTol) break;

            if (param.outLvl >= _PRINT_MORE) {
                Compres(updated_relres);
                RealRes(relres);
            }

            if (morestep >= param.restart) {
                if (param.outLvl > _PRINT_MIN) ZeroTol();
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            pk.SetValues(pk.GetSize(), 0.0);
            ++morestep;
        } // End of safe-guard check!

        // Save residual for next iteration
        abstmp = absres;

        // Compute z_k = B(r_k)
        if (type != STOP_REL_PRECRES) {
            if (pcflag == 1)
                ApplyPreconditioner(rk, zk, this->lop); // Apply preconditioner
            else
                zk = rk; // No preconditioner
        }

        // Compute beta_k = (z_k, r_k)/(z_{k-1}, r_{k-1})
        tmpb = zk.Dot(rk);
        beta = tmpb / tmpa;
        tmpa = tmpb;

        // Compute p_k = z_k + beta_k*p_{k-1}
        pk.Add(beta, 1.0, zk);
    }// End of main PCG loop

    FINISHED:// Finish iterative method
    if (param.outLvl > _PRINT_NONE)
        Final(param.numIter, param.maxIter, relres);

    param.numIter--;
    param.normInf=rk.NormInf();
    param.norm2=rk.Norm2();

    return errorCode;
}

// Clean preconditioner
void PCG::CleanPCD() {
    LOP lop;
    this->lop = lop;
    this->pcflag = 0;
}

// Apply preconditioner
void PCG::ApplyPreconditioner(const VEC &rk, VEC &zk, const LOP &lop) {
    zk = lop.MultVec(rk);
}

// Print some relevant information
void PCG::PrintInfo(const PRTLVL &prtlvl, const StopType &type, const INT &iter,
                    const DBL &relres, const DBL &absres, const DBL &factor) {
    if (prtlvl >= _PRINT_SOME) {
        if (iter > 0) {
            std::cout << iter << " | " << relres << " | " << absres << " | "
                      << factor << std::endl;
        } else {
            std::cout
                    << "-------------------------------------------------\n";
            switch (type) {
                case STOP_REL_RES:
                    std::cout
                            << "It Num |  ||r||/||b||  |  ||r||  |  Conv. Factor\n";
                    break;
                case STOP_REL_PRECRES:
                    std::cout
                            << "It Num |  ||r||_B/||b||_B |  ||r||_B  |  "
                               "Conv. Factor\n";
                    break;
                case STOP_MOD_REL_RES:
                    std::cout
                            << "It Num |  ||r||/||x||  |  ||r||  |  Conv. Factor\n";
                    break;
            }
            std::cout
                    << "-------------------------------------------------\n";
            std::cout << iter << " | " << relres << " | " << absres << std::endl;
        } // end if iter
    }
}

// Print some information about iteration number and relative residual
void PCG::Final(const INT &iter, const INT &maxit, const DBL &relres) {
    if (iter > maxit)
        std::cout << "### WARNING: MaxIt = " << maxit
                  << " reached with relative residual " << relres << std::endl;
    else if (iter >= 0) {
        std::cout << "Number of iterations = " << iter << " with relative residual "
                  << relres << std::endl;
    }
}