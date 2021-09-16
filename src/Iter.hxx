/*! \file    Iter.hxx
 *  \brief   Simple iterative methods declaration
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Dec/02/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __ITER_HEADER__ /*-- allow multiple inclusions --*/
#define __ITER_HEADER__ /**< indicate Iter.hxx has been included before */

// Standard header files
#include <cmath>

// FASPXX header files
#include "Faspxx.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

/*! \class Identity
 *  \brief Identity iterator, do nothing solver.
 */
class Identity : public SOL
{
public:
    /// Default constructor
    Identity() = default;

    /// Default destructor
    ~Identity() = default;

    /// Iterator
    FaspRetCode Solve(const VEC &b, VEC &x) override;
};

/*! \class Jacobi
 *  \brief Jacobi iterator.
 */
class Jacobi : public SOL
{
private:
    double omega;   ///< Weight for damped or weighted Jacobi
    VEC    diagInv; ///< Inverse of diagonal entries
    VEC    w;       ///< Work array for the residual

public:
    /// Default constructor.
    Jacobi()
        : omega(1.0)
        , diagInv(1.0)
        , w(0.0){};

    /// Default destructor.
    ~Jacobi() = default;

    /// Setup the Jacobi method.
    FaspRetCode Setup(const MAT &A);

    /// Clean up Jacobi data allocated during Setup.
    void Clean() override{};

    /// Solve Ax=b using the Jacobi method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;
};

#endif /* end if for __ITER_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Dec/02/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/