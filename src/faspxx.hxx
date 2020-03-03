/** \file    faspxx.hxx
 *  \brief   Main FASP++ header file
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Sep/01/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __FASPXX_HEADER__  /*-- allow multiple inclusions --*/
#define __FASPXX_HEADER__  /**< indicate faspxx.hxx has been included before */

// Definition of data length
typedef unsigned int        INT;     ///< Index type: Must be non-negative!
typedef double              DBL;     ///< Double precision numbers

// Definition of big and small numbers
const DBL  SMALL_TOL     =  1e-14;   ///< Small positive real for tolerance
const DBL  LARGE         =  1e+60;   ///< Largest double number
const DBL  SMALL         = -1e+60;   ///< Smallest double number
const DBL  CLOSE_ZERO    =  1e-20;   ///< Tolerance for closeness to zero

// Definition of time units
const DBL  CLOCK_USE_SEC =  5000;    ///< Show clock time in seconds
const DBL  CLOCK_USE_MIN =  200000;  ///< Show clock time in minutes

// Definition of constants in Krylov methods
const DBL  KSM_CHK_RATIO =  0.95;    ///< Check ratio for Krylov space methods
const int  MAX_STAG_NUM  =  20;      ///< Maximal number of stagnation checks
const int  PRT_STEP_NUM  =  20;      ///< Print iteration info every N steps

#endif /* end if for __FASPXX_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/