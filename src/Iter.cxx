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

// FASPXX header files
#include "Iter.hxx"

/// Does nothing in preconditioning
FaspRetCode Identity::Solve(const VEC& b, VEC& x)
{
    x = b;
    return FaspRetCode::SUCCESS;
}

/// Set the weight for the Jacobi method.
void Jacobi::SetWeight(const DBL weight) { this->weight = weight; }

/// Setup Jacobi preconditioner.
FaspRetCode Jacobi::Setup(const MAT& A)
{
    // Set solver type
    SetSolType(SOLType::SOLVER_JACOBI);

    // Allocate memory for temporary vectors
    try {
        work.SetValues(A.GetColSize(), 0.0);
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Get diagonal and compute its scaled reciprocal = 1 ./ diag * weight
    A.GetDiag(diagInv);
    diagInv.Reciprocal();
    diagInv.Scale(weight);

    // Print used parameters if necessary
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Solve Ax=b using the Jacobi method. Don't check problem sizes.
FaspRetCode Jacobi::Solve(const VEC& b, VEC& x)
{
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;

    PrintHead();

    // Initialize iterative method
    numIter = 0;

    // Main Jacobi loop
    while (numIter < params.maxIter) {

        // Update residual r = b - A*x
        A->Residual(b, x, work);

        // Compute norm of residual and check whether it converges
        if (numIter >= params.minIter) {
            resAbs = work.Norm2();
            if (numIter == params.minIter)
                denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if (resRel < params.relTol || resAbs < params.absTol) break;

            ratio     = resAbs / resAbsOld;
            resAbsOld = resAbs;
            PrintInfo(numIter, resRel, resAbs, ratio);
        }

        //---------------------------------------------
        // Jacobi iteration starts from here
        //---------------------------------------------

        work.PointwiseMult(diagInv); // r = weight * r ./ d
        x += work;                   // x = x + weight * r
        ++numIter;                   // iteration count

        //---------------------------------------------
        // One step of Jacobi iteration ends here
        //---------------------------------------------

    } // End of main Jacobi loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (not(numIter == params.minIter && numIter == params.maxIter)) {
        A->Residual(b, x, work); // Update final residual
        this->norm2 = resAbs = work.Norm2();
        this->normInf        = work.NormInf();
        resRel               = resAbs / denAbs;
        ratio                = resAbs / resAbsOld;
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    return errorCode;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Dec/02/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/