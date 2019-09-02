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
#include <cmath>
#include "errortype.hxx"

/*! \class VEC
 *  \brief basic vec class
 *  \author Kailei Zhang
 *  \date 09/01/2019
 *
 *  This class defines the basic VEC data structure and operations
 */
class VEC {

private:
    //! vec is a column vector of DBL type logically
    std::vector <DBL> vec;

    //! size of vec
    INT size;

public:
    //! default constructed function
    VEC() {}

    //! set the length of vec and set the same value on class object
    VEC(const INT length, const DBL value);

    //! assign vector to class object
    VEC(const std::vector <DBL> vector_object);

    //! assign VEC object to class object
    VEC(const VEC &vec);

    //! random construction function, N DBL values from begin_value to end_value
    VEC(const DBL begin_value, const DBL end_value, const INT N);

    //! random construction function, N INT values from begin_value to end_value
    VEC(const INT begin_value, const INT end_value, const INT N);

    //! assign values given in an array pointer
    VEC(const DBL *pointer, const INT element_size);

    //! destructor function
    ~VEC() {}

    //! overload equals operator
    VEC &operator = (const VEC &vec);

    //! get the value of this->vec[position]
    FaspErrorType Get(const INT position, DBL &value) const;

    //! get array = this->vec of length = min(length, this->size)
    FaspErrorType GetArray(const INT &length, DBL **array) const;

    //! get the size of this->vec
    FaspErrorType GetSize(INT &size) const;

    //! this->vec = a * vec + b * this->vec
    FaspErrorType Add(VEC vec, DBL a, DBL b);

    //! scale this->vec[j] = a * this->vec[j] by a scalar
    FaspErrorType Scale(const DBL a);

    //! scale this->vec[j] *= vec[j] by a vector
    FaspErrorType PointWiseMult(VEC vec);

    //! this->vec[j] = this->vec[j] / vec[j]
    FaspErrorType PointWiseDivide(VEC vec);

    //! shift this->vec[j] += s by a scalar
    FaspErrorType Shift(const DBL s);

    //! compute abs(this->vec[j])
    FaspErrorType Abs();

    //! this->vec[j] = 1 / this->vec[j]
    FaspErrorType Reciprocal();

    //! copy vec = this->vec
    FaspErrorType Copy(VEC &vec) const;

    //! find max(this->vec[j])
    FaspErrorType Max(DBL &max) const;

    //! find min(this->vec[j])
    FaspErrorType Min(DBL &min) const;

    //! find l2-norm of this->vec
    FaspErrorType Norm2(DBL &norm2) const;

    //! find infinity norm of this->vec
    FaspErrorType NormInf(DBL &normInf) const;

    //! dot product of this->vec and vec
    FaspErrorType Dot(VEC vec, DBL &dot) const;
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/