/*! \file    SOL.hxx
 *  \brief   Solver class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __SOL_HEADER__      /*-- allow multiple inclusions --*/
#define __SOL_HEADER__      /**< indicate SOL.hxx has been included before */

#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "faspxx.hxx"
#include "RetCode.hxx"
#include "ErrorLog.hxx"
#include "Param.hxx"
#include "LOP.hxx"
#include "VEC.hxx"

/*! \class SOL
 *  \brief Iterative solvers.
 */
class SOL {

private:
    enum SOLType {
        _PCG, _GMRES, _BICGSTAB, _COUNT
    };

protected:
    SOL *pc;        ///< Preconditioner, inlopfffpc
    const LOP *A;   ///< Matrix object
    Output verbose; ///< Output level
    INT maxIter;    ///< Maximal number of iterations
    DBL relTol;     ///< Tolerance for relative residual
    DBL absTol;     ///< Tolerance for absolute residual
    INT restart;    ///< Tolerance for absolute residual
    SOLType type;   ///< Solver
    DBL norm2;      ///< l2-norm
    DBL norminf;    ///< Infty-norm
    INT numIter;    ///< Iterations
    bool view;      ///< View all parameters
    bool mark;        ///< Mark where the PC is requesting memory

    /// Warning for actual relative residual
    void RealRes(DBL relres);

    /// Warning for computed relative residual
    void CompRes(DBL relres);

    /// Output relative difference and residual
    void DiffRes(DBL reldiff, DBL relres);

    /// Print out iteration information for iterative solvers
    void PrintInfo(const Output &verbose, const INT &iter, const DBL &resRel,
                   const DBL &resAbs, const DBL &factor);

    /// Print out final status of an iterative method
    void PrintFinal(const Output &verbose, const INT &iter, const INT &maxit,
                    const DBL &resRel);

    /// Select solver
    const char *SelectSolver(SOLType type);

public:

    /// Default constructor
    SOL() : pc(nullptr), A(nullptr), verbose(PRINT_NONE), maxIter(100),
            relTol(1e-4), absTol(1e-8), restart(25), norm2(1.0), norminf(1.0),
            numIter(0), mark(false) {};

    /// constructor
    SOL(Output verbose, INT maxIter, DBL relTol, DBL absTol, INT restart);

    /// Set 'verbose' 's value
    void SetPrtLvl(Output verbose);

    /// Set 'maxIter' 's value
    void SetMaxIter(INT maxIter);

    /// Set 'relTol' 's value
    void SetRelTol(DBL relTol);

    /// Set 'absTol' 's value
    void SetAbsTol(DBL absTol);

    /// Set 'restart' 's value
    void SetRestart(INT restart);

    /// Set 'solver' type
    void SetSolver(SOLType solver);

    /// Get residual 's 'l2-norm'
    DBL GetNorm2();

    /// Get residual 's 'infty-norm'
    DBL GetInfNorm();

    /// Get iterations
    INT GetIterations();

    /// Print parameters
    void Print(std::ostream &out = std::cout) const;

    /// Set 'verbose', 'maxIter', 'relTol', 'absTol', 'restart' 's values from file
    FaspRetCode SetOptionsFromFile(const char *file = nullptr, const char *
        prefix = nullptr);

    /// check and allocate memory
    virtual FaspRetCode Setup(const LOP &_A) {
        FASPXX_ABORT("Not supported!")
    }

    /// build preconditioner operator
    virtual void SetPC(SOL *pc);

    /// solve by SOL
    virtual FaspRetCode Solve(const VEC &b, VEC &x) {
        FASPXX_ABORT("Not supported!");
    }

    /// Release temporary memory
    virtual void Clean();

    /// destructor
    ~SOL();
};

#endif /* end if for __SOL_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/