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

/// identity preconditioner
FaspRetCode Identity::Solve(const VEC& x, VEC& y) {
    y = x;
    return FaspRetCode::SUCCESS;
}

/// constructor
Jacobi::Jacobi(std::vector<DBL> diag) {
    this->diagReciprocal.SetValues(diag);
    this->diagReciprocal.Reciprocal();
}

/// jacobi preconditioner
FaspRetCode Jacobi::Solve(const VEC& x, VEC& y) {
    y = x;
    y.PointwiseMult(this->diagReciprocal);
    return FaspRetCode ::SUCCESS;
}