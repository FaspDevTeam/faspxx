/** \file    Faspxx.hxx
 *  \brief   Public FASP++ header file for users
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

/*----------------------------------------------------------------------------*/
/* Definition of data-type length                                             */
/*----------------------------------------------------------------------------*/

typedef unsigned int INDEX; ///< Index type: Must be non-negative!
typedef unsigned int INT;   ///< Regular integer numbers
typedef double       DBL;   ///< Double precision numbers

/*----------------------------------------------------------------------------*/
/* Definition of constants for range, time units, and tolerance               */
/*----------------------------------------------------------------------------*/

const DBL SMALL_TOL     = 1e-14;  ///< Small positive real for tolerance
const DBL LARGE         = 1e+60;  ///< Largest double number
const DBL SMALL         = -1e+60; ///< Smallest double number
const DBL CLOSE_ZERO    = 1e-20;  ///< Tolerance for almost zero
const DBL CLOCK_USE_SEC = 5000;   ///< Show clock time in seconds
const DBL CLOCK_USE_MIN = 200000; ///< Show clock time in minutes

/*----------------------------------------------------------------------------*/
/* Definition of constants for solvers and preconditioners                    */
/*----------------------------------------------------------------------------*/

const INT MAX_ITER_NUM  = 100000; ///< Maximal number of multigrid levels
const DBL KSM_CHK_RATIO = 0.95;   ///< Check ratio for Krylov space methods
const INT MAX_STAG_NUM  = 20;     ///< Maximal number of stagnation checks
const INT PRT_STEP_NUM  = 20;     ///< Print iteration info every N steps
const INT MAX_MG_LEVEL  = 20;     ///< Maximal number of multigrid levels

#endif /* end if for __FASPXX_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/01/2019      Create file                          */
/*  Chensong Zhang      Sep/26/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/