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
    this->nrow = nrow;
    this->ncol = nrow;
}

/// Assign LOP object to *this
LOP::LOP(const LOP& lop){
    this->nrow = lop.nrow;
    this->ncol = lop.ncol;
}

/// Overload = operator
LOP& LOP::operator=(const LOP& lop){
    this->nrow = lop.nrow;
    this->ncol = lop.ncol;
    return *this;
}

/// Get row space dimension
INT LOP::GetRowSize() const{
    return this->nrow;
}

/// Get column space dimension
INT LOP::GetColSize() const{
    return this->ncol;
}

/// Get (*this)[i][j]
DBL LOP::GetValue(const INT& row, const INT& col) const{
    if (row==col)
        return 1.0;
    else
        return 0.0;
}

/// Get the whole row-th row in *this into VEC object
void LOP::GetRow(const INT& row, std::vector<DBL>& v) const{
    v.resize(1);
    v[0]=1;
}

/// Get the whole col-th column in *this into VEC object
void LOP::GetCol(const INT& col, std::vector<DBL>& v) const{
    v.resize(1);
    v[0]=1;
}

/// Get the whole diagonal entries in *this into VEC object
void LOP::GetDiag(std::vector<DBL>& v) const{
    v.resize(this->nrow>this->ncol?this->ncol:this->nrow);
    v.assign(v.size(),1.0);

}
/// Copy *this to lop
void LOP::CopyTo(LOP& lop) const{
    lop.nrow=this->nrow;
    lop.ncol=this->ncol;
}

///// Transpose *this // Todo: Check???
//void LOP::Transpose(){
//    INT tmp;
//    tmp=this->nrow;
//    this->nrow=this->ncol;
//    this->ncol=tmp;
//}

///// Return VEC = *this * v
//void LOP::Apply(const VEC& v, VEC &vt) const{
//    //suppose *this is a identical mapping
//    vt = v;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/