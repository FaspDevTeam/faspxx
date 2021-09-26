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

#ifndef __SOL_HEADER__ /*-- allow multiple inclusions --*/
#define __SOL_HEADER__ /**< indicate SOL.hxx has been included before */

// FASPXX header files
#include "ErrorLog.hxx"
#include "Faspxx.hxx"
#include "LOP.hxx"
#include "Param.hxx"
#include "RetCode.hxx"
#include "VEC.hxx"

/// Base class for iterative solvers.
class SOL
{

protected:
    const LOP* A;       ///< Coefficient matrix in Ax=b
    SOL*       pcd;     ///< Preconditioner for this solver
    double     norm2;   ///< Euclidean norm
    double     normInf; ///< Infinity norm
    int        numIter; ///< Number of iterations when exit
    SOLParams  params;  ///< solver parameters

    /// Warning for actual relative residual
    void WarnRealRes(double relres) const;

    /// Warning for computed relative residual
    void WarnCompRes(double relres) const;

    /// Output relative difference and residual
    void WarnDiffRes(double reldiff, double relres) const;

public:
    /// Default constructor.
    SOL()
        : A(nullptr)
        , pcd(nullptr)
        , norm2(LARGE)
        , normInf(LARGE)
        , numIter(0){};

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
    const char* GetSolType(SOLType type) const;

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

    /// Print out duration of solver.
    void PrintTime(const double duration, std::ostream& out = std::cout) const;

    /// Setup preconditioner operator.
    virtual void SetupPCD(SOL& pc);

    /// Setup the iterative method using linear opeartor.
    virtual FaspRetCode Setup(const LOP& A) { FASPXX_ABORT("Should be over-written!"); }

    /// Solve Ax=b using the iterative method.
    virtual FaspRetCode Solve(const VEC& b, VEC& x)
    {
        FASPXX_ABORT("Should be over-written!");
    }

    /// Release temporary memory and clean up.
    virtual void Clean() { FASPXX_ABORT("Should be over-written!"); }
};

#endif /* end if for __SOL_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Nov/25/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/