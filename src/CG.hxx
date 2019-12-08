/*! \file    CG.hxx
 *  \brief   Preconditioned CG class declaration
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Oct/11/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __CG_HEADER__      /*-- allow multiple inclusions --*/
#define __CG_HEADER__      /**< indicate CG.hxx has been included before */

#include <cmath>
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

/*! \class CG
 *  \brief Preconditioned conjugate gradient method.
 */
class CG: public SOL
{

private:
    VEC rk; ///< work vector for residual
    VEC pk; ///< work vector for preconditioned residual
    VEC zk; ///< work vector for search direction
    VEC ax; ///< work vector for A * pk

public:
    /// Default constructor。
    CG() : rk(0), pk(0), zk(0), ax(0){};

    /// Default destructor。
    ~CG(){};

    /// Setup the CG method.
    FaspRetCode Setup(const LOP& A);

    /// Solve Ax=b using the CG method.
    FaspRetCode Solve(const VEC& b, VEC& x);

    void Clean();
};

#endif /* end if for __CG_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/