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

enum PRINT{
    PRINT_NONE,
    PRINT_MIN,
    PRINT_SOME,
    PRINT_MORE,
    PRINT_MOST,
    PRINT_ALL
};

enum StopType {
    STOP_REL_RES,
    STOP_REL_PRECRES,
    STOP_MOD_REL_RES
};

struct PCGInputParam{
    INT maxIter;
    DBL relTol;
    DBL absTol;
    INT restart;
    PRINT printLevel;
    PCGInputParam():maxIter(0),relTol(0),absTol(0),restart(0),printLevel(0){};
    PCGInputParam(PCGInputParam& inParam){
        this->maxIter=inParam.maxIter;
        this->relTol=inParam.relTol;
        this->absTol=inParam.absTol;
        this->restart=inParam.restart;
        this->printLevel=inParam.printLevel;
    }
    PCGInputParam &operator=(PCGInputParam& inParam){
        this->maxIter=inParam.maxIter;
        this->relTol=inParam.relTol;
        this->absTol=inParam.absTol;
        this->restart=inParam.restart;
        this->printLevel=inParam.printLevel;
    }
    ~PCGInputParam(){};
};
struct PCGOutputParam{
    INT iter;
    DBL norm2;
    DBL normInf;
    PCGOutputParam():iter(0),norm2(0),normInf(0){};
    PCGOutputParam(PCGOutputParam &outParam){
        this->iter=outParam.iter;
        this->norm2=outParam.norm2;
        this->normInf=outParam.normInf;
    }
    PCGOutputParam &operator=(PCGOutputParam &outParam){
        this->iter=outParam.iter;
        this->norm2=outParam.norm2;
        this->normInf=outParam.normInf;
    }
    ~PCGOutputParam(){};

};

class PCG {
private:
    PCGInputParam inParam;
    PCGOutputParam outParam;
    INT pcflag;
    LOP lop;
    PRINT printLevel;

    void ApplyPreconditioner() {};

    void PrintInfo(const PRINT &ptrlvl,const StopType &type,const INT &iter,
            const DBL &relres,const DBL &absres,const DBL &factor);

    void Final(const INT &iter, const INT &maxit, const DBL &relres);

public:
    PCG() : inParam(),outParam(),pcflag(0),lop(0,0),printLevel(0){};

    PCG(PCGInputParam &inParam,LOP &lop) :inParam(inParam),outParam(),
    pcflag(0),lop(0,0),printLevel(0){};

    FaspRetCode SetUp(PCGInputParam &inParam);

    FaspRetCode SetUpPCD(const LOP &lop);

    FaspRetCode Start(MAT &A,VEC &b,VEC &x,const StopType &type,
            PCGOutputParam &outParam);

    void CleanPCD();

    ~PCG(){};
};

#endif //SRC_PCG_HXX
