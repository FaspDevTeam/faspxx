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
Jacobi::Jacobi(INT row, INT col, std::vector<DBL> diag) {
    this->row = row;
    this->col = col;
    this->diag.SetValues(diag);
}

/// jacobi preconditioner
FaspRetCode Jacobi::Solve(const VEC& x, VEC& y) {
    VEC tmp = this->diag;
    tmp.Reciprocal(); // Todo: 这个只要在构造函数做一次就可以了，不用每次Solve都算一遍 -zcs
    y = x;
    y.PointwiseMult(tmp);
    return FaspRetCode ::SUCCESS;
}