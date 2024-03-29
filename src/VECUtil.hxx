/** \file    VECUtil.hxx
 *  \brief   Tools declaration for checking and manipulating VEC
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Sep/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __VECUTIL_HXX__ /*-- allow multiple inclusions --*/
#define __VECUTIL_HXX__ /**< indicate VECUtil.hxx has been included before */

// FASPXX header files
#include "Faspxx.hxx"
#include "VEC.hxx"

/// Check whether the size of VEC object is zero.
FaspRetCode CheckVECSize(const VEC& v);

/// Check whether two VEC sizes match.
FaspRetCode CheckVECSize(const VEC& v1, const VEC& v2);

/// Check whether vector crossover.
FaspRetCode CheckVECSize(const VEC& v, const USI& position);

/// Check whether there is a zero entry in VEC object.
FaspRetCode CheckVECZero(const VEC& v, const DBL tol = SMALL_TOL);

#endif /* end if for __VECUTIL_HXX__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Sep/24/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/