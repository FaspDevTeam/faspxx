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

#include <vector>
#include "faspxx.hxx"
#include "SOL.hxx"
#include "MAT.hxx"

/*! \class Identity
 *  \brief Identity operator.
 */
class Identity : public SOL {

public:
    /// default constructor
    Identity(){};

    /// destructor
    ~Identity(){};

    /// Iterator
    virtual FaspRetCode Solve(const VEC& x, VEC& y);
};

/*! \class Jacobi
 *  \brief Jacobi iterator.
 */
class Jacobi : public SOL{

public :
    MAT tmpMat;
    VEC tmpVec;
    const VEC *tmpb;
    VEC tmp;
    VEC rk;
    VEC xTmp;

public:
    /// Default constructor.
    Jacobi(){};

    /// Default destructor.
    ~Jacobi(){};

    /// Standard constructor.
    explicit Jacobi(const MAT &A, const VEC &b);

    /// Solve Ax=b using the Jacobi method.
    virtual FaspRetCode Solve(const VEC& x, VEC& y);
};

#endif /* end if for __ITER_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/