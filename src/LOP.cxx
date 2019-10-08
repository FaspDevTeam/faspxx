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
    this->nrow=nrow;
    this->ncol=ncol;
}

/// Assign nrow, ncol=nrow to *this
LOP::LOP(const INT& nrow){
    this->nrow=nrow;
    this->ncol=nrow;
}

/// Assign LOP object to *this
LOP::LOP(const LOP& lop){
    this->nrow=lop.nrow;
    this->ncol=lop.ncol;
}

/// Overload = operator
LOP& LOP::operator=(const LOP& lop){
    this->nrow=lop.nrow;
    this->ncol=lop.ncol;
}

/// Get row space dimension
INT LOP::GetRowSize() const{
    return this->nrow;
}

/// Get column space dimension
INT LOP::GetColSize() const{
    return this->ncol;
}

/// Copy *this to lop
void LOP::CopyTo(LOP& lop) const{
    lop.nrow=this->nrow;
    lop.ncol=this->ncol;
}

/// Transpose *this // Todo: Check???
void LOP::Transpose(){
    INT tmp;
    tmp=this->nrow;
    this->nrow=this->ncol;
    this->ncol=tmp;
}

/// Return VEC = *this * v
VEC LOP::MultVec(const VEC& v) const{
    //suppose *this is a identical mapping
    return v;

}
/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/