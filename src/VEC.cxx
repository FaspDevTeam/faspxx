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
void VEC::GetArray(const INT& sizeNeed, DBL *array) const {
    if ( sizeNeed == 0 || this->size == 0 ) {
        delete[] array;
        array = nullptr;
        return;
    }

    INT len = sizeNeed > this->size ? this->size : sizeNeed;
    delete[] array;
    array=nullptr;
    array = new DBL[len];
    for ( INT j = 0; j < len; j++ )
        array[j] = this->values[j];
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

//-----------------------------------------------------------------------------------

/// Get array = (*this) of size = min(size, this->GetSize())
void VEC::GetArray2(const INT &sizeNeed, DBL *array) const {
    if (sizeNeed == 0 || this->size == 0) {
        delete[] array;
        array = nullptr;
        return;
    }

    INT len = sizeNeed > this->size ? this->size : sizeNeed;
    delete[] array;
    array = nullptr;
    array = new DBL[len];

    int len4 = len - len % 4;

    for (int j = 0; j < len4; j += 4) {
        array[j] = this->values[j];
        array[j + 1] = this->values[j + 1];
        array[j + 2] = this->values[j + 2];
        array[j + 3] = this->values[j + 3];
    }
    for (int j = len4; j < len; j++) array[j] = this->values[j];
}

/// Scale (*this)[j] = a * (*this)[j] by a scalar
void VEC::Scale2(const DBL &a) {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] *= a;
        this->values[j + 1] *= a;
        this->values[j + 2] *= a;
        this->values[j + 3] *= a;
    }

    for (INT j = len; j < this->size; j++) this->values[j] *= a;
}

/// Scale by a vector (*this)[j] *= v[j]
void VEC::PointwiseMult2(const VEC &v) {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] *= v.values[j];
        this->values[j + 1] *= v.values[j + 1];
        this->values[j + 2] *= v.values[j + 2];
        this->values[j + 3] *= v.values[j + 3];
    }

    for (INT j = len; j < this->size; j++) this->values[j] *= v.values[j];
}

/// Compute (*this)[j] = 1 / (*this)[j]
void VEC::Reciprocal2() {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] = 1 / this->values[j];
        this->values[j + 1] = 1 / this->values[j + 1];
        this->values[j + 2] = 1 / this->values[j + 2];
        this->values[j + 3] = 1 / this->values[j + 3];
    }

    for (INT j = len; j < this->size; j++) this->values[j] = 1 / this->values[j];
}

/// Inverse scale by a nonzero vector (*this)[j] = (*this)[j] / v[j]
void VEC::PointwiseDivide2(const VEC &v) {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] /= v.values[j];
        this->values[j + 1] /= v.values[j + 1];
        this->values[j + 2] /= v.values[j + 2];
        this->values[j + 3] /= v.values[j + 3];
    }

    for (INT j = len; j < this->size; j++) this->values[j] /= v.values[j];
}

/// Shift (*this)[j] += a by a scalar
void VEC::Shift2(const DBL &a) {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] += a;
        this->values[j + 1] += a;
        this->values[j + 2] += a;
        this->values[j + 3] += a;
    }

    for (INT j = len; j < this->size; j++) this->values[j] += a;
}

/// Compute *this = abs(*this)
void VEC::Abs2() {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] = fabs(this->values[j]);
        this->values[j + 1] = fabs(this->values[j + 1]);
        this->values[j + 2] = fabs(this->values[j + 2]);
        this->values[j + 3] = fabs(this->values[j + 3]);
    }

    for (INT j = len; j < this->size; j++) this->values[j] = fabs(this->values[j]);
}

/// *this = a * *this + b * vec
void VEC::Add2(const DBL &a, const DBL &b, const VEC &v) {
    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] = a * this->values[j] + b * v.values[j];
        this->values[j + 1] = a * this->values[j + 1] + b * v.values[j + 1];
        this->values[j + 2] = a * this->values[j + 2] + b * v.values[j + 2];
        this->values[j + 3] = a * this->values[j + 3] + b * v.values[j + 3];
    }

    for (INT j = len; j < this->size; j++)
        this->values[j] = a * this->values[j] + b * v.values[j];
}

/// *this = a * vec1 + b * vec2
void VEC::Add2(const DBL &a, const VEC &v1, const DBL &b, const VEC &v2) {
    this->size = v1.size;
    this->values.resize(v1.size);

    int len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        this->values[j] = a * v1.values[j] + b * v2.values[j];
        this->values[j] = a * v1.values[j] + b * v2.values[j];
        this->values[j] = a * v1.values[j] + b * v2.values[j];
        this->values[j] = a * v1.values[j] + b * v2.values[j];
    }
    for (INT j = len; j < v1.size; j++)
        this->values[j] = a * v1.values[j] + b * v2.values[j];
}

/// Find max(*this)
DBL VEC::Max2() const {
    DBL max1 = this->values[0];
    DBL max2 = this->values[1];
    DBL max3 = this->values[2];
    DBL max4 = this->values[3];

    int len = this->size - this->size % 4;

    for (int j = 4; j < len; j += 4) {
        if (max1 < this->values[j]) max1 = this->values[j];
        if (max2 < this->values[j + 1]) max2 = this->values[j + 1];
        if (max3 < this->values[j + 2]) max3 = this->values[j + 2];
        if (max4 < this->values[j + 3]) max4 = this->values[j + 3];
    }

    for (int j = len; j < this->size; j++) {
        if (max1 < this->values[j])
            max1 = this->values[j];
    }

    max1 = max1 >= max2 ? max1 : max2;
    max3 = max3 >= max4 ? max3 : max4;

    return max1 >= max3 ? max1 : max3;
}

/// Find min(*this)
DBL VEC::Min2() const {
    DBL min1 = this->values[0];
    DBL min2 = this->values[1];
    DBL min3 = this->values[2];
    DBL min4 = this->values[3];

    int len = this->size - this->size % 4;

    for (int j = 4; j < len; j += 4) {
        if (min1 > this->values[j]) min1 = this->values[j];
        if (min2 > this->values[j + 1]) min2 = this->values[j + 1];
        if (min3 > this->values[j + 2]) min3 = this->values[j + 2];
        if (min4 > this->values[j + 3]) min4 = this->values[j + 3];
    }

    for (int j = len; j < this->size; j++) {
        if (min1 > this->values[j])
            min1 = this->values[j];
    }

    min1 = min1 <= min2 ? min1 : min2;
    min3 = min3 <= min4 ? min3 : min4;

    return min1 <= min3 ? min1 : min3;
}

/// Compute Euclidean norm of *this
DBL VEC::Norm22() const {
    DBL tmp1 = 0.0;
    DBL tmp2 = 0.0;
    DBL tmp3 = 0.0;
    DBL tmp4 = 0.0;

    INT len = this->size - this->size % 4;

    for (INT j = 0; j < len; j += 4) {
        tmp1 += this->values[j] * this->values[j];
        tmp2 += this->values[j + 1] * this->values[j + 1];
        tmp3 += this->values[j + 2] * this->values[j + 2];
        tmp4 += this->values[j + 3] * this->values[j + 3];
    }

    for (INT j = len; j < this->size; j++)
        tmp1 += this->values[j] * this->values[j];

    return sqrt(tmp1 + tmp2 + tmp3 + tmp4);
}

/// Compute Infinity norm of *this
DBL VEC::NormInf2() const {

    INT len = this->size - this->size % 4;
    DBL normtmp1 = fabs(this->values[0]);
    DBL normtmp2 = fabs(this->values[1]);
    DBL normtmp3 = fabs(this->values[2]);
    DBL normtmp4 = fabs(this->values[3]);
    DBL tmp1, tmp2, tmp3, tmp4;

    for (INT j = 4; j < len; j += 4) {
        tmp1 = fabs(this->values[j]);
        tmp2 = fabs(this->values[j + 1]);
        tmp3 = fabs(this->values[j + 2]);
        tmp4 = fabs(this->values[j + 3]);
        if (tmp1 > normtmp1) normtmp1 = tmp1;
        if (tmp2 > normtmp2) normtmp2 = tmp2;
        if (tmp3 > normtmp3) normtmp3 = tmp3;
        if (tmp4 > normtmp4) normtmp4 = tmp4;
    }

    for (INT j = len; j < this->size; j++) {
        tmp1 = fabs(this->values[j]);
        if (normtmp1 < tmp1) normtmp1 = tmp1;
    }

    normtmp1 = normtmp1 >= normtmp2 ? normtmp1 : normtmp2;
    normtmp3 = normtmp3 >= normtmp4 ? normtmp3 : normtmp4;

    return (normtmp1 > normtmp3 ? normtmp1 : normtmp3);
}

/// Dot product of *this and vec
DBL VEC::Dot2(const VEC &v) const {
    DBL dot1 = 0.0, dot2 = 0.0, dot3 = 0.0, dot4 = 0.0;
    INT len = this->size - this->size % 4;

    for (int j = 0; j < len; j += 4) {
        dot1 += this->values[j] * v.values[j];
        dot2 += this->values[j + 1] * v.values[j + 1];
        dot3 += this->values[j + 2] * v.values[j + 2];
        dot4 += this->values[j + 3] * v.values[j + 3];
    }

    for (INT j = len; j < this->size; j++) dot1 += this->values[j] * v.values[j];

    return (dot1 + dot2 + dot3 + dot4);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/