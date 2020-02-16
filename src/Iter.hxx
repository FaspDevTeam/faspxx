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

#ifndef __ITER_HEADER__      /*-- allow multiple inclusions --*/
#define __ITER_HEADER__      /**< indicate Iter.hxx has been included before */

#include <cmath>
#include "faspxx.hxx"
#include "SOL.hxx"
#include "MAT.hxx"

/*! \class Identity
 *  \brief Identity operator.
 */
class Identity : public SOL {

public:
    /// Default constructor
    Identity() = default;

    /// Default destructor
    ~Identity() = default;

    /// Iterator
    FaspRetCode Solve(const VEC& b, VEC& x) override;
};

/*! \class Jacobi
 *  \brief Jacobi iterator.
 */
class Jacobi : public SOL {

public :
    double   omega;     ///< Weight for damped or weighted Jacobi
    VEC      diagInv;   ///< Inverse of diagonal entries
    VEC      rk;        ///< Work array for the residual

public:
    /// Default constructor.
    Jacobi() : omega(1.0), diagInv(1.0), rk(0.0) {};

    /// Default destructor.
    ~Jacobi() = default;

    /// Setup the Jacobi method.
    FaspRetCode Setup(const MAT& A);

    /// Clean up Jacobi data allocated during Setup.
    void Clean() override {};

    /// Solve Ax=b using the Jacobi method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;
};

#endif /* end if for __ITER_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/