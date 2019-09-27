/*! \file VEC.hxx
 *  \brief Header file for the FASP++ VEC class
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
    std::vector<DBL> vec;  ///< DBL vector for storing values of VEC
    INT size;              ///< Book-keeping size of VEC

public:
    /// Default constructor
    VEC() : vec(0), size(0) {}

    /// Assign the size and the same value to a VEC object
    VEC(const INT& size, const DBL& value);

    /// Assign a vector object to a VEC object
    VEC(const std::vector<DBL>& vt);

    /// Assign a VEC object to a VEC object
    VEC(const VEC& v);

    /// Assign a DBL array to a VEC object
    VEC(const INT& size, const DBL *ptr);

    /// Default destructor
    ~VEC() {}

    /// Overload = operator
    VEC& operator=(const VEC& v);

    /// Overload [] operator, entries can be modified
    DBL& operator[](const INT& position);

    /// Overload [] operator, entries cannot be modified
    const DBL& operator[](const INT& position) const;

    /// Overload += operator
    VEC& operator+=(const VEC& v);

    /// Overload -= operator
    VEC& operator-=(const VEC& v);

    /// Set the size of VEC object and reserve memory
    void SetSize(const INT& size);

    /// Assign the size and the same value to a VEC object
    void SetValues(const INT& size, const DBL& value);

    /// Assign a vector object to a VEC object
    void SetValues(const std::vector<DBL>& vt);

    /// Assign a DBL array to a VEC object, user should allocate memory for array
    void SetValues(const INT& size, const DBL *array);

    /// Get the value of (*this)[position]
    DBL GetValue(const INT& position) const; // Todo: get multiple entries

    /// Get array = (*this) of size = min(size, this->GetSize())
    void GetArray(const INT& size, DBL **array) const;

    /// Get the size of *this
    INT GetSize() const;

    /// Get the capacity of *this
    INT GetCapacity() const;

    /// Scale (*this)[j] = a * (*this)[j] by a scalar
    void Scale(const DBL& a);

    /// Scale by a vector (*this)[j] *= v[j]
    void PointwiseMult(const VEC& v);

    /// Compute (*this)[j] = 1 / (*this)[j]
    void Reciprocal();

    /// Inverse scale by a nonzero vector (*this)[j] = (*this)[j] / v[j]
    void PointwiseDivide(const VEC& v);

    /// Copy *this to v
    void CopyTo(VEC& v) const;

    /// Shift (*this)[j] += a by a scalar
    void Shift(const DBL& a);

    /// Compute *this = abs(*this)
    void Abs();

    /// *this = a * vec + b * *this
    void Add(const VEC& v, const DBL& a, const DBL& b);

    /// *this = a * vec1 + b * vec2
    void Add(const DBL& a, const VEC& v1, const DBL& b, const VEC& v2);

    /// Find max(*this)
    DBL Max() const;

    /// Find min(*this)
    DBL Min() const;

    /// Compute Euclidean norm of *this
    DBL Norm2() const;

    /// Compute Infinity norm of *this
    DBL NormInf() const;

    /// Dot product of *this and vec
    DBL Dot(const VEC& v) const;
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/