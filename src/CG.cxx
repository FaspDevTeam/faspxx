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

#include <iostream>
#include <iomanip>
#include "Iter.hxx"
#include "CG.hxx"

// Assign param to this->param
FaspRetCode CG::Setup(const LOP &A) {

    const INT len = A.GetColSize();

    this->A = &A;

    // Allocate memory for temporary vectors
    try {
        zk.SetValues(len, 0.0);
        pk.SetValues(len, 0.0);
        rk.SetValues(len, 0.0);
        ax.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    if (pc == nullptr) {
        pc = new Identity();
        mark=false;
    }

    return FaspRetCode::SUCCESS;
}

/// Solve by CG
FaspRetCode CG::Solve(const VEC &b, VEC &x) {

    // Check whether vector space sizes
    if (x.GetSize() != A->GetColSize() || b.GetSize() != A->GetRowSize() ||
        A->GetRowSize() != A->GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    const unsigned MaxStag = 20; // Maximum number of stagnations
    const int len = b.GetSize();
    const double maxdiff = 1e-4 * relTol; // Stagnation tolerance
    const double solinftol = 1e-20; // Infinity norm tolerance

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    double resAbs = 1e+20, tmpAbs = 1e+20;
    double resRel = 1e+20, denAbs = 1e+20;
    double factor, alpha, beta, tmpa, tmpb;

    // Initial iteration
    numIter = 0;

    A->Apply(x, rk); // A * x -> rk
    rk.XPAY(-1.0, b); // b - rk -> rk

    pc->Solve(rk, zk); // B(r_k) -> z_k

    // Compute initial residual
    tmpAbs = rk.Norm2();
    denAbs = (1e-20 > tmpAbs) ? 1e-20 : tmpAbs;
    resRel = tmpAbs / denAbs;

    // If initial residual is already small, no need to iterate
    if (resRel < relTol || tmpAbs < absTol) goto FINISHED;

    // Prepare for the main loop
    PrintInfo(numIter, resRel, tmpAbs, 0.0);
    pk = zk;
    tmpa = zk.Dot(rk);

    // Main CG loop
    while (numIter < maxIter) {
        numIter++; // iteration count

        A->Apply(pk, ax); // ax = A * p_k
        // alpha_k = (z_{k-1}, r_{k-1})/(A*p_{k-1},p_{k-1})
        tmpb = ax.Dot(pk);
        if (fabs(tmpb) > 1e-40) alpha = tmpa / tmpb;
        else {
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        x.AXPY(alpha, pk); // x_k = x_{k-1} + alpha_k*p_{k-1}
        rk.AXPY(-alpha, ax); // r_k = r_{k-1} - alpha_k*A*p_{k-1}

        // Compute norm of residual
        resAbs = rk.Norm2();
        resRel = resAbs / denAbs;
        factor = resAbs / tmpAbs;

        // Output iteration information if needed
        PrintInfo(numIter, resRel, tmpAbs, factor);

        if (factor > 0.9) // Only check when converge slowly
        {
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            double norminf = x.NormInf();
            if (norminf < solinftol) {
                if (verbose > PRINT_MIN) FASPXX_WARNING(
                        "Iteration stopped -- solution almost zero!");
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference stagnated, try to restart
            double reldiff = fabs(alpha) * this->pk.Norm2() / x.Norm2();
            if ((stagStep <= MaxStag) && (reldiff < maxdiff)) {
                if (verbose > PRINT_SOME) {
                    WarnDiffRes(reldiff, resRel);
                    FASPXX_WARNING("Iteration restarted -- stagnation!");
                }

                A->Apply(x, this->rk);
                this->rk.XPAY(-1.0, b);
                resAbs = this->rk.Norm2();
                resRel = resAbs / denAbs;
                if (verbose > PRINT_SOME) WarnRealRes(resRel);

                if (resRel < relTol) break;
                else {
                    if (stagStep >= MaxStag) {
                        if (verbose > PRINT_MIN) FASPXX_WARNING(
                                "Iteration stopped -- stagnation!");
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->pk.SetValues(len, 0.0);
                    ++stagStep;
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence
        if (resRel < relTol) {
            // Compute true residual r = b - Ax and update residual
            A->Apply(x, this->rk);
            this->rk.XPAY(-1.0, b);

            // Compute residual norms
            double updated_resRel = resRel;
            resAbs = rk.Norm2();
            resRel = resAbs / denAbs;

            // Check convergence
            if (resRel < relTol) break;

            if (verbose >= PRINT_MORE) {
                WarnCompRes(updated_resRel);
                WarnRealRes(resRel);
            }

            if (moreStep >= restart) {
                if (verbose > PRINT_MIN)
                    FASPXX_WARNING("The tolerence might be too small!");
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            this->pk.SetValues(len, 0.0);
            ++moreStep;
        } // End of safe-guard check!

        // Save residual for next iteration
        tmpAbs = resAbs;

        // Apply preconditioner z_k = B(r_k)
        pc->Solve(rk, zk);

        // Compute beta_k = (z_k, r_k)/(z_{k-1}, r_{k-1})
        tmpb = zk.Dot(rk);
        beta = tmpb / tmpa;
        tmpa = tmpb;

        // Compute p_k = z_k + beta_k*p_{k-1}
        pk.XPAY(beta, zk);

    } // End of main CG loop

FINISHED: // Finish iterative method
    this->normInf = rk.NormInf();
    this->norm2   = rk.Norm2();
    PrintFinal();

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/