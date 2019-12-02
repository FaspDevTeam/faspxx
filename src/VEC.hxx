/*! \file    VEC.hxx
 *  \brief   Vector class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    09/01/2019
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
 *  \brief General vector class.
 */
class VEC {

private:
    std::vector<DBL> values;  ///< Actual values of vector in DBL.
    INT size;                 ///< Book-keeping size of VEC. NOT values.size!

public:
    friend class MAT;

    /// Default constructor.
    explicit VEC() : values(0), size(0) {}

    /// Construct a new VEC with the given size and a constant value.
    explicit VEC(const INT& size, const DBL& value = 0.0);

    /// Construct a new VEC by copying values from a vector.
    explicit VEC(const std::vector<DBL>& src);

    /// Clone from another VEC.
    VEC(const VEC& src);

    /// Construct a new VEC by copying values from a pointer.
    explicit VEC(const INT& size, const DBL *src);

    /// Default destructor.
    ~VEC() = default;

    /// Overload the = operator.
    VEC& operator=(const VEC& v);

    /// Overload the [] operator.
    DBL& operator[](const INT& position);

    /// Overload the [] operator, entries cannot be modified.
    const DBL& operator[](const INT& position) const;

    /// Overload += operator.
    VEC& operator+=(const VEC& v);

    /// Overload -= operator.
    VEC& operator-=(const VEC& v);

    /// Set the size of VEC object and reserve memory.
    void Reserve(const INT& size);

    /// Assign the size and the same value to a VEC object.
    void SetValues(const INT& size, const DBL& value = 0.0);

    /// Assign a vector object to a VEC object.
    void SetValues(const std::vector<DBL>& src);

    /// Assign values of a DBL array to a VEC object.
    void SetValues(const INT& size, const DBL *array);

    /// Get the value of (*this)[position].
    DBL GetValue(const INT& position) const;

    /// Get value of this->values[index[j] and save it in array[j].
    /// \note Users should allocate memory for array before calling this function!
    void GetValues(const INT& size, const INT *index, DBL *array) const;

    /// Get pointer to this->values.
    void GetArray(DBL **array);

    /// Get pointer to this->values, entries cannot be modified.
    void GetArray(const DBL **array) const;

    /// Get the size of *this.
    INT GetSize() const;

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

    /// x = x + a * y.
    void AXPY(const DBL& a, const VEC& y);

    /// x = y + a * x.
    void XPAY(const DBL& a, const VEC& y);

    /// x = a * x + b * y.
    void AXPBY(const DBL& a, const DBL& b, const VEC& y);

    /// *this = a * v1 + b * v2.
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

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/