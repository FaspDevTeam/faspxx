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
#include "Param.hxx"
#include "ErrorLog.hxx"

class PCG {
private:
    const LOP* lop;
    VEC rk;
    VEC pk;
    VEC zk;
    VEC ax;

    /// Print out iteration information for iterative solvers
    void PrintInfo(const PRTLVL& prtlvl, const INT& iter, const DBL& relres,
                   const DBL& absres, const DBL& factor);

    /// Print out final status of an iterative method
    void PrintFinal(const PRTLVL& prtlvl, const INT& iter, const INT& maxit,
                    const DBL& relres);

public:
    /// constructor by default
    PCG() : lop(nullptr), rk(0), pk(0),
            zk(0), ax(0){};

    /// check and allocate memory
    FaspRetCode Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param);

    /// build preconditioner operator
    void SetupPCD(const LOP* lop);

    /// solve by PCG
    FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x,IterParam& param);

    /// clean preconditioner operator
    void CleanPCD();

    /// Release temporary memory
    void Clean();

    /// destructor
    ~PCG() {};
};

//! Warning for actual relative residual
static __inline__ void RealRes(DBL relres){
    std::cout<<"### WARNING: The actual relative residual = "
             <<relres<<std::endl;
}

//! Warning for computed relative residual
static __inline__ void Compres(DBL relres){
    std::cout<<"### WARNING: The computed relative residual = "
             <<relres<<std::endl;
}

//! Output relative difference and residual
static __inline__ void DiffRes(DBL reldiff,DBL relres){
    std::cout<<"||u-u'|| = "<<reldiff<<
             " and the comp. rel. res. = "<<relres<<std::endl;
}

#endif /* end if for __PCG_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
