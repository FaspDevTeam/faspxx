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

class PCG {
private:
    const LOP* lop; //fff这些私有变量是否定义成指针或者引用较好
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
    PCG() : rk(0), pk(0),
            zk(0), ax(0) { lop = new IdentityLOP(rk.GetSize()); }

    /// check and allocate memory
    FaspRetCode Setup(const MAT& A,const VEC& b,VEC& x,const IterParam& param);

    /// build preconditioner operator
    void SetupPCD(const LOP* lop);

    /// solve by PCG
    FaspRetCode Solve(const MAT& A, const VEC& b, VEC& x,IterParam& param);

    /// clean preconfitioner operator
    void CleanPCD();

    /// Release temporary memory
    void Clean();

    /// destructor
    ~PCG() { delete lop; }; //fff这种方式不太好:这里只真的默认的 new IdentityLOP()
};

//! Warning for solution close to zero
static __inline__ void ZeroSol(){
    std::cout<<"### WARNING: Iteration stopped -- solution almost "<<
             "zero! [ "<<__FILE__<<" , "<<__LINE__<<" ]"<<std::endl;
}

//! Warning for iteration restarted
static __inline__ void Restart(){
    std::cout<<"### WARNING: Iteration restarted -- stagnation! [ "<<
             __FILE__<<" , "<<__LINE__<<" ]"<<std::endl;
}

//! Warning for stagged iteration
static __inline__ void Stagged(){
    std::cout<<"### WARNING: Iteration stopped -- staggnation! [ "<<
             __FILE__<<" , "<<__LINE__<<" ]"<<std::endl;
}

//! Warning for tolerance practically close to zero
static __inline__ void ZeroTol(){
    std::cout<<"### WARNING: The tolerence might be too small! [ "<<
             __FILE__<<" , "<<__LINE__<<" ]"<<std::endl;
}

//! Warning for divided by zero
static __inline__ void DivZero(){
    std::cout<<"### WARNING: Divided by zero! [ "<<
             __FILE__<<" , "<<__LINE__<<" ]"<<std::endl;
}

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