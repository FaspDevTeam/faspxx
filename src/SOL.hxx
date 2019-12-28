/*! \file    SOL.hxx
 *  \brief   Iterative solver class declaration
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

using namespace std;

/// Iterative solver type.
enum SOLType {
    CG        = 1,   ///< Conjugate Gradient
    BICGSTAB  = 2,   ///< Bi-Conjugate Gradient Stabilized
    MINRES    = 3,   ///< Minimal Residual
    GMRES     = 4,   ///< Generalized Minimal Residual
    FGMRES    = 5,   ///< Flexible GMRES
    VFGMRES   = 6,   ///< Variable-restarting FGMRES
    Jacobi    = 11,  ///< Jacobi iteration
};

/// Iterative solver parameters.
struct SOLParams {
    SOLType   type;       ///< Algorithm type
    string    algName;    ///< Algorithm name
    int       maxIter;    ///< Maximal number of iterations
    int       minIter;    ///< Minimal number of iterations
    int       safeIter;   ///< Minimal number of iterations before safe-guard
    int       restart;    ///< Restart number
    double    relTol;     ///< Tolerance for relative residual
    double    absTol;     ///< Tolerance for absolute residual
    double    weight;     ///< Weight for correction schemes
    Output    verbose;    ///< Output verbosity level

    SOLParams() : type(SOLType::CG), maxIter(100), minIter(0), safeIter(500),
                  restart(20), relTol(1e-6), absTol(1e-8), weight(1.0),
                  verbose(PRINT_NONE) {}
};

/// Base class for iterative solvers.
class SOL {

protected:
    const LOP *  A;          ///< Coefficient matrix in Ax=b
    SOL *        pc;         ///< Preconditioner for this solver
    bool         withPC;     ///< Given a PC or not
    double       norm2;      ///< Euclidean norm
    double       normInf;    ///< Infinity norm
    int          numIter;    ///< Number of iterations when exit
    SOLParams    params;     ///< solver parameters

    /// Warning for actual relative residual
    void WarnRealRes(double relres) const;

    /// Warning for computed relative residual
    void WarnCompRes(double relres) const;

    /// Output relative difference and residual
    void WarnDiffRes(double reldiff, double relres) const;

public:

    /// Default constructor.
    SOL() : A(nullptr), pc(nullptr), withPC(false),
            norm2(1.0), normInf(1.0), numIter(0) {};

    /// Default destructor.
    ~SOL();

    /// Set output level.
    void SetOutput(Output verbose);

    /// Set max number of iterations.
    void SetMaxIter(int maxIter);

    /// Set min number of iterations.
    void SetMinIter(int minIter);

    /// Set number of safe-guard iterations.
    void SetSafeIter(int safeIter);

    /// Set restart number for Krylov methods.
    void SetRestart(int restart);

    /// Set tolerance for relative residual.
    void SetRelTol(double relTol);

    /// Set tolerance for absolute residual.
    void SetAbsTol(double absTol);

    /// Set weight for correction schemes.
    void SetWeight(double alpha);

    /// Set solver type.
    void SetSolType(SOLType solver);

    /// Set solver type from its name.
    static void SetSolTypeFromName(SOLParams& params);

    /// Get solver type.
    const char * GetSolType(SOLType type) const;

    /// Get Euclidean norm of residual.
    double GetNorm2() const;

    /// Get infinity norm of residual.
    double GetInfNorm() const;

    /// Get number of iterations.
    int GetIterations() const;

    /// Print parameters.
    void PrintParam(std::ostream& out = std::cout) const;

    /// Print out iteration information table header.
    void PrintHead(std::ostream& out = std::cout) const;

    /// Print out iteration information for iterative solvers.
    void PrintInfo(const int& iter, const double& resRel, const double& resAbs,
                   const double& factor, std::ostream& out = std::cout) const;

    /// Print out final status of an iterative method.
    void PrintFinal(const int& iter, const double& resRel, const double& resAbs,
                    const double& ratio, std::ostream& out = std::cout) const;

    /// Setup preconditioner operator.
    virtual void SetPC(SOL *pc);

    /// Setup the iterative method.
    virtual FaspRetCode Setup(const LOP& A) {
        FASPXX_ABORT("Not supported!");
    }

    /// Release temporary memory and clean up.
    virtual void Clean() {
        FASPXX_ABORT("Not supported!");
    }

    /// Solve Ax=b using the iterative method.
    virtual FaspRetCode Solve(const VEC& b, VEC& x) {
        FASPXX_ABORT("Not supported!");
    }

};

#endif /* end if for __SOL_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/