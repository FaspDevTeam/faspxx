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
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"
#include "Param.hxx"
#include "ErrorLog.hxx"

class PCG: public SOL
{
private:
    LOP*   pc; /// preconditioner
    VEC    rk; /// work vector for residual
    VEC    zk; /// work vector for preconditioned residual
    VEC    pk; /// work vector for search direction
    VEC    ax; /// work vector for A*pk
    // TODO: it should also has a name member, like "Preconditioned Conjugate Gradient" --zcs

public:
    /// Default constructor
    PCG() : pc(nullptr), rk(0), pk(0), zk(0), ax(0){};

    /// Check and allocate memory for PCG
    FaspRetCode Setup(const LOP& A, const VEC& b, VEC& x); // TODO: Get rid of b and x --zcs

    /// Setup preconditioner operator
    void SetPC(LOP* lop); // TODO: PCD::Setup --zcs

    /// Solve by PCG
    FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x);

    /// Clean up preconditioner operator
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