/*! \file VEC.cxx
 *  \brief Vector class definition
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cmath>
#include "VEC.hxx"

/// Assign the size and the same value to a VEC object
VEC::VEC(const INT& sizeNeed, const DBL& valueNeed) {
    this->values.assign(sizeNeed, valueNeed);
    this->size = sizeNeed;
}

/// Assign a vector object to a VEC object
VEC::VEC(const std::vector<DBL>& vt) {
    this->values.operator=(vt);
    this->size = vt.size();
}

/// Assign a const VEC object to a VEC object
VEC::VEC(const VEC& v) {
    this->values.operator=(v.values);
    this->size = v.size;
}

/// Assign a DBL array to a VEC object
VEC::VEC(const INT& size, const DBL *ptr) {
    if ( ptr == nullptr || size == 0 ) {
        this->values.resize(0);
        this->size = 0;
        return;
    }
    this->values.assign(ptr, ptr + size);
    this->size = size;
}

/// Overload = operator
VEC& VEC::operator=(const VEC& v) {
    this->values.operator=(v.values);
    this->size = v.size;
    return *this;
}

/// Overload [] operator, entries can be modified
DBL& VEC::operator[](const INT& position) {
    return this->values[position];
}

/// Overload [] operator, entries cannot be modified
const DBL& VEC::operator[](const INT& position) const {
    return this->values[position];
}

/// Overload += operator
VEC& VEC::operator+=(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] += v.values[j];
    return *this;
}

/// Overload -= operator
VEC& VEC::operator-=(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] -= v.values[j];
    return *this;
}

/// Set the size of VEC object and reserve memory
void VEC::SetSize(const INT& sizeNeed) {
    this->size = sizeNeed;
    this->values.resize(sizeNeed); // Must set the size of vector as well!!!
}

/// Assign the size and the same value to a VEC object
void VEC::SetValues(const INT& sizeNeed, const DBL& valueNeed) {
    this->size = sizeNeed;
    this->values.assign(sizeNeed, valueNeed);
}

/// Assign a vector object to a VEC object
void VEC::SetValues(const std::vector<DBL>& vt) {
    this->values.operator=(vt);
    this->size = vt.size();
}

/// Assign a DBL array to a VEC object, user should allocate memory for array
void VEC::SetValues(const INT& sizeNeed, const DBL *array) {
    if ( array == nullptr || sizeNeed == 0 ) {
        this->size = 0;
        this->values.resize(0);
        return; // return an empty VEC object!
    }
    this->values.assign(array, array + sizeNeed);
    this->size = sizeNeed;
}

/// Get the value of (*this)[position]
DBL VEC::GetValue(const INT& position) const {
    return this->values.at(position);
}

/// Get array = (*this) of size = min(size, this->GetSize())
// Todo: Why use **array??? TEST(VEC_Scale, Scale) not Pass!!!
void VEC::GetArray(const INT& sizeNeed, DBL **array) const {
    if ( sizeNeed == 0 || this->size == 0 ) {
        delete[] *array;
        *array = nullptr;
        return;
    }

    INT len = sizeNeed > this->size ? this->size : sizeNeed;
    delete[] *array;
    *array=nullptr;
    *array = new DBL[len];
    for ( INT j = 0; j < len; j++ )
        (*array)[j] = this->values[j];
}

/// Get the size of *this
INT VEC::GetSize() const {
    return this->size;
}

/// Get the capacity of *this
INT VEC::GetCapacity() const {
    return this->values.capacity();
}

/// Scale (*this)[j] = a * (*this)[j] by a scalar
void VEC::Scale(const DBL& a) {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] *= a;
}

/// Scale by a vector (*this)[j] *= v[j]
void VEC::PointwiseMult(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] *= v.values[j];
}

/// Compute (*this)[j] = 1 / (*this)[j]
void VEC::Reciprocal() {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] = 1 / this->values[j];
}

/// Inverse scale by a nonzero vector (*this)[j] = (*this)[j] / v[j]
void VEC::PointwiseDivide(const VEC& v) {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] /= v.values[j];
}

/// Copy *this to v
void VEC::CopyTo(VEC& vec) const {
    vec.values.operator=(this->values);
    vec.size = this->size;
}

/// Shift (*this)[j] += a by a scalar
void VEC::Shift(const DBL& a) {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] += a;
}

/// Compute *this = abs(*this)
void VEC::Abs() {
    for ( INT j = 0; j < this->size; j++ ) this->values[j] = fabs(this->values[j]);
}

/// *this = a * *this + b * vec
void VEC::Add(const DBL& a, const DBL& b, const VEC& v) {
    for ( INT j = 0; j < this->size; j++ )
        this->values[j] = a * this->values[j] + b * v.values[j];
}

/// *this = a * vec1 + b * vec2
void VEC::Add(const DBL& a, const VEC& v1, const DBL& b, const VEC& v2) {
    this->size = v1.size;
    this->values.resize(v1.size);
    for ( INT j = 0; j < v1.size; j++ ) this->values[j] = a * v1.values[j] + b * v2.values[j];
}

/// Find max(*this)
DBL VEC::Max() const {
    DBL max = this->values[0];
    for ( INT j = 1; j < this->size; j++ ) {
        if ( max < this->values[j] ) max = this->values[j];
    }
    return max;
}

/// Find min(*this)
DBL VEC::Min() const {
    DBL min = this->values[0];
    for ( INT j = 1; j < this->size; j++ ) {
        if ( min > this->values[j] ) min = this->values[j];
    }
    return min;
}

/// Compute Euclidean norm of *this
DBL VEC::Norm2() const {
    DBL norm2 = 0.0;
    for ( INT j = 0; j < this->size; j++ ) norm2 += this->values[j] * this->values[j];
    return sqrt(norm2);
}

/// Compute Infinity norm of *this
DBL VEC::NormInf() const {
    DBL normInf = fabs(this->values[0]), absValue;
    for ( INT j = 1; j < this->size; j++ ) {
        absValue = fabs(this->values[j]);
        if ( normInf < absValue ) normInf = absValue;
    }
    return normInf;
}

/// Dot product of *this and vec
DBL VEC::Dot(const VEC& v) const {
    DBL dot = 0.0;
    for ( INT j = 0; j < this->size; j++ ) dot += this->values[j] * v.values[j];
    return dot;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/