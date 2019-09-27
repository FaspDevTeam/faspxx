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

/// Assign the size and the same value to a VEC object
VEC::VEC(const INT& size, const DBL& value) {
    this->vec.assign(size, value);
    this->size = size;
}

/// Assign a vector object to a VEC object
VEC::VEC(const std::vector<DBL>& vt) {
    this->vec.operator=(vt);
    this->size = vt.size();
}

/// Assign a const VEC object to a VEC object
VEC::VEC(const VEC& v) {
    this->vec.operator=(v.vec);
    this->size = v.size;
}

/// Assign a DBL array to a VEC object
VEC::VEC(const INT& size, const DBL *ptr) {
    if ( ptr == nullptr || size == 0 ) {
        this->vec.resize(0);
        this->size = 0;
        return;
    }
    this->vec.assign(ptr, ptr + size);
    this->size = size;
}

/// Overload = operator
VEC& VEC::operator=(const VEC& v) {
    this->vec.operator=(v.vec);
    this->size = v.size;
    return *this;
}

/// Overload [] operator, entries can be modified
DBL& VEC::operator[](const INT& position) {
    return this->vec[position];
}

/// Overload [] operator, entries cannot be modified
const DBL& VEC::operator[](const INT& position) const {
    return this->vec[position];
}

/// Overload += operator
VEC& VEC::operator+=(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] += v.vec[j];
    return *this;
}

/// Overload -= operator
VEC& VEC::operator-=(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] -= v.vec[j];
    return *this;
}

/// Set the size of VEC object and reserve memory
void VEC::SetSize(const INT& size) {
    this->size = size;
    this->vec.resize(size);
}

/// Assign the size and the same value to a VEC object
void VEC::SetValues(const INT& size, const DBL& value) {
    this->size = size;
    this->vec.assign(size, value);
}

/// Assign a vector object to a VEC object
void VEC::SetValues(const std::vector<DBL>& vt) {
    this->vec.operator=(vt);
    this->size = vt.size();
}

/// Assign a DBL array to a VEC object, user should allocate memory for array
void VEC::SetValues(const INT& size, const DBL *array) {
    if ( array == nullptr || size == 0 ) {
        this->size = 0;
        this->vec.resize(0);
        return;
    }
    this->vec.assign(array, array + size);
    this->size = size;
}

/// Get the value of (*this)[position]
DBL VEC::GetValue(const INT& position) const {
    return this->vec.at(position);
}

/// Get array = (*this) of size = min(size, this->GetSize())
// Todo: Why use **array???
void VEC::GetArray(const INT& size, DBL **array) const {
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

/// Get the size of *this
INT VEC::GetSize() const {
    return this->size;
}

/// Get the capacity of *this
INT VEC::GetCapacity() const {
    return this->vec.capacity();
}

/// Scale (*this)[j] = a * (*this)[j] by a scalar
void VEC::Scale(const DBL& a) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] *= a;
}

/// Scale by a vector (*this)[j] *= v[j]
void VEC::PointwiseMult(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] *= v.vec[j];
}

/// Compute (*this)[j] = 1 / (*this)[j]
void VEC::Reciprocal() {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] = 1 / this->vec[j];
}

/// Inverse scale by a nonzero vector (*this)[j] = (*this)[j] / v[j]
void VEC::PointwiseDivide(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] /= v.vec[j];
}

/// Copy *this to v
void VEC::CopyTo(VEC& vec) const {
    vec.vec.operator=(this->vec);
    vec.size = this->size;
}

/// Shift (*this)[j] += a by a scalar
void VEC::Shift(const DBL& a) {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] += a;
}

/// Compute *this = abs(*this)
void VEC::Abs() {
    for ( INT j = 0; j < this->size; j++ ) this->vec[j] = fabs(this->vec[j]);
}

/// *this = a * vec + b * *this
void VEC::Add(const VEC& v, const DBL& a, const DBL& b) {
    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] = a * this->vec[j] + b * v.vec[j];
}

/// *this = a * vec1 + b * vec2
void VEC::Add(const DBL& a, const VEC& v1, const DBL& b, const VEC& v2) {
    this->size = v1.size;
    this->vec.resize(v1.size);
    for ( INT j = 0; j < v1.size; j++ ) this->vec[j] = a * v1.vec[j] + b * v2.vec[j];
}

/// Find max(*this)
DBL VEC::Max() const {
    DBL max = this->vec[0];
    for ( INT j = 1; j < this->size; j++ ) {
        if ( max < this->vec[j] ) max = this->vec[j];
    }
    return max;
}

/// Find min(*this)
DBL VEC::Min() const {
    DBL min = this->vec[0];
    for ( INT j = 1; j < this->size; j++ ) {
        if ( min > this->vec[j] ) min = this->vec[j];
    }
    return min;
}

/// Compute Euclidean norm of *this
DBL VEC::Norm2() const {
    DBL norm2 = 0.0;
    for ( INT j = 0; j < this->size; j++ ) norm2 += this->vec[j] * this->vec[j];
    return sqrt(norm2);
}

/// Compute Infinity norm of *this
DBL VEC::NormInf() const {
    DBL normInf = fabs(this->vec[0]), absValue;
    for ( INT j = 1; j < this->size; j++ ) {
        absValue = fabs(this->vec[j]);
        if ( normInf < absValue ) normInf = absValue;
    }
    return normInf;
}

/// Dot product of *this and vec
DBL VEC::Dot(const VEC& v) const {
    DBL dot = 0.0;
    for ( INT j = 0; j < this->size; j++ ) dot += this->vec[j] * v.vec[j];
    return dot;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/