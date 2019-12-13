/*! \file    Iter.cxx
 *  \brief   Simple iterative methods definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Dec/02/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "Iter.hxx"

/// Does nothing in preconditioning
FaspRetCode Identity::Solve(const VEC &x, VEC &y) {
    y = x;
    return FaspRetCode::SUCCESS;
}

/// Standard constructor.
Jacobi::Jacobi(const MAT &A, const VEC &b, double alpha) {
    A.GetDiag(diagInv);
    diagInv.Reciprocal();

    this->A = &A;
    this->b = b;
    this->alpha = alpha;

    try {
        tmp.SetValues(b.GetSize(), 0.0);
    } catch (std::bad_alloc &ex) {
        throw ("Insufficient memory!");
    }

}

/// Solve Ax=b using the Jacobi method.
FaspRetCode Jacobi::Solve(const VEC &x, VEC &y) {
    if (x.GetSize() != A->GetColSize() || A->GetRowSize() != b.GetSize() ||
        x.GetSize() != y.GetSize() || A->GetRowSize() != A->GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    const INT len = b.GetSize();
    const unsigned maxStag = 20; // maximum number of stagnation before quit
    const double solStagTol = 1e-4 * relTol; // solution stagnation tolerance
    const double solZeroTol = 1e-20; // solution close to zero tolerance
    unsigned stagStep = 0, moreStep = 0;
    double resAbs, tmpAbs, resRel, denAbs;
    double factor;

    // Initialize iterative method
    numIter = 0;
    y = x;

    A->Apply(y, tmp); // tmp = A * x

    tmp.XPAY(-1.0, b); // tmp = b - tmp

    // Compute initial residual
    resAbs = tmp.Norm2();
    denAbs = (1e-20 > resAbs) ? 1e-20 : resAbs;
    resRel = resAbs / denAbs;
    tmpAbs = resAbs; // Save residual for the next iteration

    // If initial residual is very small, no need to iterate
    PrintInfo(numIter, resRel, resAbs, 0.0);
    if (resRel < relTol || resAbs < absTol) goto FINISHED;

    // Main Jacobi loop
    while (numIter < maxIter) {

        ++numIter; // iteration count

        // Update solution and residual
        tmp.PointwiseMult(diagInv);
        y.XPAY(alpha, tmp); // y = y + alpha * tmp

        A->Apply(y, tmp); // tmp = A * x
        tmp.XPAY(-1.0, b); // tmp = b - tmp

        // Compute norm of residual and output iteration information if needed
        resAbs = tmp.Norm2();
        resRel = resAbs / denAbs;
        factor = resAbs / tmpAbs;
        PrintInfo(numIter, resRel, resAbs, factor);

        // Apply stagnation checks if it converges slowly
        if (factor > 0.95 && numIter > minIter) {
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            double xNormInf = x.NormInf();
            if (xNormInf < solZeroTol) {
                if (verbose > PRINT_MIN) FASPXX_WARNING(
                        "Iteration stopped due to x almost zero!");
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference close to zero, try to restart
            double xRelDiff = fabs(alpha) * this->tmp.Norm2() / y.Norm2();
            if ((stagStep <= maxStag) && (xRelDiff < solStagTol)) {
                // Compute and update the residual before restart
                A->Apply(y, this->tmp);
                tmp.XPAY(-1.0, b);
                resRel = resAbs / denAbs;
                if (verbose > PRINT_SOME) WarnRealRes(resRel);

                if (resRel < relTol) break; // already converged
                else {
                    if (stagStep >= maxStag) {
                        if (verbose > PRINT_MIN) FASPXX_WARNING(
                                "Iteration stopped due to stagnation!");
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->tmp.SetValues(len, 0.0);
                    ++stagStep;
                }

                if (verbose > PRINT_SOME) {
                    WarnDiffRes(xRelDiff, resRel);
                    FASPXX_WARNING("Iteration restarted due to stagnation!");
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence!!!
        if (resRel < relTol && numIter > minIter) {
            // Compute true residual r = b - A*x and update residual
            A->Apply(y, this->tmp);
            this->tmp.XPAY(-1.0, b);

            // Compute residual norms and check convergence
            double resRelOld = resRel;
            resAbs = tmp.Norm2();
            resRel = resAbs / denAbs;
            if (resRel < relTol) break;

            // If false converged, print out warning messages
            if (verbose >= PRINT_MORE) {
                WarnCompRes(resRelOld);
                WarnRealRes(resRel);
                std::cout<<_FASPXX_LOCATION_<<std::endl;
            }

            if (moreStep >= restart) { // Note: restart has different meaning here
                if (verbose > PRINT_MIN) FASPXX_WARNING(
                        "The tolerance might be too small!");
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            this->tmp.SetValues(len,0.0);
            ++moreStep;
        } // End of safe-guard check!

        // Prepare for the next iteration
        if(numIter<maxIter){
            // Save residual for next iteration
            tmpAbs=resAbs;

            A->Apply(y, tmp); // tmp = A * x
            tmp.XPAY(-1.0, b); // tmp = b - tmp
        }
    } // End of main Jacobi loop

    FINISHED: // Finish iterative method
    this->norm2=resAbs;
    this->normInf=tmp.NormInf();
    if(verbose>PRINT_NONE) PrintFinal();

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/