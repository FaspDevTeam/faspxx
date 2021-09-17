/** \file    Faspxx.hxx
 *  \brief   Main FASP++ header file
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Sep/01/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __FASPXX_HEADER__ /*-- allow multiple inclusions --*/
#define __FASPXX_HEADER__ /**< indicate faspxx.hxx has been included before */

// Definition of data-type length
typedef unsigned int INDEX; ///< Index type: Must be non-negative!
typedef unsigned int INT;   ///< Regular integer numbers
typedef double       DBL;   ///< Double precision numbers

// Definition of range, time units, tolerance and solver constants
const DBL SMALL_TOL     = 1e-14;  ///< Small positive real for tolerance
const DBL LARGE         = 1e+60;  ///< Largest double number
const DBL SMALL         = -1e+60; ///< Smallest double number
const DBL CLOSE_ZERO    = 1e-20;  ///< Tolerance for almost zero
const DBL CLOCK_USE_SEC = 5000;   ///< Show clock time in seconds
const DBL CLOCK_USE_MIN = 200000; ///< Show clock time in minutes
const DBL KSM_CHK_RATIO = 0.95;   ///< Check ratio for Krylov space methods
const INT MAX_STAG_NUM  = 20;     ///< Maximal number of stagnation checks
const INT PRT_STEP_NUM  = 20;     ///< Print iteration info every N steps
const INT MAX_MG_LEVEL  = 20;     ///< Maximal number of multigrid levels

/// Solver types.
enum SOLType {
    CG       = 1,  ///< Conjugate Gradient
    BICGSTAB = 2,  ///< Bi-Conjugate Gradient Stabilized
    MINRES   = 3,  ///< Minimal Residual
    GMRES    = 4,  ///< Generalized Minimal Residual
    FGMRES   = 5,  ///< Flexible GMRES
    VFGMRES  = 6,  ///< Variable-restarting FGMRES
    Jacobi   = 11, ///< Jacobi method
    GS       = 12, ///< Gauss-Seidel method
    SGS      = 13, ///< Symmetrized Gauss-Seidel method
    SOR      = 14, ///< Successive over-relaxation method
    SSOR     = 15, ///< Symmetrized successive over-relaxation method
    MG       = 21, ///< Multigrid method
    FMG      = 22, ///< Full multigrid method
    UMFPACK  = 91, ///< Direct method from UMFPACK
    MUMPS    = 92, ///< Direct method from MUMPS
    SUPERLU  = 93, ///< Direct method from SUPERLU
    PARDISO  = 94, ///< Direct method from PARDISO
};

#endif /* end if for __FASPXX_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/01/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Add solver types                     */
/*----------------------------------------------------------------------------*/