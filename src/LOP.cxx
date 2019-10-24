/** \file LOP.cxx
 *  \brief Linear operator class definition
 *  \author Chensong Zhang
 *  \date Sep/27/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "faspxx.hxx"
#include "LOP.hxx"

/// Assign nrow, ncol to *this
LOP::LOP(const INT &nrow, const INT &ncol) {
    this->nrow = nrow;
    this->ncol = ncol;
}

/// Assign nrow, ncol=nrow to *this
LOP::LOP(const INT &nrow) {
    this->nrow = nrow;
    this->ncol = nrow;
}

/// Assign LOP object to *this
LOP::LOP(const LOP &lop) {
    this->nrow = lop.nrow;
    this->ncol = lop.ncol;
}

/// Overload = operator
const LOP &LOP::operator=(const LOP &lop) {
    this->nrow = lop.nrow;
    this->ncol = lop.ncol;
    return *this;
}

/// Get row space dimension
INT LOP::GetRowSize() const {
    return this->nrow;
}

/// Get column space dimension
INT LOP::GetColSize() const {
    return this->ncol;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/