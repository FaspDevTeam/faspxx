/*! \file    GMRES.hxx
 *  \brief   Preconditioned GMRES class declaration
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    July/16/2020
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __GMRES_HXX__ /*-- allow multiple inclusions --*/
#define __GMRES_HXX__ /**< indicate GMRES.hxx has been included before */

// Standard header files
#include <cmath>

// FASPXX header files
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "SOL.hxx"

/*! \class GMRES
 *  \brief Preconditioned generalized minimal residual method.
 */
class GMRES : public SOL
{
private:
    VEC wk;
    VEC tmp;
    VEC safe;

    std::vector<std::vector<double>> hh;
    std::vector<double>              hsin;
    std::vector<double>              hcos;
    std::vector<double>              var;
    std::vector<VEC>                 V;

    bool useRightPrecond;
    int  maxRestart;
    int  minRestart;
    int  restart;
    int  len;

    // Constants for variable restarting
    const double max_cr   = 0.99026806874157040;
    const double min_cr   = 0.17364817766693041;
    const int    decrease = 3;

public:
    /// Default constructor.
    GMRES()
        : wk(0)
        , tmp(0)
        , safe(0)
        , hh(0)
        , hsin(0)
        , hcos(0)
        , var(0)
        , useRightPrecond(true)
        , maxRestart(30)
        , minRestart(10){};

    /// Default destructor.
    ~GMRES() = default;

    /// Set GMRES to do right preconditioning.
    void SetRightPrecond();

    /// Set GMRES to do left preconditioning.
    void SetLeftPrecond();

    /// Set the maximum and minmum restart numbers for GMRES.
    void SetMaxMinRestart(const int maxRestart, const int minRestart);

    /// Setup the GMRES method.
    FaspRetCode Setup(const LOP& A) override;

    /// Solve Ax=b using the GMRES method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;

    /// Clean up GMRES data allocated during setup.
    void Clean() override;

private:
    /// Right-preconditioned GMRES solver.
    FaspRetCode RSolve(const VEC& b, VEC& x);

    /// Left-preconditioned GMRES slover.
    FaspRetCode LSolve(const VEC& b, VEC& x);
};

#endif /* end if for __GMRES_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        July/16/2020     Create file                          */
/*  Chensong Zhang      Sep/17/2021      Call right/left precond in Solve     */
/*----------------------------------------------------------------------------*/