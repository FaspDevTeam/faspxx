/*! \file    Umfpack.cxx
 *  \brief   UMFPACK direct solver class definition
 *  \author  Chensong Zhang
 *  \date    Sep/17/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "Umfpack.hxx"

/// Allocate memory, setup coefficient matrix of the linear system.
FaspRetCode UMFPACK::Setup(const MAT& A)
{
#if WITH_UMFPACK
    int status;

    // Set solver type
    SetSolType(SOLType::UMFPACK);

    // Setup the coefficient matrix
    this->A = &A;

    // Setup data for UMFPACK ordering (column-major)
    MAT Atrans = A;
    Atrans.TransInPlace();

    n   = Atrans.GetColSize();
    m   = Atrans.GetRowSize();
    nnz = Atrans.GetNNZ();

    if (Ap != nullptr) FASPXX_ABORT("Pointer Ap is not null!");
    Ap = Atrans.GetRowPtr();

    if (Ai != nullptr) FASPXX_ABORT("Pointer Ai is not null!");
    Ai = Atrans.GetColInd();

    if (Ax != nullptr) FASPXX_ABORT("Pointer Ax is not null!");
    Ax = Atrans.GetValues();

    // Call factorizations; see 6.11 in UMFPACK manual for error code
    if (Symbolic != nullptr) FASPXX_ABORT("Pointer Symbolic is not null!");
    status = umfpack_di_symbolic(n, n, Ap, Ai, Ax, &Symbolic, NULL, NULL);
    if (status != 0) return FaspRetCode::ERROR_DSOLVER_SETUP;

    if (Numeric != nullptr) FASPXX_ABORT("Pointer Numeric is not null!");
    status = umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, NULL, NULL);
    if (status != 0) return FaspRetCode::ERROR_DSOLVER_SETUP;

    umfpack_di_free_symbolic(&Symbolic);
    return FaspRetCode::SUCCESS;
#else
    FASPXX_ABORT("External package UMFPACK not found!");
#endif
}

/// Using the UMFPACK direct solver. Don't check problem sizes.
FaspRetCode UMFPACK::Solve(const VEC& b, VEC& x)
{
#if WITH_UMFPACK
    int status;
    status = umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, &x[0], &b[0], Numeric, NULL, NULL);
    if (status == 0) return FaspRetCode::SUCCESS;
#else
    FASPXX_ABORT("External package UMFPACK not found!");
#endif
    return FaspRetCode::ERROR_SOLVER_TYPE;
}

/// Clean up temp memory allocated for UMFPACK.
//! Note: Must be called before reusing the class.
void UMFPACK::Clean()
{
    delete[] Ap;
    delete[] Ai;
    delete[] Ax;
    Ap = nullptr;
    Ai = nullptr;
    Ax = nullptr;

#if WITH_UMFPACK
    umfpack_di_free_numeric(&Numeric);
#endif
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/17/2021      Create file                          */
/*----------------------------------------------------------------------------*/