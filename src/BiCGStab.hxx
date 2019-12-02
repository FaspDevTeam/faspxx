/*! \file    BiCGStab.hxx
 *  \brief   Preconditioned BiCGStab class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __BICGSTAB_HEADER__    /*-- allow multiple inclusions --*/
#define __BICGSTAB_HEADER__    /**< indicate BiCGStab.hxx has been included before */

#include <cmath>
#include <cfloat>
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

/*! \class BiCGStab
 *  \brief Preconditioned BiCGStab method.
 */
class BiCGStab:public SOL{
private:
    VEC r0star;
    VEC tmp;
    VEC apj;
    VEC asj;
    VEC pj;
    VEC rj;
    VEC sj;
    VEC ptmp;
    VEC stmp;
    VEC mp;
    VEC ms;

public:
    /// constructor by default
    BiCGStab():r0star(0),tmp(0),apj(0),asj(0),pj(0),
        rj(0),sj(0),ptmp(0),stmp(0),mp(0),ms(0){};

    /// check and allocate memory
    FaspRetCode Setup(const LOP& A);

    /// solve by BiCGStab
    FaspRetCode Solve(const VEC& b, VEC& x);

    /// destructor
    ~BiCGStab() {};

};

#endif /* end if for __BICGSTAB_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/