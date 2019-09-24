/*! \file vec.hxx
 *  \brief Header file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __VEC_HEADER__      /*-- allow multiple inclusions --*/
#define __VEC_HEADER__      /**< indicate vec.hxx has been included before */

#include <vector>
#include "error.hxx"

/*! \class VEC
 *  \brief basic vec class
 *  \author Kailei Zhang
 *  \date 09/01/2019
 *
 *  This class defines the basic VEC data structure and operations
 */
class VEC {

private:
    //! vec_ is a column vector of DBL type logically
    std::vector<DBL> vec_;

    //! size of vec
    INT size_;

public:
    //! default constructed function
    VEC() : size_(0), vec_(0) {}

    //! set the size of vec_ and set the same value on VEC object
    VEC(const INT size, const DBL value);

    //! assign vector object to VEC object
    VEC(const std::vector<DBL> vect);

    //! assign VEC object to VEC object
    VEC(const VEC &vec);

    //! assign pointer to VEC object
    VEC(const INT size, const DBL *ptr);

    //! destructor function
    ~VEC() {}

    //! overload equals operator
    VEC &operator=(const VEC &vec);

    //! overload [] operator
    DBL &operator[](const INT position);

    //! overload [] operator
    const DBL &operator[](const INT position) const;

    //! overload += operator
    VEC &operator+=(const VEC &vec);

    //! overload -= operator
    VEC &operator-=(const VEC &vec);

    //! set the size of VEC object
    void Reserve(const INT size);

    //! set the size of VEC object and set the same value on VEC object
    void SetValues(const INT size, const DBL value);

    //! assign vector object to VEC object
    void SetValues(const std::vector<DBL> vect);

    //! *this = array
    void SetValues(const INT size, const DBL *array);

    //! get the value of (*this)[position]
    DBL Get(const INT position) const;

    //! get array = (*this) of size = min(size, this->GetSize())
    void GetArray(const INT size, DBL **array) const;

    //! get the size of *this
    INT GetSize() const;

    //! get the capacity of *this
    INT GetCapacity();

    //! scale (*this)[j] = a * (*this)[j] by a scalar
    void Scale(const DBL a);

    //! copy *this to vec
    void CopyTo(VEC &vec) const;

    //! find max(*this)
    DBL Max() const;

    //! find min(*this)
    DBL Min() const;

    //! shift (*this)[j] += s by a scalar
    void Shift(DBL s);

    //! compute abs(*this)
    void Abs();

    //! (*this)[j] = 1 / (*this)[j]
    void Reciprocal(DBL tol);

    //! find l2-norm of *this
    DBL Norm2() const;

    //! find infinity norm of *this
    DBL NormInf() const;

    //! *this = a * vec + b * *this
    void Add(const VEC &vec, const DBL a, const DBL b);

    //! *this = a * vec1 + b * vec2
    void Add(const DBL a, const VEC &vec1, const DBL b, const VEC &vec2);

    //! dot product of *this and vec
    DBL Dot(const VEC &vec) const;

    //! scale (*this)[j] *= vec[j] by a vector
    void PointwiseMult(const VEC &vec);

    //! (*this)[j] = (*this)[j] / vec[j]
    void PointwiseDivide(const VEC &vec);
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/