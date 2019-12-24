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

#include <ios>
#include "Iter.hxx"
#include "BiCGStab.hxx"

/// Allocate memory, assign param to this->param.
FaspRetCode BiCGStab::Setup(const LOP &A) {
    const INT len = A.GetColSize();

    this->SetSolType(SOLType::BICGSTAB);

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

    // Setup the coefficient matrix
    this->A = &A;

    // If pc is null, then use regular BiCGStab without preconditioning
    if ( pc == nullptr ) {
        withPC = false;
        pc = new Identity();
    }

    return FaspRetCode::SUCCESS;
}

/// Release memory.
void BiCGStab::Clean() {
    if ( !withPC ) delete pc;
}

/// Using the Preconditioned Bi-Conjugate Gradient Stabilized method.
FaspRetCode BiCGStab::Solve(const VEC &b, VEC &x) {
    if ( x.GetSize() != A->GetColSize() || b.GetSize() != A->GetRowSize()
        || A->GetRowSize() != A->GetColSize() )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    const INT len = b.GetSize();
    const unsigned maxStag = MAX_STAG_NUM; // maximum number of stagnation before quit
    const double solStagTol = 1e-4 * relTol; // solution stagnation tolerance
    const double solZeroTol = CLOSE_ZERO; // solution close to zero tolerance
    unsigned stagStep = 0, moreStep = 0;
    double resAbsOld, resAbs, tmpAbs, resRel, denAbs;
    double alphaj, betaj, rjr0star, rjr0startmp, omegaj;
    double tmp12, ratio;

    // Initialize iterative method
    numIter = 0;

    // r0 = b - A * x_{0}
    A->Apply(x, this->tmp);
    this->rj.WAXPBY(1.0, b, -1.0, this->tmp);

    // Compute initial residual
    resAbs = this->rj.Norm2();
    denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
    resRel = resAbs / denAbs;
    tmpAbs = resAbs; // save residual for the next iteration

    // If initial residual is very small, no need to iterate
    PrintInfo(numIter, resRel, resAbs, 0.0);
    if ( resRel < relTol || resAbs < absTol ) goto FINISHED;

    // Prepare for the main loop
    // r0_{*} = r0c
    this->r0star = this->rj;

    // p0 = r0
    this->pj = this->rj;

    // Main CG loop
    while ( numIter < maxIter ) {

        ++numIter; // iteration count

        /* alpha_{j} = (rj,r0star)/(P * A * pj,r0star) */
        rjr0star = this->rj.Dot(this->r0star);

        /* main computational work */
        A->Apply(this->pj, this->apj);
        pc->Solve(this->apj, this->ptmp);

        tmp12 = this->ptmp.Dot(this->r0star);
        if ( fabs(tmp12) > 1e-40 ) alphaj = rjr0star / tmp12;
        else {
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        // sj = rj - alpha_{j} * P * A * p_{j}
        this->sj.WAXPBY(1.0, this->rj, -alphaj, this->ptmp);

        // omegaj = (P * A * sj,sj)/(P * A * sj,P * A * sj)
        A->Apply(this->sj, this->asj);
        pc->Solve(this->asj, this->stmp);
        omegaj = this->stmp.Dot(this->sj) / this->stmp.Dot(this->stmp);

        /* Update solution and residual */
        // x_{j+1} = x_{j} + alpha_{j} * P * pj + omegaj * P * s_{j}
        pc->Solve(this->pj, this->mp);
        pc->Solve(this->sj, this->ms);
        this->tmp.WAXPBY(alphaj, this->mp, omegaj, this->ms);
        x.XPAY(1.0, this->tmp);

        // r_{j+1} = sj - omegaj * P * A * sj
        this->rj.WAXPBY(1.0, this->sj, -omegaj, this->stmp);

        // Apply several checks for safety
        if ( numIter >= minIter ) {
            // Compute norm of residual and output iteration information if needed
            resAbsOld = resAbs;
            resAbs = rj.Norm2();
            resRel = resAbs / denAbs;
            ratio = resAbs / tmpAbs;
            PrintInfo(numIter, resRel, resAbs, ratio);

            // Save the best solution so far
            if ( numIter >= safeIter && resAbs < resAbsOld ) safe = x;

            // Apply stagnation checks if it converges slowly
            if ( ratio > KSM_CHK_RATIO && numIter > minIter ) {
                // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
                double xNorminf = x.NormInf();
                if ( xNorminf < solStagTol ) {
                    if ( verbose > PRINT_MIN ) FASPXX_WARNING(
                            "Iteration stopped -- solution almost zero!");
                    errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                    break;
                }

                // Check II: if relative difference stagnated, try to restart
                double xRelDiff = fabs(alphaj) * this->pj.Norm2() / x.Norm2();
                if ( (stagStep <= maxStag) && (xRelDiff < solStagTol) ) {
                    // Compute and update the residual before restart
                    A->Apply(x, this->rj);
                    this->rj.XPAY(-1.0, b);
                    resAbs = this->rj.Norm2();
                    resRel = resAbs / denAbs;
                    if ( verbose > PRINT_SOME ) {
                        FASPXX_WARNING("Iteration stagnate!");
                        WarnRealRes(resRel);
                    }

                    if ( resRel < relTol ) break; // already converged
                    else {
                        if ( stagStep >= maxStag ) {
                            if (verbose > PRINT_MIN) FASPXX_WARNING(
                                    "Iteration stopped due to stagnation!");
                            errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                            break;
                        }
                        this->pj.SetValues(len, 0.0);
                        ++stagStep;
                    }

                    if ( verbose > PRINT_SOME ) {
                        WarnDiffRes(xRelDiff, resRel);
                        FASPXX_WARNING("Iteration restarted -- stagnation!");
                    }
                } // End of stagnation check!
            } // End of check I and II

            // Check III: prevent false convergence
            if ( resRel < relTol ) {
                // Compute true residual r = b - Ax and update residual
                A->Apply(x, this->rj);
                this->rj.XPAY(-1.0, b);

                // Compute residual norms and check convergence
                double resRelOld = resRel;
                resAbs = rj.Norm2();
                resRel = resAbs / denAbs;
                if ( resRel < relTol ) break;

                if ( verbose >= PRINT_MORE ) {
                    FASPXX_WARNING("False convergence!");
                    WarnCompRes(resRelOld);
                    WarnRealRes(resRel);
                }

                if ( moreStep >= restart ) { // Note: restart has different
                    // meaning here
                    if (verbose > PRINT_MIN) FASPXX_WARNING(
                            "The tolerance might be too small!");
                    errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                    break;
                }

                // Prepare for restarting method
                this->pj.SetValues(len, 0.0);
                ++moreStep;
            } // End of check!
        }

        // Prepare for the next iteration
        if ( numIter < maxIter ) {
            // Save residual for next iteration
            tmpAbs = resAbs;

            // Compute betaj = (r_{j+1},r0^{*})/(r_{j},r0^{*}) *
            // \frac{alpha_{j}}{omega_{j}}
            rjr0startmp = rjr0star;
            rjr0star = this->rj.Dot(this->r0star);
            betaj = rjr0star / rjr0startmp * alphaj / omegaj;

            // p_{j+1} = r_{j+1} + betaj * (p_{j} - omegaj * P * A * p_{j})
            this->tmp.WAXPBY(1.0, this->pj, -omegaj, this->ptmp);
            this->pj.WAXPBY(1.0, this->rj, betaj, this->tmp);
        }

    } // End of main PCG loop

    FINISHED: // Finish iterative method
    this->norm2 = resAbs;
    this->normInf = rj.NormInf();
    if ( verbose > PRINT_NONE ) PrintFinal();

    // Restore the saved best iteration if needed
    if ( numIter > safeIter ) x = safe;

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/