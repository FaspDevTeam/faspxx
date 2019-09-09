/*! \file vec.cxx
 *  \brief Source file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "vec.hxx"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>

/*
 * 1 marks the mismatch of vectors' lengths
 */

//-----------------------------------------------------------------------------------

//! set the size of vec and set the same value on VEC object
VEC::VEC(const INT size, const DBL value) {
    this->vec.assign(size, value);
    this->size = size;
}

//! assign vector object to VEC object
VEC::VEC(std::vector<DBL> vector_obj) {
    this->vec.operator=(vector_obj);
    this->size = vector_obj.size();
}

//! assign VEC object to VEC object
VEC::VEC(const VEC &vec_obj) {
    this->vec.operator=(vec_obj.vec);
    this->size = vec_obj.size;
}

//! random construction function, N DBL values from begin_value to end_value
VEC::VEC(const DBL begin_value, const DBL end_value, const INT N) {
    this->vec.reserve(N); // performance burdens
    srand((unsigned) time(NULL));

    for (int j = 0; j < N; j++)
        this->vec[j] = rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value;

    this->size = N;
}

//! assign pointer to VEC object
VEC::VEC(const DBL *pointer, const INT size) {
    this->vec.assign(pointer, pointer + size);
    this->size = size;
}

//! overload equals operator
VEC &VEC::operator=(const VEC &vec_obj) {
    this->vec.operator=(vec_obj.vec);
    this->size = vec_obj.size;
}

//! overload [] operator
DBL &VEC::operator[](const INT position) {
    return vec[position];
}

//! overload += operator
VEC &VEC::operator+=(const VEC &vec_obj) {
    for (int j = 0; j < size; j++) {
        vec[j] += vec_obj.vec[j];
    }

    return *this;
}

//! overload -= operator
VEC &VEC::operator-=(const VEC &vec_obj) {
    for (int j = 0; j < size; j++) {
        vec[j] -= vec_obj.vec[j];
    }

    return *this;
}

//! set the size of VEC object
void VEC::SetSize(const INT size) {
    if (size < 0) {
        this->size = 0;
        this->vec.resize(0);
    }
    this->size = size;
    vec.resize(size);
}

//! set the size of VEC object and set the same value on VEC object
void VEC::SetValues(const INT size, const DBL value) {
    if (size <= 0) {
        this->size = 0;
        this->vec.resize(0);
    }
    vec.assign(value, size);
    this->size = size;
}

//! assign vector object to VEC object
void VEC::SetValues(const std::vector<DBL> vector_object) {
    this->vec.operator=(vector_object);
    this->size = vector_object.size();
}

//! random construction function, N DBL values from begin_value to end_value
void VEC::SetValues(const DBL begin_value, const DBL end_value, const INT N) {
    this->vec.resize(N); // performance burdens
    srand((unsigned) time(NULL));
    for (int j = 0; j < N; j++)
        this->vec[j] = rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value;

    this->size = N;
}

//! this->vec = array
void VEC::SetValues(const INT size, const DBL *array) {
    if (array == nullptr || size == 0) {
        this->size = 0;
        this->vec.resize(0);
    }
    this->vec.assign(array, array + size);
    this->size = size;
}

//! get the value of this->vec[position]
void VEC::Get(const INT position, DBL &value) const {
    value = vec.at(position);
}

//! get array = this->vec of size = min(size, this->size)
void VEC::GetArray(const INT &size, DBL **array) const {
    if (size == 0 || this->size == 0) {
        *array = nullptr;
    }

    int len = size > this->size ? this->size : size;
    *array = new DBL[len];
    for (int j = 0; j < len; j++)
        (*array)[j] = this->vec[j];
}

//! get the size of this->vec
INT VEC::GetSize() const {
    return this->size;
}

//! scale this->vec[j] = a * this->vec[j] by a scalar
void VEC::Scale(DBL a) {
    if (this->size == 0)
        this->vec.resize(0);
    else {
        for (int j = 0; j < this->size; j++)
            this->vec[j] = a * this->vec[j];
    }
}

//! copy vec_obj = this->vec
void VEC::Copy(VEC &vec_obj) const {
    vec_obj.vec.operator=(this->vec);
    vec_obj.size = this->size;
}

//! find max(this->vec[j])
void VEC::Max(DBL &max) const {
    if (this->size == 0) {
        max = 0;
    }

    if (this->size == 1) {
        max = this->vec[0];
    }

    if (this->size > 1) {
        max = this->vec[0];
        for (int j = 1; j < this->size; j++) {
            if (max < this->vec[j])
                max = this->vec[j];
        }
    }
}

//! find min(this->vec[j])
void VEC::Min(DBL &min) const {
    if (this->size == 0)
        min = 0;

    if (this->size == 1)
        min = this->vec[0];

    if (this->size > 1) {
        min = this->vec[0];
        for (int j = 1; j < this->size; j++) {
            if (min > this->vec[j])
                min = this->vec[j];
        }
    }
}

//! shift this->vec[j] += s by a scalar
void VEC::Shift(DBL s) {
    if (this->size == 0)
        this->vec.resize(0);

    for (int j = 0; j < this->size; j++)
        this->vec[j] += s;
}

//! compute abs(this->vec[j])
void VEC::Abs() {
    if (this->size == 0)
        this->vec.resize(0);

    for (int j = 0; j < this->size; j++)
        this->vec[j] = fabs(this->vec[j]);
}

//! this->vec[j] = 1 / this->vec[j]
void VEC::Reciprocal(DBL tol) {
    if (this->size == 0) {
        this->size = 0;
        this->vec.resize(0);
    }

    for (int j = 0; j < this->size; j++) {
        if (fabs(this->vec[j]) <= tol){
            std::cout << "-----------**********---------" << std::endl;
            std::cout << " Zero division error occurred " << std::endl;
            std::cout << "-----------**********---------" << std::endl;
        }
        return;
    }

    for (int j = 0; j < size; j++)
        vec[j] = 1 / vec[j];

    return;
}

//! find l2-norm of this->vec
DBL VEC::Norm2() const {
    if (size == 0)
        return 0;

    DBL norm2 = 0.0;
    for (int j = 0; j < size; j++)
        norm2 += vec[j] * vec[j];

    return sqrt(norm2);
}

//! find infinity norm of this->vec
DBL VEC::NormInf() const {
    if (size == 0)
        return 0;

    DBL normInf = 0.0;
    if (size == 1)
        normInf = fabs(vec[0]);
    else {
        for (int j = 1; j < size; j++) {
            if (normInf < fabs(vec[j]))
                normInf = fabs(vec[j]);
        }
    }

    return normInf;
}

//! this->vec = a * this->vec + b * vec_obj
FaspErrorType VEC::Add(const VEC &vec_obj, const DBL a, const DBL b) {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        return 0;

    for (int j = 0; j < this->size; j++)
        this->vec[j] = a * this->vec[j] + b * vec_obj.vec[j];

    return 0;
}

//! this->vec = a * vec1_obj + b * vec2_obj
FaspErrorType VEC::Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj) {
    if (vec1_obj.size != vec2_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    this->size = vec1_obj.size;
    this->vec.resize(this->size);
    for (int j = 0; j < vec1_obj.size; j++)
        this->vec[j] = a * vec1_obj.vec[j] + b * vec2_obj.vec[j];

    return 0;
}

//! dot product of this->vec and vec
FaspErrorType VEC::Dot(const VEC &vec_obj, DBL &dot) const {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        dot = 0.0;

    dot = 0.0;
    for (int j = 0; j < this->size; j++)
        dot += this->vec[j] * vec_obj.vec[j];

    return 0;
}

//! scale this->vec[j] *= vec[j] by a vector
FaspErrorType VEC::PointWiseMult(const VEC &vec_obj) {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        return 0;

    for (int j = 0; j < this->size; j++)
        this->vec[j] *= vec_obj.vec[j];

    return 0;
}

//! this->vec[j] = this->vec[j] / vec[j]
FaspErrorType VEC::PointWiseDivide(const VEC &vec_obj, DBL tol) {
    if (this->size != vec_obj.size)
        return 1; //! 1 marks the mismatch of vectors' lengths

    if (this->size == 0)
        return 0;

    for (int j = 0; j < this->size; j++) {
        if (fabs(vec_obj.vec[j]) <= tol) {
            std::cout << "-----------**********---------" << std::endl;
            std::cout << " Zero division error occurred " << std::endl;
            std::cout << "-----------**********---------" << std::endl;
            return 0;
        }
    }

    for (int j = 0; j < this->size; j++)
        this->vec[j] /= vec_obj.vec[j];

    return 0;
}