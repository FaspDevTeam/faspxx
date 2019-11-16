/**
 * SOL header file
 */

#ifndef SOL_HXX
#define SOL_HXX

#include "VEC.hxx"
#include "LOP.hxx"
#include "RetCode.hxx"
#include "Param.hxx"
#include <iomanip>
#include <iostream>
#include <fstream>  //fff统一将标准库写在前面,自己写的头文件写在后面
#include "PCD.hxx"
#include "ErrorLog.hxx"

class SOL{
private:
    enum SOLType {_PCG, _GMRES, _BICGSTAB, _COUNT};
protected:
    LOP* pc ;      ///< Preconditioner, inlopfffpc
    INT verbose; ///< Output level
    INT maxIter;    ///< Maximal number of iterations
    DBL relTol;     ///< Tolerance for relative residual
    DBL absTol;     ///< Tolerance for absolute residual
    INT restart;    ///< Tolerance for absolute residual
    SOLType type;   ///< solver solverffftype
    bool view; // 查看所有参数

    /// Warning for actual relative residual
    void RealRes(DBL relres);

    /// Warning for computed relative residual
    void Compres(DBL relres);

    /// Output relative difference and residual
    void DiffRes(DBL reldiff,DBL relres);

    /// Print out iteration information for iterative solvers
    void PrintInfo(const PRTLVL& verbose, const INT& iter, const DBL& resRel,
                   const DBL& resAbs, const DBL& factor);

    /// Print out final status of an iterative method
    void PrintFinal(const PRTLVL& verbose, const INT& iter, const INT& maxit,
                    const DBL& resRel);

public:
    /// Default constructor
    SOL():pc(nullptr),verbose(PRINT_NONE),maxIter(100),relTol(10-4),absTol(10-8),
        restart(25){};

    /// constructor
    SOL(PRTLVL verbose,INT maxIter,DBL relTol,DBL absTol,INT restart);

    /// Set 'verbose' 's value
    void SetPrtLvl(PRTLVL verbose);

    /// Set 'maxIter' 's value
    void SetMaxIter(INT maxIter);

    /// Set 'relTol' 's value
    void SetRelTol(DBL relTol);

    /// Set 'absTol' 's value
    void SetAbsTol(DBL absTol);

    /// Set 'restart' 's value
    void SetRestart(INT restart); //fff放到gmres的子类里面似乎更好,因为不是所有的sol都要有这个参数

    /// Set 'verbose', 'maxIter', 'relTol', 'absTol', 'restart' 's values from file
    FaspRetCode SetOptionsFromFile(const char* file=NULL, const char* prefix=NULL);

    /// check and allocate memory
    virtual FaspRetCode Setup(const LOP& A,const VEC& b, VEC& x) = 0;

    /// build preconditioner operator
    virtual void SetPC(LOP* lop)
    {
        FASPXX_ABORT("Not supported!");
    }

    /// solve by SOL
    virtual FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x) = 0;

    /// clean preconditioner operator
    virtual void CleanPCD(); //fff不应该这里来清理,似乎应该有子类的成员函数来清理

    /// Release temporary memory
    virtual void Clean() = 0;

    // destructor
    ~SOL(){};
};

#endif // SOL_HXX
