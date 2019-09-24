/*! \file vec.cxx
 *  \brief Source file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "vec.hxx"
#include <cmath>

//! set the size of vec and set the same value on VEC object
VEC::VEC(const INT size, const DBL value) {
    this->vec_.assign(size, value);
    this->size_ = size;
}

//! assign vector object to VEC object
VEC::VEC(std::vector<DBL> vect) {
    this->vec_.operator=(vect);
    this->size_ = vect.size();
}

//! assign VEC object to VEC object
VEC::VEC(const VEC &vec) {
    this->vec_.operator=(vec.vec_);
    this->size_ = vec.size_;
}

//! assign pointer to VEC object
VEC::VEC(const INT size, const DBL *ptr) {
    if ( ptr == nullptr || size == 0 ) {
        this->vec_.resize(0);
        this->size_ = 0;
        return;
    }
    this->vec_.assign(ptr, ptr + size);
    this->size_ = size;
}

//! overload equals operator
VEC &VEC::operator=(const VEC &vec) {
    this->vec_.operator=(vec.vec_);
    this->size_ = vec.size_;

    return *this;
}

//! overload [] operator
DBL &VEC::operator[](const INT position) {
    return this->vec_[position];
}

//! overload [] operator
const DBL &VEC::operator[](const INT position) const {
    return this->vec_[position];
}

//! overload += operator
VEC &VEC::operator+=(const VEC &vec) {
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] += vec.vec_[j];

    return *this;
}

//! overload -= operator
VEC &VEC::operator-=(const VEC &vec) {
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] -= vec.vec_[j];

    return *this;
}

//! reserve the memory for VEC object only
void VEC::Reserve(const INT size) {
    this->vec_.reserve(size);
}

//! set the size of VEC object and set the same value on VEC object
void VEC::SetValues(const INT size, const DBL value) {
    this->size_ = size;
    this->vec_.assign(size, value);
}

//! assign vector object to VEC object
void VEC::SetValues(const std::vector<DBL> vect) {
    this->vec_.operator=(vect);
    this->size_ = vect.size();
}

//! *this = array
void VEC::SetValues(const INT size, const DBL *array) {
    if ( array == nullptr || size == 0 ) {
        this->size_ = 0;
        this->vec_.resize(0);
        return;
    }

    this->vec_.assign(array, array + size);
    this->size_ = size;
}

//! get the value of (*this)[position]
DBL VEC::Get(const INT position) const {
    return this->vec_.at(position);
}

//! get array = *this of size = min(size, this->GetSize())
void VEC::GetArray(const INT size, DBL **array) const {
    if ( size == 0 || this->size_ == 0 ) {
        delete[] *array;
        *array = nullptr;
        return;
    }

    INT len = size > this->size_ ? this->size_ : size;
    delete[] *array;
    *array=nullptr;
    *array = new DBL[len];
    for ( INT j = 0; j < len; j++ )
        (*array)[j] = this->vec_[j];
}

//! get the size of *this
INT VEC::GetSize() const {
    return this->size_;
}

//! get the capacity of VEC object
INT VEC::GetCapacity() {
    return this->vec_.capacity();
}

//! scale (*this)[j] = a * (*this)[j] by a scalar
void VEC::Scale(DBL a) {
    if ( this->size_ == 0 )
        return;
    else {
        for ( INT j = 0; j < this->size_; j++ )
            this->vec_[j] = a * this->vec_[j];
    }
}

//! copy vec = *this
void VEC::CopyTo(VEC &vec) const {
    vec.vec_.operator=(this->vec_);
    vec.size_ = this->size_;
}

//! find max(*this)
DBL VEC::Max() const {
    DBL max;
    if ( this->size_ == 1 )
        max = this->vec_[0];
    else {
        max = this->vec_[0];
        for ( INT j = 1; j < this->size_; j++ ) {
            if ( max < this->vec_[j] )
                max = this->vec_[j];
        }
    }

    return max;
}

//! find min(*this)
DBL VEC::Min() const {
    DBL min = 0;
    if ( this->size_ == 1 )
        min = this->vec_[0];
    else {
        min = this->vec_[0];
        for ( INT j = 1; j < this->size_; j++ ) {
            if ( min > this->vec_[j] )
                min = this->vec_[j];
        }
    }

    return min;
}

//! shift (*this)[j] += s by a scalar
void VEC::Shift(DBL s) {
    if ( this->size_ == 0 )
        this->vec_.resize(0);
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] += s;
}

//! compute abs((*this)[j])
void VEC::Abs() {
    if ( this->size_ == 0 ) {
        this->vec_.resize(0);
        return;
    }
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] = fabs(this->vec_[j]);
}

//! (*this)[j] = 1 / (*this)[j]
void VEC::Reciprocal(DBL tol) {
    if ( this->size_ == 0 ) {
        this->vec_.resize(0);
        return;
    }
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] = 1 / this->vec_[j];
}

//! find l2-norm of *this
DBL VEC::Norm2() const {
    DBL norm2 = 0.0;
    for ( INT j = 0; j < this->size_; j++ )
        norm2 += this->vec_[j] * this->vec_[j];

    return sqrt(norm2);
}

//! find infinity norm of *this
DBL VEC::NormInf() const {
    DBL normInf = 0.0;
    if ( this->size_ == 1 )
        normInf = fabs(this->vec_[0]);
    else {
        for ( INT j = 1; j < this->size_; j++ ) {
            if ( normInf < fabs(this->vec_[j]))
                normInf = fabs(this->vec_[j]);
        }
    }

    return normInf;
}

//! *this = a * *this + b * vec
void VEC::Add(const VEC &vec, const DBL a, const DBL b) {
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] = a * this->vec_[j] + b * vec.vec_[j];
}

//! *this = a * vec1 + b * vec2
void VEC::Add(const DBL a, const VEC &vec1, const DBL b, const VEC &vec2) {
    this->size_ = vec1.size_;
    this->vec_.resize(vec1.size_);
    for ( INT j = 0; j < vec1.size_; j++ )
        this->vec_[j] = a * vec1.vec_[j] + b * vec2.vec_[j];
}

//! dot product of *this and vec
DBL VEC::Dot(const VEC &vec) const {
    DBL dot = 0.0;
    for ( INT j = 0; j < this->size_; j++ )
        dot += this->vec_[j] * vec.vec_[j];

    return dot;
}

//! scale (*this)[j] *= vec[j] by a vector
void VEC::PointwiseMult(const VEC &vec) {
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] *= vec.vec_[j];
}

//! (*this)[j] = (*this)[j] / vec[j]
void VEC::PointwiseDivide(const VEC &vec) {
    for ( INT j = 0; j < this->size_; j++ )
        this->vec_[j] /= vec.vec_[j];
}