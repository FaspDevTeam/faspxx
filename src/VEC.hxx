/*! \file VEC.hxx
 *  \brief Vector class declaration
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __VEC_HEADER__      /*-- allow multiple inclusions --*/
#define __VEC_HEADER__      /**< indicate VEC.hxx has been included before */

#include <vector>
#include "faspxx.hxx"
#include "RetCode.hxx"

/*! \class VEC
 *  \brief The VEC data structure and basic operations
 *  \author Kailei Zhang
 *  \date 09/01/2019
 */
class VEC {

private:
    std::vector<DBL> values;  ///< Actual values of vector in DBL
    INT size;                 ///< Book-keeping size of VEC: It's NOT values.size!

public:
    friend class MAT;

    /// Default constructor
    explicit VEC() : values(0), size(0) {}

    /// Assign the size and the same value to a VEC object
    explicit VEC(const INT &size, const DBL &value = 0.0);

    /// Assign a vector object to a VEC object
    explicit VEC(const std::vector<DBL> &vt);

    /// Assign a const VEC object to a VEC object
    VEC(const VEC &v);

    /// Assign a DBL array to a VEC object
    explicit VEC(const INT &size, const DBL *ptr);

    /// Default destructor
    ~VEC() = default;

    /// Overload = operator
    VEC &operator=(const VEC &v);

    /// Overload [] operator, entries can be modified
    DBL &operator[](const INT &position);

    /// Overload [] operator, entries cannot be modified
    const DBL &operator[](const INT &position) const;

    /// Overload += operator
    VEC &operator+=(const VEC &v);

    /// Overload -= operator
    VEC &operator-=(const VEC &v);

    /// Set the size of VEC object and reserve memory
    void Reserve(const INT &size);

    /// Shrink *this 's memory to this->size
    void ShrinkToSize();

    /// Assign the size and the same value to a VEC object
    void SetValues(const INT &size, const DBL &value = 0.0);

    /// Assign a vector object to a VEC object
    void SetValues(const std::vector<DBL> &vt);

    /// Assign a DBL array to a VEC object, user should allocate memory for array
    void SetValues(const INT &size, const DBL *array);

    /// Get the value of (*this)[position]
    DBL GetValue(const INT &position) const;

    /// Get "array" = the set of { this->values[indexPtr[j] mod this->size ] } and
    // "size" is the size of "indexPtr" and "array"
    void GetValues(const INT &size, const INT *indexPtr, DBL *array) const;

    /// pointer points this->values
    void GetArray(DBL **array);

    /// pointer points this->values
    void GetArray(const DBL **array) const;

    /// Get the size of *this
    INT GetSize() const;

    /// Get the capacity of *this
    INT GetCapacity() const;

    /// Scale (*this)[j] = a * (*this)[j] by a scalar
    void Scale(const DBL &a);

    /// Scale by a vector (*this)[j] *= v[j]
    void PointwiseMult(const VEC &v);

    /// Compute (*this)[j] = 1 / (*this)[j]
    void Reciprocal();

    /// Inverse scale by a nonzero vector (*this)[j] = (*this)[j] / v[j]
    void PointwiseDivide(const VEC &v);

    /// Copy *this to v
    void CopyTo(VEC &v) const;

    /// Shift (*this)[j] += a by a scalar
    void Shift(const DBL &a);

    /// Compute *this = abs(*this)
    void Abs();

    /// y = y + a * x
    void AXPY(const DBL &a, const VEC &x);

    /// y = x + a * y
    void XPAY(const DBL &a, const VEC &x);

    /// *this = a * *this + b * v
    void AXPBY(const DBL &a, const DBL &b, const VEC &v);

    /// *this = a * v1 + b * v2
    void WAXPBY(const DBL &a, const VEC &v1, const DBL &b, const VEC &v2);

    /// Find max(*this)
    DBL Max() const;

    /// Find min(*this)
    DBL Min() const;

    /// Compute Euclidean norm of *this
    DBL Norm2() const;

    /// Compute Infinity norm of *this
    DBL NormInf() const;

    /// Dot product of with v
    DBL Dot(const VEC &v) const;
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/