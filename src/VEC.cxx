/*! \file vec.cxx
 *  \brief Source file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cmath>
#include "VEC.hxx"

//! set the size of vec and set the same value on VEC object
VEC::VEC(const INT &size, const DBL &value) {
    this->vec.assign(size, value);
    this->size = size;
}

//! assign vector object to VEC object
VEC::VEC(const std::vector<DBL> &vt) {
    this->vec.operator=(vt);
    this->size = vt.size();
}

//! assign VEC object to VEC object
VEC::VEC(const VEC &v) {
    this->vec.operator=(v.vec);
    this->size = v.size;
}

//! assign pointer to VEC object
VEC::VEC(const INT &size, const DBL *ptr) {
    if ( ptr == nullptr || size == 0 ) {
        this->vec.resize(0);
        this->size = 0;
        return;
    }
    this->vec.assign(ptr, ptr + size);
    this->size = size;
}

//! overload equals operator
VEC &VEC::operator=(const VEC &v) {
    this->vec.operator=(v.vec);
    this->size = v.size;
    return *this;
}

//! overload [] operator
DBL &VEC::operator[](const INT &position) {
    return this->vec[position];
}

//! overload [] operator
const DBL &VEC::operator[](const INT &position) const {
    return this->vec[position];
}

//! overload += operator
VEC &VEC::operator+=(const VEC &v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] += v.vec[j];
    return *this;
}

//! overload -= operator
VEC &VEC::operator-=(const VEC &v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] -= v.vec[j];
    return *this;
}

//! reserve the memory for VEC object only
void VEC::SetSize(const INT &size) {
    this->size = size;
    this->vec.resize(size);
}

//! set the size of VEC object and set the same value on VEC object
void VEC::SetValues(const INT &size, const DBL &value) {
    this->size = size;
    this->vec.assign(size, value);
}

//! assign vector object to VEC object
void VEC::SetValues(const std::vector<DBL> &vt) {
    this->vec.operator=(vt);
    this->size = vt.size();
}

//! *this = array
void VEC::SetValues(const INT &size, const DBL *array) {
    if ( array == nullptr || size == 0 ) {
        this->size = 0;
        this->vec.resize(0);
        return;
    }

    this->vec.assign(array, array + size);
    this->size = size;
}

//! get the value of (*this)[position]
DBL VEC::GetValue(const INT &position) const {
    return this->vec.at(position);
}

//! get array = *this of size = min(size, this->GetSize())
// Todo: Need to fix **array!!!
void VEC::GetArray(const INT &size, DBL **array) const {
    if ( size == 0 || this->size == 0 ) {
        delete[] *array;
        *array = nullptr;
        return;
    }

    INT len = size > this->size ? this->size : size;
    delete[] *array;
    *array=nullptr;
    *array = new DBL[len];
    for ( INT j = 0; j < len; j++ )
        (*array)[j] = this->vec[j];
}

//! get the size of *this
INT VEC::GetSize() const {
    return this->size;
}

//! get the capacity of VEC object
INT VEC::GetCapacity() const {
    return this->vec.capacity();
}

//! scale (*this)[j] = a * (*this)[j] by a scalar
void VEC::Scale(const DBL &a) {
    if ( this->size == 0 )
        return;
    else {
        for ( INT j = 0; j < this->size; j++ ) this->vec[j] *= a;
    }
}

//! copy vec = *this
void VEC::CopyTo(VEC &vec) const {
    vec.vec.operator=(this->vec);
    vec.size = this->size;
}

//! find max(*this)
DBL VEC::Max() const {
    DBL max = this->vec[0];
    for ( INT j = 1; j < this->size; j++ ) {
        if ( max < this->vec[j] ) max = this->vec[j];
    }
    return max;
}

//! find min(*this)
DBL VEC::Min() const {
    DBL min = this->vec[0];
    for ( INT j = 1; j < this->size; j++ ) {
        if ( min > this->vec[j] ) min = this->vec[j];
    }
    return min;
}

//! shift (*this)[j] += a
void VEC::Shift(const DBL &a) {
    if ( this->size == 0 ) return; // do nothing
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] += a;
}

//! compute abs((*this)[j])
void VEC::Abs() {
    if ( this->size == 0 ) return; // do nothing
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] = fabs(this->vec[j]);
}

//! (*this)[j] = 1 / (*this)[j]
void VEC::Reciprocal() {
    if ( this->size == 0 ) return; // do nothing
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] = 1 / this->vec[j];
}

//! find l2-norm of *this
DBL VEC::Norm2() const {
    DBL norm2 = 0.0;
    for ( INT j = 0; j < this->size; j++ ) norm2 += this->vec[j] * this->vec[j];
    return sqrt(norm2);
}

//! find infinity norm of *this
DBL VEC::NormInf() const {
    DBL normInf = fabs(this->vec[0]), absValue;
    for ( INT j = 1; j < this->size; j++ ) {
        absValue = fabs(this->vec[j]);
        if ( normInf < absValue ) normInf = absValue;
    }
    return normInf;
}

//! *this = a * *this + b * vec
void VEC::Add(const VEC &v, const DBL &a, const DBL &b) {
    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] = a * this->vec[j] + b * v.vec[j];
}

//! *this = a * vec1 + b * vec2
void VEC::Add(const DBL &a, const VEC &v1, const DBL &b, const VEC &v2) {
    this->size = v1.size;
    this->vec.resize(v1.size);
    for ( INT j = 0; j < v1.size; j++ ) this->vec[j] = a * v1.vec[j] + b * v2.vec[j];
}

//! dot product of *this and vec
DBL VEC::Dot(const VEC &v) const {
    DBL dot = 0.0;
    for ( INT j = 0; j < this->size; j++ ) dot += this->vec[j] * v.vec[j];
    return dot;
}

//! scale (*this)[j] *= vec[j] by a vector
void VEC::PointwiseMult(const VEC &v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] *= v.vec[j];
}

//! (*this)[j] = (*this)[j] / vec[j]
void VEC::PointwiseDivide(const VEC &v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] /= v.vec[j];
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/