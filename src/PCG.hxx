/**
 * a header file for PCG
 */

#ifndef SRC_PCG_HXX
#define SRC_PCG_HXX

#include <cfloat>
#include "MAT.hxx"
#include "LOP.hxx"
#include <cmath>

/*---------------------------------------------------------------------------------*/

//! Warning for solution close to zero
#define ZEROSOL std::cout<<"### WARNING: Iteration stopped -- solution almost "<< \
"zero! [ "<<__FILE__<<" , "<<__FUNCTION__<<" , "<<__LINE__<<" ]"<<std::endl;

//! Warning for iteration restarted
#define RESTART std::cout<<"### WARNING: Iteration restarted -- stagnation! [ "<< \
__FILE__<<" , "<<__FUNCTION__<<" , "<<__LINE__<<" ]"<<std::endl;

//! Warning for stagged iteration
#define STAGGED std::cout<<"### WARNING: Iteration stopped -- staggnation! [ "<< \
__FILE__<<" , "<<__FUNCTION__<<" , "<<__LINE__<<" ]"<<std::endl;

//! Warning for tolerance practically close to zero
#define ZEROTOL std::cout<<"### WARNING: The tolerence might be too small! [ "<< \
__FILE__<<" , "<<__FUNCTION__<<" , "<<__LINE__<<" ]"<<std::endl;

//! Warning for divided by zero
#define DIVZERO std::cout<<"### WARNING: Divided by zero! [ "<< \
__FILE__<<" , "<<__FUNCTION__<<" , "<<__LINE__<<" ]"<<std::endl;

//! Warning for actual relative residual
#define REALRES(relres) std::cout<<"### WARNING: The actual relative residual = " \
<<relres<<std::endl;

//! Warning for computed relative residual
#define COMPRES(relres) std::cout<<"### WARNING: The computed relative residual = " \
<<relres<<std::endl;

//! Output relative difference and residual
#define DIFFRES(reldiff, relres) std::cout<<"||u-u'|| = "<<reldiff<< \
" and the comp. rel. res. = "<<relres<<std::endl;

enum StopType {
    STOP_REL_RES,
    STOP_REL_PRECRES,
    STOP_MOD_REL_RES
};

class PCG {
private:
    DBL rtol;
    INT maxIt;
    INT pc;
    MAT A;
    MAT P;
    VEC b;

    void ApplyPreconditioner() {};

    void Final(const INT iter, const INT maxit, const DBL relres);

public:
    PCG() : rtol(0), maxIt(0), pc(0) {};

    PCG(DBL &rtol_, DBL &maxIt_, INT &pc_) : rtol(rtol_), maxIt(maxIt),
                                             pc(pc) {};

    FaspRetCode SetUp(const MAT &A_, const VEC &b_, const DBL &rtol_, const INT
    &maxIt_);

    FaspRetCode SetUpPCD(const MAT &P);

    FaspRetCode Start(VEC &x, INT &iter,const StopType &type);

    void CleanPCD();

    void Clean();

};

#endif //SRC_PCG_HXX
