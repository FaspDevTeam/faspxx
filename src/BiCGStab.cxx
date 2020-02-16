/*! \file    BiCGStab.hxx
 *  \brief   Preconditioned BiCGStab class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "Iter.hxx"
#include "BiCGStab.hxx"

/// Allocate memory, assign param to this->param.
FaspRetCode BiCGStab::Setup(const LOP &A)
{
    const INT len = A.GetColSize();

    // Allocate memory for temporary vectors
    try {
        r0star.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        apj.SetValues(len, 0.0);
        asj.SetValues(len, 0.0);
        pj.SetValues(len, 0.0);
        rj.SetValues(len, 0.0);
        sj.SetValues(len, 0.0);
        ptmp.SetValues(len, 0.0);
        stmp.SetValues(len, 0.0);
        ms.SetValues(len, 0.0);
        mp.SetValues(len, 0.0);
        safe.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Set method type
    SetSolType(SOLType::BICGSTAB);

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Release additional memory allocated for CG.
void BiCGStab::Clean()
{
    // Nothing is needed for the moment!
}

/// Using the Preconditioned Bi-Conjugate Gradient Stabilized method.
FaspRetCode BiCGStab::Solve(const VEC &b, VEC &x)
{
    if ( params.verbose > PRINT_NONE ) std::cout << "Use BiCGStab to solve Ax=b ...\n";

    // Check whether vector space sizes match
    if ( x.GetSize() != A->GetColSize() || b.GetSize() != A->GetRowSize()
                                        || A->GetRowSize() != A->GetColSize() )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    const INT len = b.GetSize();
    const int maxStag = MAX_STAG_NUM; // maximum number of stagnation before quit
    const double solStagTol = 1e-4 * params.relTol; // solution stagnation tolerance

    int stagStep = 0, moreStep = 0;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;
    double alpha, beta, rjr0star, rjr0startmp, omega, tmp12;

    PrintHead();

    // Initialize iterative method
    numIter = 0;
    A->Apply(x, this->tmp); // A * x -> tmp
    this->rj.WAXPBY(1.0, b, -1.0, this->tmp);

    // Prepare for the main loop
    this->r0star = this->rj;  // r0_{*} = r0c
    this->pj     = this->rj;  // p0 = r0

    // Main BiCGStab loop
    while ( numIter < params.maxIter ) {

        // Start from minIter instead of 0
        if ( numIter == params.minIter ) {
            resAbs = rj.Norm2();
            denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if ( resRel < params.relTol || resAbs < params.absTol ) break;
        }

        if ( numIter >= params.minIter ) PrintInfo(numIter, resRel, resAbs, ratio);

        //---------------------------------------------
        // BiCGStab iteration starts from here
        //---------------------------------------------

        ++numIter; // iteration count

        /* alpha_{j} = (rj,r0star)/(P * A * pj,r0star) */
        rjr0star = this->rj.Dot(this->r0star);

        /* main computational work */
        A->Apply(this->pj, this->apj);
        ptmp.SetValues(len,0.0);
        pc->Solve(this->apj, this->ptmp);

        tmp12 = this->ptmp.Dot(this->r0star);
        if ( fabs(tmp12) > 1e-40 ) alpha = rjr0star / tmp12;
        else {
            FASPXX_WARNING("Divided by zero!") // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            break;
        }

        // sj = rj - alpha_{j} * P * A * p_{j}
        this->sj.WAXPBY(1.0, this->rj, -alpha, this->ptmp);

        // omega_j = (P * A * sj,sj)/(P * A * sj,P * A * sj)
        A->Apply(this->sj, this->asj);
        stmp.SetValues(len,0.0);
        pc->Solve(this->asj, this->stmp);
        omega = this->stmp.Dot(this->sj) / this->stmp.Dot(this->stmp);

        /* Update solution and residual */
        // x_{j+1} = x_{j} + alpha_{j} * P * pj + omega_j * P * s_{j}
        mp.SetValues(len,0.0);
        pc->Solve(this->pj, this->mp);
        ms.SetValues(len,0.0);
        pc->Solve(this->sj, this->ms);
        this->tmp.WAXPBY(alpha, this->mp, omega, this->ms);
        x.XPAY(1.0, this->tmp);

        // r_{j+1} = sj - omega_j * P * A * sj
        this->rj.WAXPBY(1.0, this->sj, -omega, this->stmp);

        //---------------------------------------------
        // One step of BiCGStab iteration ends here
        //---------------------------------------------

        // Apply several checks for safety
        if ( numIter >= params.minIter ) {
            // Compute norm of residual and output iteration information if needed
            resAbs = rj.Norm2();
            resRel = resAbs / denAbs;
            ratio  = resAbs / resAbsOld;

            // Save the best solution so far
            if ( numIter >= params.safeIter && resAbs < resAbsOld ) safe = x;

            // Apply stagnation checks if it converges slowly
            if ( ratio > KSM_CHK_RATIO && numIter > params.minIter ) {
                // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
                double xNorminf = x.NormInf();
                if ( xNorminf < solStagTol ) {
                    if ( params.verbose > PRINT_MIN )
                        FASPXX_WARNING("Iteration stopped due to x vanishes!")
                    errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                    break;
                }

                // Check II: if relative difference stagnated, try to restart
                double xRelDiff = fabs(alpha) * this->pj.Norm2() / x.Norm2();
                if ( (stagStep <= maxStag) && (xRelDiff < solStagTol) ) {
                    // Compute and update the residual before restart
                    A->Apply(x, this->rj);
                    this->rj.XPAY(-1.0, b);
                    resAbs = this->rj.Norm2();
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
                        this->pj.SetValues(len, 0.0);
                        ++stagStep;
                    }

                    if ( params.verbose > PRINT_SOME ) {
                        WarnDiffRes(xRelDiff, resRel);
                        FASPXX_WARNING("Iteration restarted due to stagnation!")
                    }
                } // End of stagnation check!
            } // End of check I and II

            // Check III: prevent false convergence
            if ( resRel < params.relTol ) {
                // Compute true residual r = b - Ax and update residual
                A->Apply(x, this->rj);
                this->rj.XPAY(-1.0, b);

                // Compute residual norms and check convergence
                double resRelOld = resRel;
                resAbs = rj.Norm2();
                resRel = resAbs / denAbs;
                if ( resRel < params.relTol || resAbs < params.absTol ) break;

                if ( params.verbose >= PRINT_MORE ) {
                    FASPXX_WARNING("False convergence!")
                    WarnCompRes(resRelOld);
                    WarnRealRes(resRel);
                }

                if ( moreStep >= params.restart ) {
                    // Note: restart has different meaning here
                    if ( params.verbose > PRINT_MIN )
                        FASPXX_WARNING("The tolerance might be too small!")
                    errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                    break;
                }

                // Prepare for restarting method
                this->pj.SetValues(len, 0.0);
                ++moreStep;
            } // End of check!
        }

        // Prepare for the next iteration
        if ( numIter < params.maxIter ) {
            // Save residual for next iteration
            resAbsOld = resAbs;

            // beta_j = (r_{j+1},r0^{*}) / (r_{j},r0^{*}) * alpha_j / omega_j
            rjr0startmp = rjr0star;
            rjr0star = this->rj.Dot(this->r0star);
            beta = rjr0star / rjr0startmp * alpha / omega;

            // p_{j+1} = r_{j+1} + beta_j * (p_{j} - omega_j * P * A * p_{j})
            this->tmp.WAXPBY(1.0, this->pj, -omega, this->ptmp);
            this->pj.WAXPBY(1.0, this->rj, beta, this->tmp);
        }

    } // End of main BiCGStab loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if ( not (numIter == params.minIter && numIter == params.maxIter) ) {
        this->norm2 = resAbs;
        this->normInf = rj.NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    // Restore the saved best iteration if needed
    if ( numIter > params.safeIter ) x = safe;

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/