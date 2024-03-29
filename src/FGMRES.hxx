/*! \file    FGMRES.hxx
 *  \brief   Preconditioned Flexible GMRES class declaration
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    July/16/2020
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __FGMRES_HXX__ /*-- allow multiple inclusions --*/
#define __FGMRES_HXX__ /**< indicate FGMRES.hxx has been included before */

// Standard header files
#include <cmath>

// FASPXX header files
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "SOL.hxx"

/*! \class FGMRES
 *  \brief Preconditioned flexible generalized minimal residual method.
 */
class FGMRES : public SOL
{
private:
    VEC wk;
    VEC safe;
    VEC tmp;

    std::vector<std::vector<double>> hh;
    std::vector<double>              hsin;
    std::vector<double>              hcos;
    std::vector<double>              var;

    std::vector<VEC> V;
    std::vector<VEC> Z;

    USI maxRestart;
    USI minRestart;
    USI restart;
    USI len;

    // Constants for variable restarting
    const double max_cr   = 0.99026806874157040;
    const double min_cr   = 0.17364817766693041;
    const USI    decrease = 3;

public:
    /// Default constructor.
    FGMRES()
        : wk(0)
        , safe(0)
        , tmp(0)
        , hh(0)
        , hsin(0)
        , hcos(0)
        , var(0)
        , maxRestart(30)
        , minRestart(10)
        , restart(20)
        , len(0){};

    /// Default destructor.
    ~FGMRES() = default;

    /// Set the maximum and minmum restart numbers for variable FGMRES.
    void SetMaxMinRestart(const USI maxRestart, const USI minRestart);

    /// Setup the FGMRES method.
    FaspRetCode Setup(const LOP& A) override;

    /// Solve Ax=b using the FGMRES method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;

    /// Clean up FGMRES data allocated during setup.
    void Clean() override;
};

#endif /* end if for __FGMRES_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        July/16/2020     Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/