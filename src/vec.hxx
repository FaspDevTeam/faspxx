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
    //! vec is a column vector of DBL type logically
    std::vector<DBL> vec;

    //! size of vec
    INT size;

public:
    //! default constructed function
    VEC() : size(0), vec(0) {}

    /**
     * set VEC object 's length to "size" and all its elements ' values is "value"
     * if the input parameter "size" <= 0, and iniialize it by default constructor;
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
    //VEC(const INT N, const DBL min, const DBL max);

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
    VEC &operator=(const VEC &vec_obj);

    /**
     * attention : the overloaded function doesn't check whether
     * the index crossover
     */
    //! overload [] operator
    DBL &operator[](const INT position);

    /**
     * attention : the overloaded function doesn't check whether
     * the index crossover
     */
    //! overload [] operator
    const DBL &operator[](const INT position) const;

    /**
     * if there is a mismatch between "vec_obj" 's size and *this 's size
     * it throws an exception and return the *this
     */
    //! overload += operator
    VEC &operator+=(const VEC &vec_obj);

    /**
     * if there is a mismatch between "vec_obj" 's size and *this 's size
     * it throws an exception and return the *this
     */
    //! overload -= operator
    VEC &operator-=(const VEC &vec_obj);

    /**
     * if the input parameter "size" < 0, throw an exception.
     * Don't change the *this
     */
    //! set the size of VEC object
    void SetSize(const INT size);

    /**
     * if the input parameter "size" < 0, throw an exception.
     * Don't change the *this
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
     * if the input parameter "size" < 0, throw an exception.
     * Don't change the *this
     */
    //! this->vec = array
    void SetValues(const INT size, const DBL *array);

    /**
     * if "position >= size" or "position < 0" happens , then throw an
     * exception, and return zero
     */
    //! get the value of this->vec[position]
    DBL Get(const INT position) const;

    /**
     * if the input parameter "size" < 0, throw an exception.
     * if size == 0 , set "array" as nullptr, other cases are
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
    //! copy this->vec to vec_obj
    void CopyTo(VEC &vec_obj) const;

    /**
     * if the VEC object doesn't contain any elements,
     * throw an exception and set the maximum value to zero
     */
    //! find max(this->vec[j])
    DBL Max() const;

    /**
     * if the VEC object doesn't contain any elements,
     * throw an exception and set the minimum value to zero
     */
    //! find min(this->vec[j])
    DBL Min() const;

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
     * if the VEC object doesn't contain any elements, throw an exception and set
     * l2-norm to zero, other cases is normally dealt.
     */
    //! find l2-norm of this->vec
    DBL Norm2() const;

    /**
     * if the VEC object doesn't contain any elements, throw an exception and set the
     * infinity norm to zero, other cases is normally dealt
     */
    //! find infinity norm of this->vec
    DBL NormInf() const;

    /**
     * if there is a mismatch between "vec_obj" and *this, throw an exception and end
     * the function
     */
    //! this->vec = a * vec_obj + b * this->vec
    void Add(const VEC &vec_obj, const DBL a, const DBL b);

    /**
     * there is a mismatch between "vec1_obj" and "vec2_obj", throw an exception and
     * end the function
     */
    //! this->vec = a * vec1_obj + b * vec2_obj
    void Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj);

    /**
     * if the input parameter "vec_obj" 's size is not equal to this->size,
     * throw an exception and end the function and set the returned value as zero.
     * if the *this 's size is equal to zero, throw an exception and end the function and
     * set the returned value as zero.
     */
    //! dot product of this->vec and vec_obj
    DBL Dot(const VEC &vec_obj) const;

    /**
     * if "this->size" is not equal to "vec_obj.size" or "this->size" is equal to zero,
     * throw an exception and end the function.
     */
    //! scale this->vec[j] *= vec_obj[j] by a vector
    void PointwiseMult(const VEC &vec_obj);

    /**
     * if "this->size" is not equal to "vec_obj.size" or "this->size" is equal to zero.
     * throw an exception.
     * if there is an element whose absolute value is less than "tol", throw an
     * exception and end the function.
     */
    //! this->vec[j] = this->vec[j] / vec_obj[j]
    void PointwiseDivide(const VEC &vec_obj, DBL tol);
};

#endif /* end if for __VEC_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
