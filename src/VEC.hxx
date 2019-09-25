/*! \file vec.hxx
 *  \brief Header file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __vecHEADER__      /*-- allow multiple inclusions --*/
#define __vecHEADER__      /**< indicate vec.hxx has been included before */

#include <vector>
#include "RetCode.hxx"

/*! \class VEC
 *  \brief The VEC data structure and basic operations
 *  \author Kailei Zhang
 *  \date 09/01/2019
 */
class VEC {

private:
    //! vec is a column vector of DBL type logically
    std::vector<DBL> vec;

    //! size of vec
    INT size;

public:
    //! default constructed function
    VEC() : size(0), vec(0) {}

    //! set the size of vec and set the same value on VEC object
    VEC(const INT& size, const DBL& value);

    //! assign a vector object to VEC object
    VEC(const std::vector<DBL>& vt);

    //! assign a VEC object to VEC object
    VEC(const VEC& v);

    //! assign an array to VEC object
    VEC(const INT& size, const DBL *ptr);

    //! destructor function
    ~VEC() {}

    //! overload equals operator
    VEC& operator=(const VEC& v);

    //! overload [] operator
    DBL& operator[](const INT& position);

    //! overload [] operator
    const DBL& operator[](const INT& position) const;

    //! overload += operator
    VEC& operator+=(const VEC& v);

    //! overload -= operator
    VEC& operator-=(const VEC& v);

    //! set the size of VEC object
    void SetSize(const INT& size);

    //! set the size of VEC object and set the same value on VEC object
    void SetValues(const INT& size, const DBL& value);

    //! assign vector object to VEC object
    void SetValues(const std::vector<DBL>&vect);

    //! *this = array
    void SetValues(const INT& size, const DBL *array);

    //! get the value of (*this)[position]
    DBL GetValue(const INT& position) const;

    //! get array = (*this) of size = min(size, this->GetSize())
    void GetArray(const INT& size, DBL **array) const;

    //! get the size of *this
    INT GetSize() const;

    //! get the capacity of *this
    INT GetCapacity() const;

    //! scale (*this)[j] = a * (*this)[j] by a scalar
    void Scale(const DBL& a);

    //! copy *this to vec
    void CopyTo(VEC& vec) const;

    //! find max(*this)
    DBL Max() const;

    //! find min(*this)
    DBL Min() const;

    //! shift (*this)[j] += a by a scalar
    void Shift(const DBL& a);

    //! compute abs(*this)
    void Abs();

    //! (*this)[j] = 1 / (*this)[j]
    void Reciprocal();

    //! find l2-norm of *this
    DBL Norm2() const;

    //! find infinity norm of *this
    DBL NormInf() const;

    //! *this = a * vec + b * *this
    void Add(const VEC& v, const DBL& a, const DBL& b);

    //! *this = a * vec1 + b * vec2
    void Add(const DBL& a, const VEC& v1, const DBL& b, const VEC& v2);

    //! dot product of *this and vec
    DBL Dot(const VEC& v) const;

    //! scale (*this)[j] *= v[j] by a vector
    void PointwiseMult(const VEC& v);

    //! (*this)[j] = (*this)[j] / v[j]
    void PointwiseDivide(const VEC& v);
};

#endif /* end if for __vecHEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/