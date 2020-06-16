/*! \file    CG.cxx
 *  \brief   Preconditioned CG class definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Oct/13/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "Iter.hxx"
#include "CG.hxx"

/// Allocate memory, setup coefficient matrix of the linear system.
FaspRetCode CG::Setup(const LOP &A)
{
    // Set solver type
    SetSolType(SOLType::CG);

    // Allocate memory for temporary vectors
    try {
        const INT len = A.GetColSize();
        zk.SetValues(len, 0.0);
        pk.SetValues(len, 0.0);
        rk.SetValues(len, 0.0);
        ax.SetValues(len, 0.0);
        safe.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Release additional memory allocated for CG.
void CG::Clean()
{
    // Nothing is needed for the moment!
}

/// Using the Preconditioned Conjugate Gradient method. Don't check problem sizes.
FaspRetCode CG::Solve(const VEC &b, VEC &x)
{
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Local variables
    const INT len = b.GetSize();
    const int maxStag = MAX_STAG_NUM; // max number of stagnation checks
    const double solStagTol = 1e-4 * params.relTol; // solution stagnation tolerance
    const double solZeroTol = CLOSE_ZERO; // solution close to zero tolerance

    int stagStep = 0, moreStep = 0;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;
    double alpha, beta, tmpa, tmpb;

    PrintHead();

    // Initialize iterative method
    numIter = 0;
    A->Apply(x, rk); // A * x -> rk
    rk.XPAY(-1.0, b); // b - rk -> rk

    // Preconditioned search direction
    zk.SetValues(len,0.0); // initialize zk = 0
    pc->Solve(rk, zk); // preconditioning: B(r_k) -> z_k

    // Prepare for the main loop
    pk = zk;
    tmpa = zk.Dot(rk);

    // Main CG loop
    while ( numIter < params.maxIter ) {

        // Start from minIter instead of 0
        if ( numIter == params.minIter ) {
            resAbs = rk.Norm2();
            denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if (resRel < params.relTol || resAbs < params.absTol) break;
        }

        if ( numIter >= params.minIter ) PrintInfo(numIter, resRel, resAbs, ratio);

        //---------------------------------------------
        // CG iteration starts from here
        //---------------------------------------------

        ++numIter; // iteration count

        A->Apply(pk, ax); // ax = A * p_k, main computational work

        // alpha_k = (z_{k-1}, r_{k-1})/(A*p_{k-1},p_{k-1})
        tmpb = ax.Dot(pk);
        if ( fabs(tmpb) > CLOSE_ZERO * CLOSE_ZERO )
            alpha = tmpa / tmpb;
        else {
            FASPXX_WARNING("Divided by zero!")
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            break;
        }

        // Update solution and residual
        x.AXPY(alpha, pk);   // x_k = x_{k-1} + alpha_k*p_{k-1}
        rk.AXPY(-alpha, ax); // r_k = r_{k-1} - alpha_k*A*p_{k-1}

        //---------------------------------------------
        // One step of CG iteration ends here
        //---------------------------------------------

        // Apply several checks for robustness
        if ( numIter >= params.minIter ) {
            // Compute norm of residual and output iteration information if needed
            resAbs = rk.Norm2();
            resRel = resAbs / denAbs;
            ratio  = resAbs / resAbsOld; // convergence ratio between two steps

            // Save the best solution so far
            if ( numIter >= params.safeIter && resAbs < resAbsOld ) safe = x;

            // Apply stagnation checks if it converges slowly
            if ( ratio > KSM_CHK_RATIO ) {
                // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
                double xNormInf = x.NormInf();
                if (xNormInf < solZeroTol) {
                    if (params.verbose > PRINT_MIN)
                        FASPXX_WARNING("Iteration stopped due to x vanishes!")
                    errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                    break;
                }

                // Check II: if relative difference close to zero, try to restart
                double xRelDiff = fabs(alpha) * this->pk.Norm2() / x.Norm2();
                if ( (stagStep <= maxStag) && (xRelDiff < solStagTol) ) {
                    // Compute and update the residual before restart
                    A->Apply(x, this->rk);
                    this->rk.XPAY(-1.0, b);
                    resAbs = this->rk.Norm2();
                    resRel = resAbs / denAbs;
                    if ( params.verbose > PRINT_SOME ) {
                        FASPXX_WARNING("Possible iteration stagnate!")
                        WarnRealRes(resRel);
                    }

                    if ( resRel < params.relTol || resAbs < params.absTol ) break;
                    else {
                        if ( stagStep >= maxStag ) {
                            if ( params.verbose > PRINT_MIN )
                                FASPXX_WARNING("Iteration stopped due to stagnation!")
                            errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                            break;
                        }
                        this->pk.SetValues(len, 0.0);
                        ++stagStep;
                    }

                    if ( params.verbose > PRINT_SOME ) {
                        WarnDiffRes(xRelDiff, resRel);
                        FASPXX_WARNING("Iteration restarted due to stagnation!")
                    }
                } // End of stagnation check!
            } // End of check I and II

            // Check III: prevent false convergence!!!
            if ( resRel < params.relTol ) {
                // Compute and update the true residual r = b - Ax
                A->Apply(x, this->rk);
                this->rk.XPAY(-1.0, b);

                // Compute residual norms and check convergence
                double resRelOld = resRel;
                resAbs = rk.Norm2();
                resRel = resAbs / denAbs;
                if ( resRel < params.relTol || resAbs < params.absTol ) break;

                // If false converged, print out warning messages
                if ( params.verbose >= PRINT_MORE ) {
                    FASPXX_WARNING("False convergence!");
                    WarnCompRes(resRelOld);
                    WarnRealRes(resRel);
                }

                if ( moreStep >= params.restart ) {
                    // Note: restart has different meaning here
                    if ( params.verbose > PRINT_MIN )
                        FASPXX_WARNING("The tolerance is too small!")
                    errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                    break;
                }

                // Prepare for restarting method
                this->pk.SetValues(0.0);
                ++moreStep;
            } // End of check!
        }

        // Prepare for the next iteration
        if ( numIter < params.maxIter ) {
            // Save the residual for next iteration
            resAbsOld = resAbs;

            // Apply preconditioner z_k = B(r_k)
            zk.SetValues(len,0.0);
            pc->Solve(rk, zk);

            // Compute beta_k = (z_k, r_k) / (z_{k-1}, r_{k-1})
            tmpb = zk.Dot(rk);
            beta = tmpb / tmpa;
            tmpa = tmpb;

            // Compute p_k = z_k + beta_k*p_{k-1}
            pk.XPAY(beta, zk);
        }

    } // End of main CG loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if ( not (numIter == params.minIter && numIter == params.maxIter) ) {
        this->norm2 = resAbs;
        this->normInf = rk.NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    // Restore the saved best iteration if needed
    if ( numIter > params.safeIter ) x = safe;

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/