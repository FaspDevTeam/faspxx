/*! \file PCG.hxx
 *  \brief PCG class declaration
 *  \author Chensong Zhang, Kailei Zhang
 *  \date Oct/11/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __PCG_HEADER__      /*-- allow multiple inclusions --*/
#define __PCG_HEADER__      /**< indicate PCG.hxx has been included before */

#include <cmath>
#include "MAT.hxx"
#include "LOP.hxx"
#include "ErrorLog.hxx"
#include "SOL.hxx"


class PCG: public SOL
{
private:
    VEC rk; /// work vector for residual
    VEC pk; /// work vector for preconditioned residual
    VEC zk; /// work vector for search direction
    VEC ax; /// work vector for A * pk
    // TODO: it should also has a name member, like "Preconditioner Conjugate Gradient" --zcs

public:
    /// constructor by default
    PCG() : rk(0), pk(0),
            zk(0), ax(0){};

    /// check and allocate memory
    FaspRetCode Setup(const Mat& A,const VEC& b,VEC& x);

    /// build preconditioner operator
    void SetPC(LOP* lop); // TODO: PCD::Setup --zcs

    /// solve by PCG
    FaspRetCode Solve(const VEC& b, VEC& x);

    /// clean preconditioner operator
    void CleanPCD(); // TODO: PCD::Clean --zcs

    /// Release temporary memory
    void Clean();

    /// destructor
    ~PCG(){};
};

#endif /* end if for __PCG_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
