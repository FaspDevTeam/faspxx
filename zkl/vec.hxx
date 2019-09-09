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
    std::vector<DBL> vec;

    //! size of vec
    INT size;

public:
    //! default constructed function
    VEC() {}

    //! set the size of vec and set the same value on VEC object
    VEC(const INT size, const DBL value);

    //! assign vector object to VEC object
    VEC(const std::vector<DBL> vector_obj);

    //! assign VEC object to VEC object
    VEC(const VEC &vec_obj);

    //! random construction function, N DBL values from begin_value to end_value
    VEC(const DBL begin_value, const DBL end_value, const INT N);

    //! assign pointer to VEC object
    VEC(const DBL *pointer, const INT size);

    //! destructor function
    ~VEC() {}

    //! overload equals operator
    VEC &operator = (const VEC &vec_obj);

    //! overload [] operator
    DBL &operator [] (const INT position);

    //! overload += operator
    VEC &operator += (const VEC &vec_obj);

    //! overload -= operator
    VEC &operator -= (const VEC &vec_obj);

    //! set the size of VEC object
    void SetSize(const INT size);

    //! set the size of VEC object and set the same value on VEC object
    void SetValues(const INT size, const DBL value);

    //! assign vector object to VEC object
    void SetValues(const std::vector<DBL> vector_obj);

    //! random construction function, N DBL values from begin_value to end_value
    void SetValues(const DBL begin_value, const DBL end_value, const INT N);

    //! this->vec = array
    void SetValues(const INT size, const DBL *array);

    //! get the value of this->vec[position]
    void Get(const INT position, DBL &value) const;

    //! get array = this->vec of size = min(size, this->size)
    void GetArray(const INT &size, DBL **array) const;

    //! get the size of this->vec
    INT GetSize() const;

    //! get the capacity of VEC object
    INT GetCapacity();

    //! scale this->vec[j] = a * this->vec[j] by a scalar
    void Scale(const DBL a);

    //! copy vec_obj = this->vec
    void Copy(VEC &vec_obj) const;

    //! find max(this->vec[j])
    void Max(DBL &max) const;

    //! find min(this->vec[j])
    void Min(DBL &min) const;

    //! shift this->vec[j] += s by a scalar
    void Shift(DBL s);

    //! compute abs(this->vec[j])
    void Abs();

    //! this->vec[j] = 1 / this->vec[j]
    void Reciprocal(DBL tol);

    //! find l2-norm of this->vec
    DBL Norm2() const;

    //! find infinity norm of this->vec
    DBL NormInf() const;

    //! this->vec = a * vec_obj + b * this->vec
    FaspErrorType Add(const VEC &vec_obj, const DBL a, const DBL b);

    //! this->vec = a * vec1_obj + b * vec2_obj
    FaspErrorType Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj);

    //! dot product of this->vec and vec_obj
    FaspErrorType Dot(const VEC &vec_obj, DBL &dot) const;

    //! scale this->vec[j] *= vec_obj[j] by a vector
    FaspErrorType PointWiseMult(const VEC &vec_obj);

    //! this->vec[j] = this->vec[j] / vec_obj[j]
    FaspErrorType PointWiseDivide(const VEC &vec_obj,DBL tol);
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/