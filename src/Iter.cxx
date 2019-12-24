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
    if ( x.GetSize() != A->GetColSize() || A->GetRowSize() != b.GetSize() ||
        x.GetSize() != y.GetSize() || A->GetRowSize() != A->GetColSize() )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    double resAbs, tmpAbs, resRel, denAbs, ratio;

    // Initialize iterative method
    numIter = 0;
    y = x;

    A->Apply(y, tmp); // tmp = A * x

    tmp.XPAY(-1.0, b); // tmp = b - tmp

    // Compute initial residual
    resAbs = tmp.Norm2();
    denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
    resRel = resAbs / denAbs;
    tmpAbs = resAbs; // Save residual for the next iteration

    // If initial residual is very small, no need to iterate
    PrintInfo(numIter, resRel, resAbs, 0.0);
    if ( resRel < params.relTol || resAbs < params.absTol ) goto FINISHED;

    // Main Jacobi loop
    while ( numIter < params.maxIter ) {

        ++numIter; // iteration count

        // Update solution and residual
        tmp.PointwiseMult(diagInv);
        y.XPAY(alpha, tmp); // y = y + alpha * tmp

        A->Apply(y, tmp); // tmp = A * x
        tmp.XPAY(-1.0, b); // tmp = b - tmp

        // Compute norm of residual and output iteration information if needed
        resAbs = tmp.Norm2();
        resRel = resAbs / denAbs;
        ratio = resAbs / tmpAbs;
        PrintInfo(numIter, resRel, resAbs, ratio);

        // Prepare for the next iteration
        if( numIter < params.maxIter ){
            // Save residual for next iteration
            tmpAbs=resAbs;

            A->Apply(y, tmp); // tmp = A * x
            tmp.XPAY(-1.0, b); // tmp = b - tmp
        }
    } // End of main Jacobi loop

    FINISHED: // Finish iterative method
    this->norm2=resAbs;
    this->normInf=tmp.NormInf();
    if( params.verbose > PRINT_NONE ) PrintFinal();

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/