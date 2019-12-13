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
    /// default constructor
    Identity() {};

    /// destructor
    ~Identity() {};

    /// Iterator
    virtual FaspRetCode Solve(const VEC &x, VEC &y);
};

/*! \class Jacobi
 *  \brief Jacobi iterator.
 */
class Jacobi : public SOL {

public :
    VEC diagInv;
    VEC b;
    double alpha;
    VEC tmp;

public:
    /// Default constructor.
    Jacobi() : alpha(0) {};

    /// Default destructor.
    ~Jacobi() {};

    /// Standard constructor.
    explicit Jacobi(const MAT &A, const VEC &b, double alpha);

    /// Solve Ax=b using the Jacobi method.
    virtual FaspRetCode Solve(const VEC &x, VEC &y);
};

#endif /* end if for __ITER_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/