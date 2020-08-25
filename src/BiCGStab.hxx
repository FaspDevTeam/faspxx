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

#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

/*! \class BiCGStab
 *  \brief Preconditioned bi-conjugate gradient stabilized method.
 */
class BiCGStab:public SOL
{
private:
    int len;
    VEC r0star;  ///< Work vector for r0*
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
    VEC safe;    ///< Work vector for safe-guard

public:
    /// Default constructor.
    BiCGStab() : len(0), r0star(0), tmp(0), apj(0), asj(0), pj(0),
                 rj(0), sj(0), ptmp(0), stmp(0), mp(0),
                 ms(0),safe(0) { };

    /// Default destructor.
    ~BiCGStab() = default;

    /// Setup the BiCGStab method.
    FaspRetCode Setup(const LOP& A) override;

    /// Clean up CG data allocated during Setup.
    void Clean() override;

    /// Solve Ax=b using the BiCGStab method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;
};

#endif /* end if for __BICGSTAB_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/