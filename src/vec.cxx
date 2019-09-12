/*! \file vec.cxx
 *  \brief Source file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "vec.hxx"
#include <cstdlib>
#include <cmath>
#include <iostream>

/*
 * 1 marks the mismatch of vectors' lengths
 */

//-----------------------------------------------------------------------------------

void VEC::WarningPrint(const INT warningtype) const {
    switch (warningtype) {
        case 1:
            std::cout << "----------------***************----------------"
                      << std::endl;
            std::cout << "Warning : the parameter 'size' < 0" << std::endl;
            std::cout << "and VEC object 's size is automatically set to 0"
                      << std::endl;
            std::cout << "----------------***************-----------------"
                      << std::endl;
            break;
        case 2:
            std::cout << "----------------***************----------------"
                      << std::endl;
            std::cout << "Warning : the parameter 'N' < 0" << std::endl;
            std::cout << "and VEC object 's size is automatically set to 0"
                      << std::endl;
            std::cout << "----------------***************-----------------"
                      << std::endl;
            break;
        case 3:
            std::cout << "----------------***************----------------"
                      << std::endl;
            std::cout << "Warning : the parameter 'pointer' == nullptr or"
                      << std::endl;
            std::cout << " the parameter 'size' <= 0, and then VEC object "
                      << std::endl;
            std::cout << "'s size is automatically set to 0" << std::endl;
            std::cout << "----------------***************-----------------"
                      << std::endl;
            break;
        case 4:
            std::cout<<"-----------------*****************-----------------"<<std
            ::endl;
            std::cout<<"'position' is more than the dimension of VEC object"<<std
            ::endl;
            std::cout<<"-----------------*****************-----------------"<<std
            ::endl;
            break;
        case 5:
            std::cout << "----------***********----------" << std::endl;
            std::cout << "the VEC object is empty, and set " << std::endl;
            std::cout << "its maximum as zero" << std::endl;
            std::cout << "----------***********----------" << std::endl;
            break;
        case 6:
            std::cout << "----------***********----------" << std::endl;
            std::cout << "the VEC object is empty, so set " << std::endl;
            std::cout << "its minimum as zero" << std::endl;
            std::cout << "----------***********----------" << std::endl;
            break;
        case 7:
            std::cout << "----------**********----------" << std::endl;
            std::cout << "  Warning : empty VEC object  " << std::endl;
            std::cout << "----------**********----------" << std::endl;
            break;
        case 8:
            std::cout << "------------------****************------------------"
                      << std::endl;
            std::cout << "the size of VEC object is zero and set 'dot' as zero"
                      << std::endl;
            std::cout << "------------------****************-------------------"
                      << std::endl;
            break;
        case 9:
            std::cout << "-----------**********---------" << std::endl;
            std::cout << " Zero division error occurred " << std::endl;
            std::cout << "-----------**********---------" << std::endl;
            break;
        default:
            break;
    }
}

/**
 * set VEC object 's length to "size" and all its elements ' values is "value"
 * if the input parameter "size" <= 0, then let this->size=0, and this->vec.resize(0);
 * if the input parameter "size" > 0, the let this->size=size, and all it's elements
 * is the same as "value"
 */
//! set the size of vec and set the same value on VEC object
VEC::VEC(const INT size, const DBL value) {
    if(size<0){
        this->size=0;
        this->vec.resize(0);
        WarningPrint(1);
        return;
    }

    this->vec.assign(size, value);
    this->size = size;
}

/**
 * no matter what the input parameter "vector_obj" is, set its values on
 * this->vec and its size on this->size
 */
//! assign vector object to VEC object
VEC::VEC(std::vector<DBL> vector_obj) {
    this->vec.operator=(vector_obj);
    this->size = vector_obj.size();
}

/**
 * no matter what the input parameter "vec_obj" is, set its values on
 * this->vec and its size on this->size
 */
//! assign VEC object to VEC object
VEC::VEC(const VEC &vec_obj) {
    this->vec.operator=(vec_obj.vec);
    this->size = vec_obj.size;
}

/**
 * if the input parameter "N" <= 0, let this->values.size() = 0, and
 * size = 0, other cases are normally dealt
 */
//! random construction function, N DBL values from begin_value to end_value
VEC::VEC(const INT N,const DBL min, const DBL max) {
    if(N<0){
        this->vec.resize(0);
        this->size=0;
        WarningPrint(2);
        return ;
    }
    this->vec.resize(N); // performance burdens
    srand((unsigned) time(NULL));

    for (INT j = 0; j < N; j++)
        this->vec[j] = rand() / (DBL) RAND_MAX * (max-min) + min;

    this->size = N;
}

/**
 * if the input parameter "pointer" is null or the input parameter
 * "size" is not more than 0, the VEC object is set as null
 */
//! assign pointer to VEC object
VEC::VEC(const INT size, const DBL *pointer) {
    if(pointer== nullptr || size <=0){
        this->vec.resize(0);
        this->size=0;
        WarningPrint(3);
        return;
    }
    this->vec.assign(pointer, pointer + size);
    this->size = size;
}

/**
 * no matter what the input parameter is, set "vec_obj" to
 * *this
 */
//! overload equals operator
VEC &VEC::operator=(const VEC &vec_obj) {
    this->vec.operator=(vec_obj.vec);
    this->size = vec_obj.size;
    return *this;
}

/**
 * attention : the overloaded function doesn't check whether
 * the index crossover
 */
//! overload [] operator
DBL &VEC::operator[](const INT position){
    return this->vec[position];
}

/**
 * attention : the overloaded function doesn't check whether
 * the index crossover
 */
//! overload [] operator
const DBL& VEC::operator[](const INT position) const{
    return this->vec[position];
}

/**
 * attention : the overloaded function doesn't check whether
 * the sizes of both VEC objects match or their sizes both aren't
 * zero
 */
//! overload += operator
VEC &VEC::operator+=(const VEC &vec_obj) {
    for (INT j = 0; j < this->size; j++)
        this->vec[j] += vec_obj.vec[j];

    return *this;
}

/**
 * attention : the overloaded function doesn't check whether
 * the sizes of both VEC objects match or their sizes both aren't
 * zero
 */
//! overload -= operator
VEC &VEC::operator-=(const VEC &vec_obj) {
    for (INT j = 0; j < this->size; j++)
        this->vec[j] -= vec_obj.vec[j];

    return *this;
}

/**
 * if the input parameter "size" < 0, and set the size of VEC object to zero
 */
//! set the size of VEC object
void VEC::SetSize(const INT size) {
    if (size < 0) {
        this->vec.resize(0);
        this->size = 0;
        WarningPrint(1);
        return;
    }
    this->size = size;
    this->vec.resize(size);
}

/**
 * if the input parameter "size" < 0, then set the VEC object 's size to zero
 */
//! set the size of VEC object and set the same value on VEC object
void VEC::SetValues(const INT size, const DBL value) {
    if (size < 0) {
        this->size = 0;
        this->vec.resize(0);
        WarningPrint(1);
        return;
    }
    this->vec.assign(size, value);
    this->size = size;
}

/**
 * no matter what the "vector_object" is, set it on the VEC object and its size to
 * this->size
 */
//! assign vector object to VEC object
void VEC::SetValues(const std::vector<DBL> vector_object) {
    this->vec.operator=(vector_object);
    this->size = vector_object.size();
}

/**
 * if the input parameter "N" <= 0, let this->values.size() = 0, and
 * size = 0, other cases are normally dealt
 */
//! random construction function, N DBL values from begin_value to end_value
void VEC::SetValues(const INT N, const DBL min, const DBL max) {
    if(N<0){
        this->vec.resize(0);
        this->size=0;
        WarningPrint(2);
        return ;
    }
    this->vec.resize(N); // performance burdens
    srand((unsigned) time(NULL));
    for (INT j = 0; j < N; j++)
        this->vec[j] = rand() / (DBL) RAND_MAX * (max-min) + min;

    this->size = N;
}

/**
 * if the input parameter "size" is 0, or "array" is null,
 * then set the this->size = 0, and this->vec.size()=0
 */
//! this->vec = array
void VEC::SetValues(const INT size, const DBL *array) {
    if (array == nullptr || size == 0) {
        this->size = 0;
        this->vec.resize(0);
        return;
    }
    this->vec.assign(array, array + size);
    this->size = size;
}

/**
 * "postion < size" avoids the error in the VEC object' size;
 * and the "this->vec.at()" avoids the error in the vector object
 */
//! get the value of this->vec[position]
DBL VEC::Get(const INT position) const {
    if (position<size) {
        return this->vec.at(position);
    }
    else {
        WarningPrint(4);
    }
}

/**
 * if the input parameter "size" <= 0 or this->size == 0,
 * set the input parameter "array" to null. other cases are
 * normally dealt
 */
//! get array = this->vec of size = min(size, this->size)
void VEC::GetArray(const INT &size, DBL **array) const {
    if (size <= 0 || this->size == 0) {
        *array = nullptr;
        return;
    }

    INT len = size > this->size ? this->size : size;
    *array = new DBL[len];
    for (INT j = 0; j < len; j++)
        (*array)[j] = this->vec[j];
}

/**
 * get the VEC object 's size
 */
//! get the size of this->vec
INT VEC::GetSize() const {
    return this->size;
}

/**
 * get the size of allocated memory for the VEC object
 */
//! get the capacity of VEC object
INT VEC::GetCapacity() {
    return this->vec.capacity();
}

/**
 * if VEC object 's size == 0, return the inital VEC object
 */
//! scale this->vec[j] = a * this->vec[j] by a scalar
void VEC::Scale(DBL a) {
    if (this->size == 0)
        return;
    else {
        for (INT j = 0; j < this->size; j++)
            this->vec[j] = a * this->vec[j];
    }
}

/**
 * no matter what the *this is, copy it into the vec_obj
 */
//! copy vec_obj = this->vec
void VEC::Copy(VEC &vec_obj) const {
    vec_obj.vec.operator=(this->vec);
    vec_obj.size = this->size;
}

/**
 * if the VEC object doesn't contain any elements,
 * set the maximum value to zero
 */
//! find max(this->vec[j])
void VEC::Max(DBL &max) const {
    switch(this->size){
        case 0:
            max=0;
            WarningPrint(5);
            break;
        case 1:
            max=this->vec[0];
            break;
        default:
            max=this->vec[0];
            for(INT j=1;j<this->size;j++){
                if(max<this->vec[j])
                    max=this->vec[j];
            }
            break;
    }
}

/**
 * if the VEC object doesn't contain any elements,
 * set the minimum value to zero
 */
//! find min(this->vec[j])
void VEC::Min(DBL &min) const {
    switch(this->size){
        case 0:
            min=0;
            WarningPrint(6);
            break;
        case 1:
            min=this->vec[0];
            break;
        default:
            min=this->vec[0];
            for(INT j=1;j<this->size;j++){
                if(min>this->vec[j])
                    min=this->vec[j];
            }
            break;
    }
}

/**
 * if the VEC object doesn't contain any elements,
 * No action on the VEC object!
 */
//! shift this->vec[j] += s by a scalar
void VEC::Shift(DBL s) {
    if (this->size == 0)
        this->vec.resize(0);

    for (INT j = 0; j < this->size; j++)
        this->vec[j] += s;
}

/**
 * if the VEC object doesn't contain any elements,
 * No action on the VEC object!
 */
//! compute abs(this->vec[j])
void VEC::Abs() {
    if (this->size == 0) {
        this->vec.resize(0);
        return;
    }

    for (INT j = 0; j < this->size; j++)
        this->vec[j] = fabs(this->vec[j]);
}

/**
 * if No elements is contained in the VEC object, there is no action
 * on it. and the input parameter "tol" is a tolerance factor, if the
 * absolute value of a number is less than "tol", we will think it
 * occurs zero division error.
 */
//! this->vec[j] = 1 / this->vec[j]
void VEC::Reciprocal(DBL tol) {
    if (this->size == 0) {
        this->vec.resize(0);
        return;
    }

    for (INT j = 0; j < this->size; j++) {
        if (fabs(this->vec[j]) <= tol) {
            WarningPrint(9);
        }
        return;
    }

    for (INT j = 0; j < this->size; j++)
        this->vec[j] = 1 / this->vec[j];
}

/**
 * if the VEC object doesn't contain any elements, set l2-norm to
 * zero, other cases is normally dealt.
 */
//! find l2-norm of this->vec
DBL VEC::Norm2() const {
    if (this->size == 0){
        WarningPrint(7);
        return 0.0;
    }

    DBL norm2 = 0.0;
    for (INT j = 0; j < this->size; j++)
        norm2 += this->vec[j] * this->vec[j];

    return sqrt(norm2);
}

/**
 * if the VEC object doesn't contain any elements, set the infinity
 * norm to zero, other cases is normally dealt
 */
//! find infinity norm of this->vec
DBL VEC::NormInf() const {
    if (this->size == 0){
        WarningPrint(7);
        return 0.0;
    }

    DBL normInf = 0.0;
    if (this->size == 1)
        normInf = fabs(this->vec[0]);
    else {
        for (INT j = 1; j < this->size; j++) {
            if (normInf < fabs(this->vec[j]))
                normInf = fabs(this->vec[j]);
        }
    }

    return normInf;
}

/**
 * there is only a possible error that 1 marks the mismatch of
 * vectors' lengths
 */
//! this->vec = a * this->vec + b * vec_obj
FaspErrorType VEC::Add(const VEC &vec_obj, const DBL a, const DBL b) {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        return 0;

    for (INT j = 0; j < this->size; j++)
        this->vec[j] = a * this->vec[j] + b * vec_obj.vec[j];

    return 0;
}

/**
 * there is only a possible error that 1 marks the mismatch of vectors'
 * lengths
 */
//! this->vec = a * vec1_obj + b * vec2_obj
FaspErrorType VEC::Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj) {
    if (vec1_obj.size != vec2_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    this->size = vec1_obj.size;
    this->vec.resize(this->size);
    for (INT j = 0; j < vec1_obj.size; j++)
        this->vec[j] = a * vec1_obj.vec[j] + b * vec2_obj.vec[j];

    return 0;
}

/**
 * if the input parameter "vec_obj" 's size is not equal to this->size,
 * 1 marks the mismatch of vectors lengths.
 * if the *this 's size is equal to zero, we define the input paramter dot
 * as zero, and output the warning message.
 */
//! dot product of this->vec and vec_obj
FaspErrorType VEC::Dot(const VEC &vec_obj, DBL &dot) const {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0){
        WarningPrint(8);
        dot=0.0;
        return 0;
    }

    dot = 0.0;
    for (INT j = 0; j < this->size; j++)
        dot += this->vec[j] * vec_obj.vec[j];

    return 0;
}

/**
 * there is only a possible error that 1 marks the mismatch of
 * vectors' lengths
 */
//! scale this->vec[j] *= vec_obj[j] by a vector
FaspErrorType VEC::PointwiseMult(const VEC &vec_obj) {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        return 0;

    for (INT j = 0; j < this->size; j++)
        this->vec[j] *= vec_obj.vec[j];

    return 0;
}

/**
 * if the input parameter "vec_obj" 's size is not equal to this->size,
 * 1 marks the mismatch of vectors lengths.
 * if the *this 's size is zero, we don't perform any operations on *this.
 * and the input parameter "tol" is a tolerance factor, if the absolute value
 * of a element in the input parameter "vec_obj" is less than "tol", then exit
 * the function and give warning message.
 */
//! this->vec[j] = this->vec[j] / vec_obj[j]
FaspErrorType VEC::PointwiseDivide(const VEC &vec_obj, DBL tol) {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        return 0;

    for (INT j = 0; j < this->size; j++) {
        if (fabs(vec_obj.vec[j]) <= tol) {
            WarningPrint(9);
            return 0;
        }
    }

    for (INT j = 0; j < this->size; j++)
        this->vec[j] /= vec_obj.vec[j];

    return 0;
}