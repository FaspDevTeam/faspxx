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
FaspRetCode Identity::Solve(const VEC &b, VEC &x)
{
    x = b;
    return FaspRetCode::SUCCESS;
}

/// Standard constructor.
Jacobi::Jacobi(const MAT &A, double alpha)
{
    A.GetDiag(diagInv);
    diagInv.Reciprocal();

    this->A = &A;
    this->alpha = alpha;

    try {
        rk.SetValues(A.GetColSize(), 0.0);
    } catch (std::bad_alloc &ex) {
        throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
    }
}

/// Solve Ax=b using the Jacobi method.
FaspRetCode Jacobi::Solve(const VEC &b, VEC &x)
{
    if ( params.verbose > PRINT_NONE ) std::cout << "Use Jacobi to solve Ax=b ...\n";

    // Check whether vector space sizes
    if ( x.GetSize() != A->GetColSize() || b.GetSize() != A->GetRowSize()
                                        || A->GetRowSize() != A->GetColSize() )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;

    PrintHead();

    // Initialize iterative method
    numIter = 0;
    A->Apply(x, rk); // rk = A * x
    rk.XPAY(-1.0, b); // rk = b - rk

    // Main Jacobi loop
    while ( numIter < params.maxIter ) {

        // Compute residual norm from minIter
        if ( numIter == params.minIter ) {
            resAbs = rk.Norm2();
            denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if ( resRel < params.relTol || resAbs < params.absTol ) break;
        }

        // Print iteration information if verbose > 0
        if ( numIter >= params.minIter )
            PrintInfo(numIter, resRel, resAbs, ratio);

        ++numIter; // iteration count

        //---------------------------------------------
        // Jacobi iteration starts from here
        //---------------------------------------------

        rk.PointwiseMult(diagInv); // rk = rk ./ dk
        x.AXPY(alpha, rk); // x = x + alpha * rk

        //---------------------------------------------
        // One step of Jacobi iteration ends here
        //---------------------------------------------

        // Prepare for the next iteration
        if ( numIter < params.maxIter ) {
            // Update residual
            A->Apply(x, rk); // rk = A * x
            rk.XPAY(-1.0, b); // rk = b - rk

            // Compute norm of residual and check whether it converges
            if ( numIter >= params.minIter ) {
                resAbs = rk.Norm2();
                resRel = resAbs / denAbs;
                if ( resRel < params.relTol || resAbs < params.absTol ) break;

                ratio = resAbs / resAbsOld;
                resAbsOld = resAbs;
            }
        }
    } // End of main Jacobi loop

    // Finish iterative method
    this->norm2 = resAbs;
    this->normInf = rk.NormInf();
    PrintFinal(numIter, resRel, resAbs, ratio);

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/