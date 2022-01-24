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

#ifdef _MSC_VER /*-- Define inline for MSVC --*/
#define __inline__ __inline 
#endif

/*----------------------------------------------------------------------------*/
/* Definition of data-type length                                             */
/*----------------------------------------------------------------------------*/

typedef int          INT; ///< Regular integer numbers
typedef unsigned int USI; ///< Unsigned integer numbers
typedef double       DBL; ///< Double precision numbers

/*----------------------------------------------------------------------------*/
/* Definition of constants for range, time units, and tolerance               */
/*----------------------------------------------------------------------------*/

const DBL SMALL_TOL     = 1e-14;  ///< Small positive real for tolerance
const DBL LARGE_DBL     = 1e+60;  ///< Largest double number
const DBL SMALL_DBL     = -1e+60; ///< Smallest double number
const DBL CLOSE_ZERO    = 1e-20;  ///< Tolerance for almost zero
const DBL CLOCK_USE_SEC = 5000;   ///< Show clock time in seconds
const DBL CLOCK_USE_MIN = 200000; ///< Show clock time in minutes

/*----------------------------------------------------------------------------*/
/* Definition of constants for solvers and preconditioners                    */
/*----------------------------------------------------------------------------*/

const USI MAX_ITER_NUM  = 100000; ///< Maximal number of multigrid levels
const DBL KSM_CHK_RATIO = 0.95;   ///< Check ratio for Krylov space methods
const USI MAX_STAG_NUM  = 20;     ///< Maximal number of stagnation checks
const USI PRT_STEP_NUM  = 20;     ///< Print iteration info every N steps
const USI MAX_MG_LEVEL  = 20;     ///< Maximal number of multigrid levels

#endif /* end if for __FASPXX_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/01/2019      Create file                          */
/*  Chensong Zhang      Sep/26/2021      Restructure file                     */
/*  Chensong Zhang      Sep/29/2021      Add USI and INT types                */
/*----------------------------------------------------------------------------*/