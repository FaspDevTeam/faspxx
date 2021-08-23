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

#ifndef __GMRES_HXX__      /*-- allow multiple inclusions --*/
#define __GMRES_HXX__      /**< indicate GMRES.hxx has been included before */

// FASPXX header files
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"
// Standard header files
#include <cmath>

/*! \class GMRES
 *  \brief Preconditioned generalized minimal residual method.
 */
class GMRES : public SOL {

private:
    VEC wk;
    VEC tmp;
    VEC safe;

    std::vector<std::vector<double>> hh;
    std::vector<double> hsin;
    std::vector<double> hcos;
    std::vector<double> var;

    std::vector<VEC> V;

    int maxRestart;
    int minRestart;
    int restart;
    int len;

    // Constants for variable restarting
    const double max_cr = 0.99026806874157040;
    const double min_cr = 0.17364817766693041;
    const int  decrease = 3;

public:
    /// Default constructor.
    GMRES() : wk(0), safe(0), tmp(0), hh(0), hsin(0), hcos(0), var(0),
              maxRestart{30}, minRestart(10) {};

    /// Default destructor.
    ~GMRES() = default;

    /// Set the maximum and minmum restart numbers for variable GMRES.
    void SetMaxMinRestart(const int maxRestart, const int minRestart);

    /// Setup the GMRES method.
    FaspRetCode Setup(const LOP &A) override;

    /// Clean up GMRES data allocated during setup.
    void Clean() override;

private:
    /// Solve Ax=b using the GMRES method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

public:
    /// Right-preconditioned GMRES solver.
    FaspRetCode RSolve(const VEC &b, VEC &x);

    /// Left-preconditioned GMRES slover.
    FaspRetCode LSolve(const VEC &b, VEC &x);

};

#endif /* end if for __GMRES_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/