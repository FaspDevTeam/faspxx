/*! \file    SOL.hxx
 *  \brief   Solver class declaration
 *  \author  Kailei Zhang, Chensong Zhang, Ronghong Fan
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
        CG        = 1,       ///< Conjugate Gradient
        BICGSTAB  = 2,       ///< Bi-Conjugate Gradient Stabilized
        MINRES    = 3,       ///< Minimal Residual
        GMRES     = 4,       ///< Generalized Minimal Residual
        FGMRES    = 5,       ///< Flexible GMRES
        VFGMRES   = 6,       ///< Variable-restarting FGMRES
    };

protected:
    SOLType      type;       ///< Algorithm type
    const LOP *  A;          ///< Coefficient matrix in Ax=b
    INT          maxIter;    ///< Maximal number of iterations
    INT          minIter;    ///< Minimal number of iterations
    DBL          relTol;     ///< Tolerance for relative residual
    DBL          absTol;     ///< Tolerance for absolute residual
    INT          restart;    ///< Restart number
    DBL          norm2;      ///< Euclidean norm
    DBL          normInf;    ///< Infinity norm
    INT          numIter;    ///< Number of iterations when exit
    SOL *        pc;         ///< Preconditioner for this solver
    Output       verbose;    ///< Output verbosity level
    bool         mark;       ///< Mark where the PC is allocating memory

    /// Warning for actual relative residual
    void WarnRealRes(DBL relres) const;

    /// Warning for computed relative residual
    void WarnCompRes(DBL relres) const;

    /// Output relative difference and residual
    void WarnDiffRes(DBL reldiff, DBL relres) const;

    /// Print out iteration information for iterative solvers
    void PrintInfo(const INT& iter, const DBL& resRel, const DBL& resAbs,
                   const DBL& factor, std::ostream& out = std::cout) const;

public:

    /// Default constructor
    SOL() : type(SOLType::CG), A(nullptr), maxIter(100), minIter(0),
            relTol(1e-6), absTol(1e-8), restart(25), norm2(1.0), normInf(1.0),
            numIter(0), pc(nullptr), verbose(PRINT_NONE), mark(false) {};

    /// Constructor // Todo: 有用吗？如果有用，为什么只有这几个参数？-zcs
    SOL(Output verbose, INT maxIter, DBL relTol, DBL absTol, INT restart);

    /// Default destructor
    ~SOL();

    /// Set 'verbose' 's value
    void SetOutput(Output verbose);

    /// Set 'maxIter' 's value
    void SetMaxIter(INT maxIter);

    /// Set 'minIter' 's value
    void SetMinIter(INT maxIter);

    /// Set 'relTol' 's value
    void SetRelTol(DBL relTol);

    /// Set 'absTol' 's value
    void SetAbsTol(DBL absTol);

    /// Set 'restart' 's value
    void SetRestart(INT restart);

    /// Set 'solver' type
    void SetSolType(SOLType solver);

    /// Get solver type
    const char * GetSolType(SOLType type) const;

    /// Get residual 's Euclidean norm
    DBL GetNorm2() const;

    /// Get residual 's infinity norm
    DBL GetInfNorm() const;

    /// Get iterations
    INT GetIterations() const;

    /// Print parameters
    void PrintParam(std::ostream& out = std::cout) const;

    /// Print out final status of an iterative method
    void PrintFinal(std::ostream& out = std::cout) const;

    /// Set parameters from a disk file
    void SetSolFromFile(const char *file = nullptr, const char *prefix = nullptr);

    /// Check and allocate memory
    virtual FaspRetCode Setup(const LOP& _A) {
        FASPXX_ABORT("Not supported!");
    }

    /// Setup preconditioner operator
    virtual void SetPC(SOL *pc);

    /// Solve Ax=b using the iterative method
    virtual FaspRetCode Solve(const VEC& b, VEC& x) {
        FASPXX_ABORT("Not supported!");
    }

    /// Release temporary memory and clean up
    virtual void Clean();
};

#endif /* end if for __SOL_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/