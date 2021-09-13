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

#ifndef __CG_HEADER__ /*-- allow multiple inclusions --*/
#define __CG_HEADER__ /**< indicate CG.hxx has been included before */

// FASPXX header files
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

/*! \class CG
 *  \brief Preconditioned conjugate gradient method.
 */
class CG : public SOL
{
private:
    int len;  ///< dimension of the solution vector
    VEC rk;   ///< Work vector for residual
    VEC pk;   ///< Work vector for preconditioned residual
    VEC zk;   ///< Work vector for search direction
    VEC ax;   ///< Work vector for A * pk
    VEC safe; ///< Work vector for safe-guard

public:
    /// Default constructor.
    CG()
        : len(0)
        , rk(0)
        , pk(0)
        , zk(0)
        , ax(0)
        , safe(0){};

    /// Default destructor.
    ~CG() = default;

    /// Setup the CG method.
    FaspRetCode Setup(const LOP &A) override;

    /// Solve Ax=b using the CG method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

    /// Clean up CG data allocated during Setup.
    void Clean() override;
};

#endif /* end if for __CG_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/