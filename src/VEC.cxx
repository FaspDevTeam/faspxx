/*! \file    VEC.cxx
 *  \brief   Vector class definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Oct/13/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cmath>
#include "VEC.hxx"

/// Assign the size and the same value to a VEC object.
VEC::VEC(const INT& size, const DBL& value)
{
    this->values.assign(size, value);
    this->size = size;
}

/// Assign a vector object to a VEC object.
VEC::VEC(const std::vector<DBL>& src)
{
    this->values = src;
    this->size = src.size();
}

/// Assign a const VEC object to a VEC object.
VEC::VEC(const VEC& src)
{
    this->values = src.values;
    this->size = src.size;
}

/// Assign a DBL array to a VEC object. If source is nullptr, return an empty VEC.
VEC::VEC(const INT& size, const DBL *src)
{
    if ( src == nullptr || size == 0 ) {
        this->size = 0;
        return;
    }
    this->values.assign(src, src + size);
    this->size = size;
}

/// Assignment for the VEC object.
VEC& VEC::operator=(const VEC& src)
{
    this->values = src.values;
    this->size = src.size;
    return *this;
}

/// Regular [] operator, same behavior as array.
DBL& VEC::operator[](const INT& position)
{
    return this->values[position];
}

/// Const [] operator, entries cannot be modified.
const DBL& VEC::operator[](const INT& position) const
{
    return this->values[position];
}

/// Unroll for loops to speed up calculation.
VEC& VEC::operator+=(const VEC& v)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     += v.values[i];
        this->values[i + 1] += v.values[i + 1];
        this->values[i + 2] += v.values[i + 2];
        this->values[i + 3] += v.values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) this->values[i] += v.values[i];
    return *this;
}

/// Unroll for loops to speed up calculation.
VEC& VEC::operator-=(const VEC& v)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     -= v.values[i];
        this->values[i + 1] -= v.values[i + 1];
        this->values[i + 2] -= v.values[i + 2];
        this->values[i + 3] -= v.values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) this->values[i] -= v.values[i];
    return *this;
}

/// Reserve memory for the vector values without changing the size.
void VEC::Reserve(const INT& size)
{
    this->values.reserve(size);
}

/// Assign a single value to a VEC object.
void VEC::SetValues(const INT& size, const DBL& value)
{
    this->size = size;
    this->values.assign(size, value);
}

/// Assign vector values to a VEC object.
void VEC::SetValues(const std::vector<DBL>& src)
{
    this->values = src;
    this->size = src.size();
}

/// Assign a DBL array to a VEC object. If source is nullptr, return an empty VEC.
void VEC::SetValues(const INT& size, const DBL *array)
{
    if ( array == nullptr || size == 0 ) {
        this->values.resize(0);
        this->size = 0;
        return;
    }
    this->values.assign(array, array + size);
    this->size = size;
}

/// Return the value of (*this)[position].
DBL VEC::GetValue(const INT& position) const
{
    return this->values.at(position);
}

/// Get multiple values and save them in an array.
void VEC::GetValues(const INT& size, const INT *index, DBL *array) const
{
    if ( size == 0 || this->size == 0 ) {
        array = nullptr;
        return;
    }
    for ( INT j = 0; j < size; ++j ) array[j] = this->values[index[j] % this->size];
}

/// The pointer array points this->values and it can be used to access data of VEC.
void VEC::GetArray(DBL **array)
{
    *array = this->values.data();
}

/// The pointer array points this->values and it can be used to access data of VEC.
/// The values cannot be modified.
void VEC::GetArray(const DBL **array) const
{
    *array = this->values.data();
}

/// Return the size of VEC.
INT VEC::GetSize() const
{
    return this->size;
}

/// (*this)[j] = a * (*this)[j], unroll long for loops.
void VEC::Scale(const DBL& a)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     *= a;
        this->values[i + 1] *= a;
        this->values[i + 2] *= a;
        this->values[i + 3] *= a;
    }
    for ( i = len; i < this->size; ++i ) this->values[i] *= a;
}

/// (*this)[j] *= v[j], unroll long for loops.
void VEC::PointwiseMult(const VEC& v)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     *= v.values[i];
        this->values[i + 1] *= v.values[i + 1];
        this->values[i + 2] *= v.values[i + 2];
        this->values[i + 3] *= v.values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) this->values[i] *= v.values[i];
}

/// (*this)[i] = 1 / (*this)[i], unroll long for loops.
void VEC::Reciprocal()
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     = 1.0 / this->values[i];
        this->values[i + 1] = 1.0 / this->values[i + 1];
        this->values[i + 2] = 1.0 / this->values[i + 2];
        this->values[i + 3] = 1.0 / this->values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) this->values[i] = 1 / this->values[i];
}

/// Divide by a nonzero vector (*this)[i] = (*this)[i] / v[i], unroll long for loops.
void VEC::PointwiseDivide(const VEC& v)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     /= v.values[i];
        this->values[i + 1] /= v.values[i + 1];
        this->values[i + 2] /= v.values[i + 2];
        this->values[i + 3] /= v.values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) this->values[i] /= v.values[i];
}

/// dst = *this.
void VEC::CopyTo(VEC& dst) const
{
    dst.values = this->values;
    dst.size = this->size;
}

/// (*this)[i] += a, unroll long for loops.
void VEC::Shift(const DBL& a)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     += a;
        this->values[i + 1] += a;
        this->values[i + 2] += a;
        this->values[i + 3] += a;
    }
    for ( i = len; i < this->size; ++i ) this->values[i] += a;
}

/// (*this)[i] = abs((*this)[i]), unroll long for loops.
void VEC::Abs()
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     = fabs(this->values[i]);
        this->values[i + 1] = fabs(this->values[i + 1]);
        this->values[i + 2] = fabs(this->values[i + 2]);
        this->values[i + 3] = fabs(this->values[i + 3]);
    }
    for ( i = len; i < this->size; ++i ) this->values[i] = fabs(this->values[i]);
}

/// y = a * x + y, unroll long for loops.
void VEC::AXPY(const DBL& a, const VEC& x)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     += a * x.values[i];
        this->values[i + 1] += a * x.values[i + 1];
        this->values[i + 2] += a * x.values[i + 2];
        this->values[i + 3] += a * x.values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) this->values[i] += a * x.values[i];
}

/// y = x + a * y, unroll long for loops.
void VEC::XPAY(const DBL& a, const VEC& x)
{
    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        this->values[i]     = a * this->values[i]     + x.values[i];
        this->values[i + 1] = a * this->values[i + 1] + x.values[i + 1];
        this->values[i + 2] = a * this->values[i + 2] + x.values[i + 2];
        this->values[i + 3] = a * this->values[i + 3] + x.values[i + 3];
    }
    for ( i = len; i < this->size; ++i )
        this->values[i] = x.values[i] + a * this->values[i];
}

/// x = a * x + b * y, unroll long for loops.
void VEC::AXPBY(const DBL& a, const DBL& b, const VEC& y)
{
    INT i;
    const INT len = this->size - this->size % 4;
    switch ( (a == 1.0) + 2 * (b == 1.0) ) {
        case 0:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     = a * this->values[i]     + b * y.values[i];
                this->values[i + 1] = a * this->values[i + 1] + b * y.values[i + 1];
                this->values[i + 2] = a * this->values[i + 2] + b * y.values[i + 2];
                this->values[i + 3] = a * this->values[i + 3] + b * y.values[i + 3];
            }
            for ( i = len; i < this->size; ++i )
                this->values[i] = a * this->values[i] + b * y.values[i];
            break;

        case 1:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     += b * y.values[i];
                this->values[i + 1] += b * y.values[i + 1];
                this->values[i + 2] += b * y.values[i + 2];
                this->values[i + 3] += b * y.values[i + 3];
            }
            for ( i = len; i < this->size; ++i ) this->values[i] += b * y.values[i];
            break;

        case 2:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     = a * this->values[i]     + y.values[i];
                this->values[i + 1] = a * this->values[i + 1] + y.values[i + 1];
                this->values[i + 2] = a * this->values[i + 2] + y.values[i + 2];
                this->values[i + 3] = a * this->values[i + 3] + y.values[i + 3];
            }
            for ( i = len; i < this->size; ++i )
                this->values[i] = a * this->values[i] + y.values[i];
            break;

        case 3:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     += y.values[i];
                this->values[i + 1] += y.values[i + 1];
                this->values[i + 2] += y.values[i + 2];
                this->values[i + 3] += y.values[i + 3];
            }
            for ( i = len; i < this->size; ++i ) this->values[i] += y.values[i];
    }
}

/// *this = a * v1 + b * v2, unroll long for loops.
void VEC::WAXPBY(const DBL& a, const VEC& v1, const DBL& b, const VEC& v2)
{
    INT i;
    this->size = v1.size;
    const INT len = this->size - this->size % 4;
    switch ( (a == 1) + 2 * (b == 1) ) {
        case 0:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     = a * v1.values[i]     + b * v2.values[i];
                this->values[i + 1] = a * v1.values[i + 1] + b * v2.values[i + 1];
                this->values[i + 2] = a * v1.values[i + 2] + b * v2.values[i + 2];
                this->values[i + 3] = a * v1.values[i + 3] + b * v2.values[i + 3];
            }
            for ( i = len; i < this->size; ++i )
                this->values[i] = a * v1.values[i] + b * v2.values[i];
            break;

        case 1:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     = v1.values[i]     + b * v2.values[i];
                this->values[i + 1] = v1.values[i + 1] + b * v2.values[i + 1];
                this->values[i + 2] = v1.values[i + 2] + b * v2.values[i + 2];
                this->values[i + 3] = v1.values[i + 3] + b * v2.values[i + 3];
            }
            for ( i = len; i < this->size; ++i )
                this->values[i] = v1.values[i] + b * v2.values[i];
            break;

        case 2:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     = a * v1.values[i]     + v2.values[i];
                this->values[i + 1] = a * v1.values[i + 1] + v2.values[i + 1];
                this->values[i + 2] = a * v1.values[i + 2] + v2.values[i + 2];
                this->values[i + 3] = a * v1.values[i + 3] + v2.values[i + 3];
            }
            for ( i = len; i < this->size; ++i )
                this->values[i] = a * v1.values[i] + v2.values[i];
            break;

        case 3:
            for ( i = 0; i < len; i += 4 ) {
                this->values[i]     = v1.values[i]     + v2.values[i];
                this->values[i + 1] = v1.values[i + 1] + v2.values[i + 1];
                this->values[i + 2] = v1.values[i + 2] + v2.values[i + 2];
                this->values[i + 3] = v1.values[i + 3] + v2.values[i + 3];
            }
            for ( i = len; i < this->size; ++i )
                this->values[i] = v1.values[i] + v2.values[i];
            break;
    }
}

/// Find maximal value, unroll long for loops.
DBL VEC::Max() const
{
    DBL max1 = SMALL, max2 = SMALL, max3 = SMALL, max4 = SMALL;

    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        if ( max1 < this->values[i] )     max1 = this->values[i];
        if ( max2 < this->values[i + 1] ) max2 = this->values[i + 1];
        if ( max3 < this->values[i + 2] ) max3 = this->values[i + 2];
        if ( max4 < this->values[i + 3] ) max4 = this->values[i + 3];
    }
    for ( i = len; i < this->size; ++i )
        if ( max1 < this->values[i] ) max1 = this->values[i];

    max1 = max1 >= max2 ? max1 : max2;
    max3 = max3 >= max4 ? max3 : max4;
    return max1 >= max3 ? max1 : max3;
}

/// Find min(*this), unroll long for loops.
DBL VEC::Min() const
{
    DBL min1 = LARGE, min2 = LARGE, min3 = LARGE, min4 = LARGE;

    INT i;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        if (min1 > this->values[i])     min1 = this->values[i];
        if (min2 > this->values[i + 1]) min2 = this->values[i + 1];
        if (min3 > this->values[i + 2]) min3 = this->values[i + 2];
        if (min4 > this->values[i + 3]) min4 = this->values[i + 3];
    }
    for ( i = len; i < this->size; ++i )
        if (min1 > this->values[i]) min1 = this->values[i];

    min1 = min1 <= min2 ? min1 : min2;
    min3 = min3 <= min4 ? min3 : min4;
    return min1 <= min3 ? min1 : min3;
}

/// Compute Euclidean norm of *this, unroll long for loops.
DBL VEC::Norm2() const
{
    INT i;
    DBL tmp1 = 0.0, tmp2 = 0.0, tmp3 = 0.0, tmp4 = 0.0;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        tmp1 += std::pow(this->values[i], 2);
        tmp2 += std::pow(this->values[i + 1], 2);
        tmp3 += std::pow(this->values[i + 2], 2);
        tmp4 += std::pow(this->values[i + 3], 2);
    }
    for ( i = len; i < this->size; ++i ) tmp1 += std::pow(this->values[i], 2);

    return sqrt(tmp1 + tmp2 + tmp3 + tmp4);
}

/// Compute infinity norm of *this, unroll long for loops.
DBL VEC::NormInf() const
{
    INT i;
    DBL tmpNorm1 = 0.0, tmpNorm2 = 0.0, tmpNorm3 = 0.0, tmpNorm4 = 0.0;
    DBL tmp1, tmp2, tmp3, tmp4;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        tmp1 = fabs(this->values[i]);
        if ( tmp1 > tmpNorm1 ) tmpNorm1 = tmp1;
        tmp2 = fabs(this->values[i + 1]);
        if ( tmp2 > tmpNorm2 ) tmpNorm2 = tmp2;
        tmp3 = fabs(this->values[i + 2]);
        if ( tmp3 > tmpNorm3 ) tmpNorm3 = tmp3;
        tmp4 = fabs(this->values[i + 3]);
        if ( tmp4 > tmpNorm4 ) tmpNorm4 = tmp4;
    }
    for ( i = len; i < this->size; ++i ) {
        tmp1 = fabs(this->values[i]);
        if ( tmp1 > tmpNorm1 ) tmpNorm1 = tmp1;
    }

    tmpNorm1 = tmpNorm1 >= tmpNorm2 ? tmpNorm1 : tmpNorm2;
    tmpNorm3 = tmpNorm3 >= tmpNorm4 ? tmpNorm3 : tmpNorm4;
    return (tmpNorm1 > tmpNorm3 ? tmpNorm1 : tmpNorm3);
}

/// Dot product of with v, unroll long for loops.
DBL VEC::Dot(const VEC& v) const
{
    INT i;
    DBL dot1 = 0.0, dot2 = 0.0, dot3 = 0.0, dot4 = 0.0;
    const INT len = this->size - this->size % 4;
    for ( i = 0; i < len; i += 4 ) {
        dot1 += this->values[i]     * v.values[i];
        dot2 += this->values[i + 1] * v.values[i + 1];
        dot3 += this->values[i + 2] * v.values[i + 2];
        dot4 += this->values[i + 3] * v.values[i + 3];
    }
    for ( i = len; i < this->size; ++i ) dot1 += this->values[i] * v.values[i];
    return (dot1 + dot2 + dot3 + dot4);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/