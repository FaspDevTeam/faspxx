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
FaspRetCode Identity::Solve(const VEC& b, VEC& x)
{
    x = b;
    return FaspRetCode::SUCCESS;
}

/// Setup Jacobi preconditioner.
FaspRetCode Jacobi::Setup(const MAT& A)
{
    const INT len = A.GetColSize();
    SetSolType(SOLType::Jacobi);

    // Allocate memory for temporary vectors
    try {
        rk.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Get diagonal and compute its reciprocal
    A.GetDiag(diagInv);
    diagInv.Reciprocal();

    // Setup the coefficient matrix
    this->A = &A;
    this->omega = params.weight;

    // Print used parameters if necessary
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Solve Ax=b using the Jacobi method.
FaspRetCode Jacobi::Solve(const VEC& b, VEC& x)
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

    // Main Jacobi loop
    while ( numIter < params.maxIter ) {

        // Update residual
        A->Apply(x, rk); // rk = A * x
        rk.XPAY(-1.0, b); // rk = b - rk

        // Compute norm of residual and check whether it converges
        if ( numIter >= params.minIter ) {
            resAbs = rk.Norm2();
            if ( numIter == params.minIter )
                denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if ( resRel < params.relTol || resAbs < params.absTol ) break;

            ratio = resAbs / resAbsOld;
            resAbsOld = resAbs;
            PrintInfo(numIter, resRel, resAbs, ratio);
        }

        //---------------------------------------------
        // Jacobi iteration starts from here
        //---------------------------------------------

        ++numIter;                 // iteration count
        rk.PointwiseMult(diagInv); // rk = rk ./ dk
        x.AXPY(omega, rk);         // x = x + omega * rk

        //---------------------------------------------
        // One step of Jacobi iteration ends here
        //---------------------------------------------

    } // End of main Jacobi loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if ( not (numIter == params.minIter && numIter == params.maxIter) ) {
        this->norm2 = resAbs;
        this->normInf = rk.NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/