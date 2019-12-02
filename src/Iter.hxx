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

#include "faspxx.hxx"
#include "SOL.hxx"

/*! \class Identity
 *  \brief Identity iterator.
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
    INT row; // Todo: 有什么用? -zcs
    INT col; // Todo: 有什么用? -zcs
    VEC diag;

public:
    /// default constructor
    Jacobi():row(0),col(0){};

    /// destructor
    ~Jacobi(){};

    /// constructor
    explicit Jacobi(INT row, INT col, std::vector<DBL> diag);

    /// Iterator
    virtual FaspRetCode Solve(const VEC& x, VEC& y);
};

#endif /* end if for __ITER_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/