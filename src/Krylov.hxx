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

#ifndef __KRYLOV_HEADER__      /*-- allow multiple inclusions --*/
#define __KRYLOV_HEADER__      /**< indicate Krylov.hxx has been included before */

// FASPXX header files
#include "RetCode.hxx"
#include "SOL.hxx"
#include "Iter.hxx"
#include "CG.hxx"
#include "BiCGStab.hxx"

/// General interface to Krylov subspace methods.
FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, SOL& pc, SOLParams& params);

#endif // __KRYLOV_HEADER__

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/