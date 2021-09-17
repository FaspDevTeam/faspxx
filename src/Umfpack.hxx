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
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

/*! \class UMFPACK
 *  \brief Interface to external direct solver: Umfpack.
 */
class UMFPACK : public SOL
{
private:
    int len;  ///< dimension of the solution vector
    VEC work; ///< Work vector for residual

public:
    /// Default constructor.
    UMFPACK()
        : len(0)
        , work(0){};

    /// Default destructor.
    ~UMFPACK() = default;

    /// Setup the CG method.
    FaspRetCode Setup(const LOP &A) override;

    /// Solve Ax=b using the CG method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

    /// Clean up CG data allocated during Setup.
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