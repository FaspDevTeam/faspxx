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
#include <fstream>
#include "PCD.hxx"

class SOL{
protected:
    LOP *inlop ;      ///< Preconditioner
    PRTLVL verbose; ///< Output level
    INT maxIter;    ///< Maximal number of iterations
    DBL relTol;     ///< Tolerance for relative residual
    DBL absTol;     ///< Tolerance for absolute residual
    INT restart;    ///< Tolerance for absolute residual

    char *solver;   ///< solver

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
    SOL():inlop(nullptr),verbose(PRINT_NONE),maxIter(100),relTol(10-4),absTol(10-8),
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
    void SetRestart(INT restart);

    /// Set 'verbose', 'maxIter', 'relTol', 'absTol', 'restart' 's values from file
    FaspRetCode SetFromFile(const char* file);

    /// check and allocate memory
    virtual FaspRetCode Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param) = 0;

    /// build preconditioner operator
    void SetupPCD(LOP* lop);

    /// solve by SOL
    virtual FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x,IterParam& param) = 0;

    /// clean preconditioner operator
    void CleanPCD();

    /// Release temporary memory
    virtual void Clean() = 0;

    // destructor
    ~SOL(){};
};

#endif // SOL_HXX
