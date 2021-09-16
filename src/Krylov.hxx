/*! \file    Krylov.hxx
 *  \brief   Declaration of interface to general Krylov subspace methods
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Dec/27/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __KRYLOV_HEADER__ /*-- allow multiple inclusions --*/
#define __KRYLOV_HEADER__ /**< indicate Krylov.hxx has been included before */

// FASPXX header files
#include "BiCGStab.hxx"
#include "CG.hxx"
#include "Iter.hxx"
#include "RetCode.hxx"
#include "SOL.hxx"

/// General interface to Krylov subspace methods.
FaspRetCode Krylov(LOP &A, VEC &b, VEC &x, SOL &pcd, SOLParams &params);

#endif // __KRYLOV_HEADER__

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Dec/27/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/