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

    void WarningPrint(const INT warningtype) const ;

public:
    //! default constructed function
    VEC():size(0), vec(0) {}

    /**
     * set VEC object 's length to "size" and all its elements ' values is "value"
    * if the input parameter "size" <= 0, then let this->size=0, and this->vec.resize(0);
    * if the input parameter "size" > 0, the let this->size=size, and all it's elements
    * is the same as "value"
    */
    //! set the size of vec and set the same value on VEC object
    VEC(const INT size, const DBL value);


    /**
     * no matter what the input parameter "vector_obj" is, set its values on
     * this->vec and its size on this->size
     */
    //! assign vector object to VEC object
    VEC(const std::vector<DBL> vector_obj);

    /**
     * no matter what the input parameter "vec_obj" is, set its values on
     * this->vec and its size on this->size
     */
    //! assign VEC object to VEC object
    VEC(const VEC &vec_obj);

    /**
     * if the input parameter "N" <= 0, let this->values.size() = 0, and
     * size = 0, other cases are normally dealt
     */
    //! random construction function, N DBL values from begin_value to end_value
    VEC(const INT N,const DBL min, const DBL max);

    /**
     * if the input parameter "pointer" is null or the input parameter
     * "size" is not more than 0, the VEC object is set as null
     */
    //! assign pointer to VEC object
    VEC(const INT size, const DBL *pointer);

    //! destructor function
    ~VEC() {}

    /**
     * no matter what the input parameter is, set "vec_obj" to
     * *this
     */
    //! overload equals operator
    VEC &operator = (const VEC &vec_obj);

    /**
     * attention : the overloaded function doesn't check whether
     * the index crossover
     */
    //! overload [] operator
    DBL &operator [] (const INT position);

    /**
     * attention : the overloaded function doesn't check whether
     * the index crossover
     */
     //! overload [] operator
     const DBL &operator [] (const INT position) const;

    /**
     * attention : the overloaded function doesn't check whether
     * the sizes of both VEC objects match or their sizes both aren't
     * zero
     */
    //! overload += operator
    VEC &operator += (const VEC &vec_obj);

    /**
     * attention : the overloaded function doesn't check whether
     * the sizes of both VEC objects match or their sizes both aren't
     * zero
     */
    //! overload -= operator
    VEC &operator -= (const VEC &vec_obj);

    /**
     * if the input parameter "size" < 0, and set the size of VEC object to zero
    */
    //! set the size of VEC object
    void SetSize(const INT size);

    /**
     * if the input parameter "size" < 0, then set the VEC object 's size to zero
    */
    //! set the size of VEC object and set the same value on VEC object
    void SetValues(const INT size, const DBL value);

    /**
     * no matter what the "vector_object" is, set it on the VEC object and its size to
     * this->size
     */
    //! assign vector object to VEC object
    void SetValues(const std::vector<DBL> vector_obj);

    /**
     * if the input parameter "N" <= 0, let this->values.size() = 0, and
     * size = 0, other cases are normally dealt
     */
    //! random construction function, N DBL values from begin_value to end_value
    void SetValues(const INT N,const DBL min, const DBL max);

    /**
     * if the input parameter "size" is 0, or "array" is null,
     * then set the this->size = 0, and this->vec.size()=0
     */
    //! this->vec = array
    void SetValues(const INT size, const DBL *array);

    /**
     * "postion < size" avoids the error in the VEC object' size;
     * and the "this->vec.at()" avoids the error in the vector object
     */
    //! get the value of this->vec[position]
    DBL Get(const INT position) const;

    /**
     * if the input parameter "size" <= 0 or this->size == 0,
     * set the input parameter "array" to null. other cases are
     * normally dealt
    */
    //! get array = this->vec of size = min(size, this->size)
    void GetArray(const INT &size, DBL **array) const;

    /**
     * get the VEC object 's size
     */
    //! get the size of this->vec
    INT GetSize() const;

    /**
     * get the size of allocated memory for the VEC object
     */
    //! get the capacity of VEC object
    INT GetCapacity();

    /**
     * if VEC object 's size == 0, return the inital VEC object
    */
    //! scale this->vec[j] = a * this->vec[j] by a scalar
    void Scale(const DBL a);

    /**
     * no matter what the *this is, copy it into the vec_obj
     */
    //! copy vec_obj = this->vec
    void Copy(VEC &vec_obj) const;

    /**
     * if the VEC object doesn't contain any elements,
     * set the maximum value to zero
     */
    //! find max(this->vec[j])
    void Max(DBL &max) const;

    /**
     * if the VEC object doesn't contain any elements,
     * set the minimum value to zero
     */
    //! find min(this->vec[j])
    void Min(DBL &min) const;

    /**
     * if the VEC object doesn't contain any elements,
     * No action on the VEC object!
     */
    //! shift this->vec[j] += s by a scalar
    void Shift(DBL s);

    /**
     * if the VEC object doesn't contain any elements,
     * No action on the VEC object!
     */
    //! compute abs(this->vec[j])
    void Abs();

    /**
     * if No elements is contained in the VEC object, there is no action
     * on it. and the input parameter "tol" is a tolerance factor, if the
     * absolute value of a number is less than "tol", we will think it
     * occurs zero division error.
     */
    //! this->vec[j] = 1 / this->vec[j]
    void Reciprocal(DBL tol);

    /**
     * if the VEC object doesn't contain any elements, set l2-norm to
     * zero, other cases is normally dealt.
     */
    //! find l2-norm of this->vec
    DBL Norm2() const;

    /**
     * if the VEC object doesn't contain any elements, set the infinity
     * norm to zero, other cases is normally dealt
     */
    //! find infinity norm of this->vec
    DBL NormInf() const;

    /**
     * there is only a possible error that 1 marks the mismatch of
     * vectors' lengths
     */
    //! this->vec = a * vec_obj + b * this->vec
    FaspErrorType Add(const VEC &vec_obj, const DBL a, const DBL b);

    /**
     * there is only a possible error that 1 marks the mismatch of vectors'
     * lengths
     */
    //! this->vec = a * vec1_obj + b * vec2_obj
    FaspErrorType Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj);

    /**
     * if the input parameter "vec_obj" 's size is not equal to this->size,
     * 1 marks the mismatch of vectors lengths.
     * if the *this 's size is equal to zero, we define the input paramter dot
     * as zero, and output the warning message.
     */
    //! dot product of this->vec and vec_obj
    FaspErrorType Dot(const VEC &vec_obj, DBL &dot) const;

    /**
     * there is only a possible error that 1 marks the mismatch of
     * vectors' lengths
     */
    //! scale this->vec[j] *= vec_obj[j] by a vector
    FaspErrorType PointwiseMult(const VEC &vec_obj);

    //! this->vec[j] = this->vec[j] / vec_obj[j]
    FaspErrorType PointwiseDivide(const VEC &vec_obj,DBL tol);
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/