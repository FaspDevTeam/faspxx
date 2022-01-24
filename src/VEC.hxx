/*! \file    VEC.hxx
 *  \brief   Vector class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Sep/01/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __VEC_HEADER__ /*-- allow multiple inclusions --*/
#define __VEC_HEADER__ /**< indicate VEC.hxx has been included before */

// Standard header files
#include <vector>

// FASPXX header files
#include "Faspxx.hxx"
#include "RetCode.hxx"

/*! \class VEC
 *  \brief General vector class.
 */
class VEC
{

private:
    USI              size;   ///< Book-keeping size of VEC. NOT values.size!
    std::vector<DBL> values; ///< Actual values of vector in DBL.

public:
    friend class MAT;

    /// Default constructor.
    explicit VEC()
        : size(0)
    {
    }

    /// Construct a VEC with the given size and a constant value.
    explicit VEC(const USI& size, const DBL& value = 0.0);

    /// Construct a VEC by copying values from a vector.
    explicit VEC(const std::vector<DBL>& src);

    /// Construct a new VEC by copying values from a pointer.
    explicit VEC(const USI& size, const DBL* src);

    /// Clone from another VEC.
    VEC(const VEC& src);

    /// Default destructor.
    ~VEC() = default;

    /// Overload the = operator.
    VEC& operator=(const VEC& v);

    /// Overload the [] operator.
    DBL& operator[](const USI& position);

    /// Overload the [] operator, entries cannot be modified.
    const DBL& operator[](const USI& position) const;

    /// Overload += operator.
    VEC& operator+=(const VEC& v);

    /// Overload -= operator.
    VEC& operator-=(const VEC& v);

    /// Set the size of VEC object and reserve memory.
    void Reserve(const USI& size);

    /// Assign the size and the same value to a VEC object.
    void SetValues(const USI& size, const DBL& value = 0.0);

    /// Assign a vector object to a VEC object.
    void SetValues(const std::vector<DBL>& src);

    /// Assign values of a DBL array to a VEC object.
    void SetValues(const USI& size, const DBL* array);

    /// Get the value of (*this)[position].
    DBL GetValue(const USI& position) const;

    /// Get value of this->values[index[j] and save it in array[j].
    /// \note Users should allocate memory for array before calling this function!
    void GetValues(const USI& size, const USI* index, DBL* array) const;

    /// Get pointer to this->values.
    void GetArray(DBL** array);

    /// Get pointer to this->values, entries cannot be modified.
    void GetArray(const DBL** array) const;

    /// Get the size of *this.
    USI GetSize() const;

    /// Scale by a scalar.
    void Scale(const DBL& a);

    /// Compute reciprocal pointwise.
    void Reciprocal();

    /// Scale by a vector pointwise.
    void PointwiseMult(const VEC& v);

    /// Divide pointwise by a nonzero vector.
    void PointwiseDivide(const VEC& v);

    /// Copy *this to another VEC.
    void CopyTo(VEC& dst) const;

    /// Shift by a scalar pointwise.
    void Shift(const DBL& a);

    /// Compute absolute values pointwise.
    void Abs();

    /// *this += a * x.
    void AXPY(const DBL& a, const VEC& x);

    /// *this = x + a * *this.
    void XPAY(const DBL& a, const VEC& x);

    /// *this = a * *this + b * y.
    void AXPBY(const DBL& a, const DBL& b, const VEC& y);

    /// *this = a * x + b * *this.
    void WAXPBY(const DBL& a, const VEC& x, const DBL& b, const VEC& y);

    /// Find maximal value.
    DBL Max() const;

    /// Find minimal value.
    DBL Min() const;

    /// Compute Euclidean norm.
    DBL Norm2() const;

    /// Compute infinity norm.
    DBL NormInf() const;

    /// Dot product of with v.
    DBL Dot(const VEC& v) const;
};

#endif /* end if for __VEC_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Sep/01/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/