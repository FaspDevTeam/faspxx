/*! \file    Iter.cxx
 *  \brief   Simple iterative methods definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Dec/02/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "faspxx.hxx"
#include "Iter.hxx"

/// Does nothing in preconditioning
FaspRetCode Identity::Solve(const VEC& x, VEC& y) {
    y = x;
    return FaspRetCode::SUCCESS;
}

/// Compute the inverse of diagonal part
Jacobi::Jacobi(std::vector<DBL> diag) {
    this->diagInv.SetValues(diag);
    this->diagInv.Reciprocal();
}

/// Apply the Jacobi iteration
// Todo: This is only one step of Jacobi method. It should look like CG. -zcs
FaspRetCode Jacobi::Solve(const VEC& x, VEC& y) {
    y = x;
    y.PointwiseMult(this->diagInv);
    return FaspRetCode ::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/