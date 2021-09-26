/*! \file    Umfpack.hxx
 *  \brief   UMFPACK direct solver class declaration
 *  \author  Chensong Zhang
 *  \date    Sep/17/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __UMFPACK_HEADER__ /*-- allow multiple inclusions --*/
#define __UMFPACK_HEADER__ /**< indicate UMFPACK.hxx has been included before */

// FASPXX header files
#include "ErrorLog.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

// Other header files
#if WITH_UMFPACK
#include "umfpack.h"
#endif

/*! \class UMFPACK
 *  \brief Interface to external direct solver: Umfpack.
 */
class UMFPACK : public SOL
{
private:
    int     n;        ///< number of rows
    int     m;        ///< number of columns
    int     nnz;      ///< number of nonzeros
    int*    Ap;       ///< CSR row pointer for Fortran
    int*    Ai;       ///< CSR columen indices for Fortran
    double* Ax;       ///< CSR values for Fortran
    void*   Symbolic; ///< symbolic factorization from UMFPACK
    void*   Numeric;  ///< numeric factorization from UMFPACK

public:
    /// Default constructor.
    UMFPACK()
        : n(0)
        , m(0)
        , nnz(0)
        , Ap(nullptr)
        , Ai(nullptr)
        , Ax(nullptr)
        , Symbolic(nullptr)
        , Numeric(nullptr){};

    /// Default destructor.
    ~UMFPACK() = default;

    /// Setup the UMFPACK direct solver.
    FaspRetCode Setup(const MAT& A);

    /// Solve Ax=b using the the UMFPACK direct solver.
    FaspRetCode Solve(const VEC& b, VEC& x) override;

    /// Clean up UMFPACK data allocated during Setup.
    void Clean() override;
};

#endif /* end if for __UMFPACK_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/17/2021      Create file                          */
/*----------------------------------------------------------------------------*/