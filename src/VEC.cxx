/*! \file VEC.cxx
 *  \brief Vector class definition
 *  \author Chensong Zhang, Kailei Zhang
 *  \date Oct/13/2019
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
    this->values = vt;
    this->size = vt.size();
}

/// Assign a const VEC object to a VEC object
VEC::VEC(const VEC& v) {
    this->values = v.values;
    this->size = v.size;
}

/// Assign a DBL array to a VEC object
VEC::VEC(const INT& size, const DBL *ptr) {
    if (ptr == nullptr || size == 0) {
        this->size = 0;
        return;
    }
    this->values.assign(ptr, ptr + size);
    this->size = size;
}

/// Overload = operator
VEC &VEC::operator=(const VEC& v) {
    this->values = v.values;
    this->size = v.size;
    return *this;
}

/// Overload [] operator, entries can be modified
DBL &VEC::operator[](const INT& position) {
    return this->values[position];
}

/// Overload [] operator, entries cannot be modified
const DBL &VEC::operator[](const INT& position) const {
    return this->values[position];
}

/// Overload += operator
VEC &VEC::operator+=(const VEC& v) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] += v.values[i];
        this->values[i + 1] += v.values[i + 1];
        this->values[i + 2] += v.values[i + 2];
        this->values[i + 3] += v.values[i + 3];
    }
    for (i = len; i < this->size; ++i) this->values[i] += v.values[i];
    return *this;
}

/// Overload -= operator
VEC &VEC::operator-=(const VEC& v) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] -= v.values[i];
        this->values[i + 1] -= v.values[i + 1];
        this->values[i + 2] -= v.values[i + 2];
        this->values[i + 3] -= v.values[i + 3];
    }
    for (i = len; i < this->size; ++i) this->values[i] -= v.values[i];
    return *this;
}

/// Set the size of VEC object and reserve memory
void VEC::Reserve(const INT& sizeNeed) {
    this->values.reserve(sizeNeed);
}

/// Shrink *this 's memory to this->size
void VEC::ShrinkToSize() {
    this->values.shrink_to_fit();
}

/// Assign the size and the same value to a VEC object
void VEC::SetValues(const INT& sizeNeed, const DBL& valueNeed) {
    this->size = sizeNeed;
    this->values.assign(sizeNeed, valueNeed);
}

/// Assign a vector object to a VEC object
void VEC::SetValues(const std::vector<DBL>& vt) {
    this->values = vt;
    this->size = vt.size();
}

/// Assign a DBL array to a VEC object, user should allocate memory for array
void VEC::SetValues(const INT& sizeNeed, const DBL *array) {
    if (array == nullptr || sizeNeed == 0) {
        this->values.resize(0);
        this->size = 0;
        return; // return an empty VEC object!
    }
    this->values.assign(array, array + sizeNeed);
    this->size = sizeNeed;
}

/// Get the value of (*this)[position]
DBL VEC::GetValue(const INT& position) const {
    return this->values.at(position);
}

/// Get array = the set of { this->values[indexPtr[j]] mod this->size } and size is
/// the size of indexPtr and parameter array
void VEC::GetValues(const INT& size, const INT *indexPtr, DBL *array) const {
    if (size == 0 || this->size == 0) {
        array = nullptr;
        return;
    }
    for (INT j = 0; j < size; ++j) array[j] = this->values[indexPtr[j] % this->size];
}

/// Get the size of *this
INT VEC::GetSize() const {
    return this->size;
}

/// Get the capacity of *this
INT VEC::GetCapacity() const {
    return this->values.capacity();
}

/// Scale (*this)[i] = a * (*this)[i] by a scalar
void VEC::Scale(const DBL& a) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] *= a;
        this->values[i + 1] *= a;
        this->values[i + 2] *= a;
        this->values[i + 3] *= a;
    }
    for (i = len; i < this->size; ++i) this->values[i] *= a;
}

/// Scale by a vector (*this)[i] *= v[i]
void VEC::PointwiseMult(const VEC& v) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] *= v.values[i];
        this->values[i + 1] *= v.values[i + 1];
        this->values[i + 2] *= v.values[i + 2];
        this->values[i + 3] *= v.values[i + 3];
    }
    for (i = len; i < this->size; ++i) this->values[i] *= v.values[i];
}

/// Compute (*this)[i] = 1 / (*this)[i]
void VEC::Reciprocal() {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] = 1.0 / this->values[i];
        this->values[i + 1] = 1.0 / this->values[i + 1];
        this->values[i + 2] = 1.0 / this->values[i + 2];
        this->values[i + 3] = 1.0 / this->values[i + 3];
    }
    for (i = len; i < this->size; ++i) this->values[i] = 1 / this->values[i];
}

/// Inverse scale by a nonzero vector (*this)[i] = (*this)[i] / v[i]
void VEC::PointwiseDivide(const VEC& v) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] /= v.values[i];
        this->values[i + 1] /= v.values[i + 1];
        this->values[i + 2] /= v.values[i + 2];
        this->values[i + 3] /= v.values[i + 3];
    }
    for (i = len; i < this->size; ++i) this->values[i] /= v.values[i];
}

/// Copy *this to v
void VEC::CopyTo(VEC& v) const {
    v.values = this->values;
    v.size = this->size;
}

/// Shift (*this)[i] += a by a scalar
void VEC::Shift(const DBL& a) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] += a;
        this->values[i + 1] += a;
        this->values[i + 2] += a;
        this->values[i + 3] += a;
    }
    for (i = len; i < this->size; ++i) this->values[i] += a;
}

/// Compute *this = abs(*this)
void VEC::Abs() {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] = fabs(this->values[i]);
        this->values[i + 1] = fabs(this->values[i + 1]);
        this->values[i + 2] = fabs(this->values[i + 2]);
        this->values[i + 3] = fabs(this->values[i + 3]);
    }
    for (i = len; i < this->size; ++i) this->values[i] = fabs(this->values[i]);
}

/// y = y + a * x
void VEC::AXPY(const DBL& a, const VEC& x) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] += a * x.values[i];
        this->values[i + 1] += a * x.values[i + 1];
        this->values[i + 2] += a * x.values[i + 2];
        this->values[i + 3] += a * x.values[i + 3];
    }
    for (i = len; i < this->size; ++i) this->values[i] += a * x.values[i];
}

/// y = x + a * y
void VEC::XPAY(const DBL& a, const VEC& x) {
    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        this->values[i] = a * this->values[i] + x.values[i];
        this->values[i + 1] = a * this->values[i + 1] + x.values[i + 1];
        this->values[i + 2] = a * this->values[i + 2] + x.values[i + 2];
        this->values[i + 3] = a * this->values[i + 3] + x.values[i + 3];
    }
    for (i = len; i < this->size; ++i)
        this->values[i] = a * this->values[i] + x.values[i];
}

/// *this = a * *this + b * v
void VEC::AXPBY(const DBL& a, const DBL& b, const VEC& v) {

    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    switch ((a == 1.0) + 2 * (b == 1.0)) {
        case 0:
            for (i = 0; i < len; i += 4) {
                this->values[i] = a * this->values[i] + b * v.values[i];
                this->values[i + 1] = a * this->values[i + 1] + b * v.values[i + 1];
                this->values[i + 2] = a * this->values[i + 2] + b * v.values[i + 2];
                this->values[i + 3] = a * this->values[i + 3] + b * v.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] = a * this->values[i] + b * v.values[i];
            break;
        case 1:
            for (i = 0; i < len; i += 4) {
                this->values[i] += b * v.values[i];
                this->values[i + 1] += b * v.values[i + 1];
                this->values[i + 2] += b * v.values[i + 2];
                this->values[i + 3] += b * v.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] += b * v.values[i];
            break;
        case 2:
            for (i = 0; i < len; i += 4) {
                this->values[i] = a * this->values[i] + v.values[i];
                this->values[i + 1] = a * this->values[i + 1] + v.values[i + 1];
                this->values[i + 2] = a * this->values[i + 2] + v.values[i + 2];
                this->values[i + 3] = a * this->values[i + 3] + v.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] = a * this->values[i] + v.values[i];
            break;
        case 3:
            for (i = 0; i < len; i += 4) {
                this->values[i] += v.values[i];
                this->values[i + 1] += v.values[i + 1];
                this->values[i + 2] += v.values[i + 2];
                this->values[i + 3] += v.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] += v.values[i];
    }

}

/// *this = a * vec1 + b * vec2
void VEC::WAXPBY(const DBL& a, const VEC& v1, const DBL& b, const VEC& v2) {

    this->size = v1.size;

    // unroll long for loops
    INT i;
    const INT len = this->size - this->size % 4;
    switch ((a == 1) + 2 * (b == 1)) {
        case 0:
            for (i = 0; i < len; i += 4) {
                this->values[i] = a * v1.values[i] + b * v2.values[i];
                this->values[i + 1] = a * v1.values[i + 1] + b * v2.values[i + 1];
                this->values[i + 2] = a * v1.values[i + 2] + b * v2.values[i + 2];
                this->values[i + 3] = a * v1.values[i + 3] + b * v2.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] = a * v1.values[i] + b * v2.values[i];
            break;
        case 1:
            for (i = 0; i < len; i += 4) {
                this->values[i] = v1.values[i] + b * v2.values[i];
                this->values[i + 1] = v1.values[i + 1] + b * v2.values[i + 1];
                this->values[i + 2] = v1.values[i + 2] + b * v2.values[i + 2];
                this->values[i + 3] = v1.values[i + 3] + b * v2.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] = v1.values[i] + b * v2.values[i];

            break;
        case 2:
            for (i = 0; i < len; i += 4) {
                this->values[i] = a * v1.values[i] + v2.values[i];
                this->values[i + 1] = a * v1.values[i + 1] + v2.values[i + 1];
                this->values[i + 2] = a * v1.values[i + 2] + v2.values[i + 2];
                this->values[i + 3] = a * v1.values[i + 3] + v2.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] = a * v1.values[i] + v2.values[i];
            break;
        case 3:
            for (i = 0; i < len; i += 4) {
                this->values[i] = v1.values[i] + v2.values[i];
                this->values[i + 1] = v1.values[i + 1] + v2.values[i + 1];
                this->values[i + 2] = v1.values[i + 2] + v2.values[i + 2];
                this->values[i + 3] = v1.values[i + 3] + v2.values[i + 3];
            }
            for (i = len; i < this->size; ++i)
                this->values[i] = v1.values[i] + v2.values[i];
            break;
    }

}

/// Find max(*this)
DBL VEC::Max() const {
    DBL max1 = this->values[0], max2 = this->values[1];
    DBL max3 = this->values[2], max4 = this->values[3];

    // unroll long for loops and binary comparison
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 4; i < len; i += 4) {
        if (max1 < this->values[i]) max1 = this->values[i];
        if (max2 < this->values[i + 1]) max2 = this->values[i + 1];
        if (max3 < this->values[i + 2]) max3 = this->values[i + 2];
        if (max4 < this->values[i + 3]) max4 = this->values[i + 3];
    }
    for (i = len; i < this->size; ++i)
        if (max1 < this->values[i]) max1 = this->values[i];

    max1 = max1 >= max2 ? max1 : max2;
    max3 = max3 >= max4 ? max3 : max4;
    return max1 >= max3 ? max1 : max3;
}

/// Find min(*this)
DBL VEC::Min() const {
    DBL min1 = this->values[0], min2 = this->values[1];
    DBL min3 = this->values[2], min4 = this->values[3];

    // unroll long for loops and binary comparison
    INT i;
    const INT len = this->size - this->size % 4;
    for (i = 4; i < len; i += 4) {
        if (min1 > this->values[i]) min1 = this->values[i];
        if (min2 > this->values[i + 1]) min2 = this->values[i + 1];
        if (min3 > this->values[i + 2]) min3 = this->values[i + 2];
        if (min4 > this->values[i + 3]) min4 = this->values[i + 3];
    }
    for (i = len; i < this->size; ++i)
        if (min1 > this->values[i]) min1 = this->values[i];

    min1 = min1 <= min2 ? min1 : min2;
    min3 = min3 <= min4 ? min3 : min4;
    return min1 <= min3 ? min1 : min3;
}

/// Compute Euclidean norm of *this
DBL VEC::Norm2() const {
    // unroll long for loops
    INT i;
    DBL tmp1 = 0.0, tmp2 = 0.0, tmp3 = 0.0, tmp4 = 0.0;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        tmp1 += std::pow(this->values[i], 2);
        tmp2 += std::pow(this->values[i + 1], 2);
        tmp3 += std::pow(this->values[i + 2], 2);
        tmp4 += std::pow(this->values[i + 3], 2);
    }
    for (i = len; i < this->size; ++i) tmp1 += std::pow(this->values[i], 2);

    return sqrt(tmp1 + tmp2 + tmp3 + tmp4);
}

/// Compute Infinity norm of *this
DBL VEC::NormInf() const {
    // unroll long for loops
    INT i;
    DBL tmpNorm1 = 0.0, tmpNorm2 = 0.0, tmpNorm3 = 0.0, tmpNorm4 = 0.0;
    DBL tmp1, tmp2, tmp3, tmp4;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        tmp1 = fabs(this->values[i]);
        if (tmp1 > tmpNorm1) tmpNorm1 = tmp1;
        tmp2 = fabs(this->values[i + 1]);
        if (tmp2 > tmpNorm2) tmpNorm2 = tmp2;
        tmp3 = fabs(this->values[i + 2]);
        if (tmp3 > tmpNorm3) tmpNorm3 = tmp3;
        tmp4 = fabs(this->values[i + 3]);
        if (tmp4 > tmpNorm4) tmpNorm4 = tmp4;
    }
    for (i = len; i < this->size; ++i) {
        tmp1 = fabs(this->values[i]);
        if (tmp1 > tmpNorm1) tmpNorm1 = tmp1;
    }

    tmpNorm1 = tmpNorm1 >= tmpNorm2 ? tmpNorm1 : tmpNorm2;
    tmpNorm3 = tmpNorm3 >= tmpNorm4 ? tmpNorm3 : tmpNorm4;
    return (tmpNorm1 > tmpNorm3 ? tmpNorm1 : tmpNorm3);
}

/// Dot product of with v
DBL VEC::Dot(const VEC& v) const {
    // unroll long for loops
    INT i;
    DBL dot1 = 0.0, dot2 = 0.0, dot3 = 0.0, dot4 = 0.0;
    const INT len = this->size - this->size % 4;
    for (i = 0; i < len; i += 4) {
        dot1 += this->values[i] * v.values[i];
        dot2 += this->values[i + 1] * v.values[i + 1];
        dot3 += this->values[i + 2] * v.values[i + 2];
        dot4 += this->values[i + 3] * v.values[i + 3];
    }
    for (i = len; i < this->size; ++i) dot1 += this->values[i] * v.values[i];
    return (dot1 + dot2 + dot3 + dot4);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/