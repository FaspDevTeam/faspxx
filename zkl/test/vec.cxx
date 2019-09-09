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

//! developers' options
//! attention : the only one can is 1 among ASSIGN_BY_INDEX,
//! ASSIGN_BY_PUSHBACK, ASSIGN_BY_ITERATOR
#define ASSIGN_BY_INDEX 1
#define ASSIGN_BY_PUSHBACK 0
#define ASSIGN_BY_ITERATOR 0
/*
 * some temporary information in this file
 * errortype 1 marks the empty VEC
 * errortype 2 marks the dividing zero error
 * errortype 3 marks the mismatch of vectors' lengths
 */

//-----------------------------------------------------------------------------------

//! set the size of vec and set the same value on class object
VEC::VEC(const INT size, const DBL value) {
    vec.assign(size, value);
    this->size=size;
}

//! assign vector to class object
VEC::VEC(std::vector <DBL> vector_obj) {
    vec.operator=(vector_obj);
    size=vector_obj.size();
}

//! assign VEC object to class object
VEC::VEC(const VEC &vec_obj) {
    this->vec.operator=(vec_obj.vec);
    this->size=vec_obj.size;
}

//! random construction function, N DBL values from begin_value to end_value
VEC::VEC(const DBL begin_value, const DBL end_value, const INT N) {
    vec.reserve(N); // performance burdens
    srand((unsigned) time(NULL));
#if ASSIGN_BY_INDEX
    for (int j = 0; j < N; j++)
        vec[j] = 1.0*rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value;
#endif
#if ASSIGN_BY_PUSHBACK
    for (int j = 0; j < N; j++)
        vec.push_back(rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value);
#endif
#if ASSIGN_BY_ITERATOR
    for (std::vector<DBL>::iterator iter = vec.begin(); iter != vec.end(); iter++)
        *iter = rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value;
#endif
    size=N;
}

//! random construction function, N INT values from begin_value to end_value
VEC::VEC(const INT begin_value, const INT end_value, const INT N) {
    vec.reserve(N); // performance burdens
    srand((unsigned) time(NULL));
#if ASSIGN_BY_INDEX
    for (int j = 0; j < N; j++)
        vec[j] = rand() % (end_value - begin_value) + begin_value;
#endif
#if ASSIGN_BY_PUSHBACK
    for(int j = 0; j < N; j++)
        vec.push_back(rand() % (end_value - begin_value) + begin_value);
#endif
#if ASSIGN_BY_ITERATOR
    for (std::vector<DBL>::iterator iter = vec.begin(); iter != vec.end(); iter++)
        *iter = rand() % (end_value - begin_value) + begin_value;
#endif
    size=N;
}

//! assign pointer to class object
VEC::VEC(const DBL *pointer, const INT size) {
    vec.assign(pointer, pointer + size);
    this->size=size;
}

//! overload equals operator
VEC &VEC::operator=(const VEC &vec_obj) {
    this->vec.operator=(vec_obj.vec);
    this->size=vec_obj.size;
}

//! overload [] operator
DBL &VEC::operator[](const INT position){
    return vec[position];
}

//! overload += operator
VEC &VEC::operator += (const VEC &vec_obj){
    for(int j=0;j<size;j++){
        vec[j]+=vec_obj.vec[j];
    }
    return *this;
}

//! overload -= operator
VEC &VEC::operator -= (const VEC &vec_obj) {
    for(int j=0;j<size;j++){
        vec[j]-=vec_obj.vec[j];
    }
    return *this;
}
//! set the size of vec
FaspErrorType VEC::SetSize(const INT size){
    if(size<0)
        return 1; //! errortype 1 marks the empty VEC

    this->size=size;
    vec.reserve(size);
    return 0;
}

//! set the size of vec and set the same value on class object
FaspErrorType VEC::SetSame(const INT size, const DBL value){
    vec.assign(value, size);
    this->size=size;
    return 0;
}

//! assign vector to class object
FaspErrorType VEC::SetVec(const std::vector <DBL> vector_object){
    vec.operator=(vector_object);
    size=vector_object.size();
    return 0;
}

//! random construction function, N DBL values from begin_value to end_value
FaspErrorType VEC::RandomDBL(const DBL begin_value, const DBL end_value, const INT N){
    vec.reserve(N); // performance burdens
    srand((unsigned) time(NULL));
#if ASSIGN_BY_INDEX
    for (int j = 0; j < N; j++)
        vec[j] = rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value;
#endif
#if ASSIGN_BY_PUSHBACK
    for (int j = 0; j < N; j++)
        vec.push_back(rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value);
#endif
#if ASSIGN_BY_ITERATOR
    for (std::vector<DBL>::iterator iter = vec.begin(); iter != vec.end(); iter++)
        *iter = rand() / (DBL) RAND_MAX * (end_value - begin_value) + begin_value;
#endif
    size=N;
}

//! random construction function, N INT values from begin_value to end_value
FaspErrorType VEC::RandomINT(const INT begin_value, const INT end_value, const INT N){
    vec.reserve(N); // performance burdens
    srand((unsigned) time(NULL));
#if ASSIGN_BY_INDEX
    for (int j = 0; j < N; j++)
        vec[j] = rand() % (end_value - begin_value) + begin_value;
#endif
#if ASSIGN_BY_PUSHBACK
    for(int j = 0; j < N; j++)
        vec.push_back(rand() % (end_value - begin_value) + begin_value);
#endif
#if ASSIGN_BY_ITERATOR
    for (std::vector<DBL>::iterator iter = vec.begin(); iter != vec.end(); iter++)
        *iter = rand() % (end_value - begin_value) + begin_value;
#endif
    size=N;
}

//! this->vec = array
FaspErrorType VEC::AssignByArray(const INT size, const DBL *array) {
    if (array == nullptr || size == 0)
        return 4; //! errortype 4 marks assignment object is null or its length is wrong
    vec.assign(array, array + size);
    this->size=size;
    return 0;
}

//! get the value of this->vec[position]
FaspErrorType VEC::Get(const INT position, DBL &value) const {
    value = vec.at(position);
    return 0;
}

//! get array = this->vec of size = min(size, this->size)
FaspErrorType VEC::GetArray(const INT &size, DBL **array) const {
    int len;
    if(this->size==0)
        return 1; //! errortype 1 marks the empty VEC

    if(size==0){
        *array= nullptr;
        return 0;
    }

    len=size>this->size?this->size:size;
    *array=new DBL[len];
    for(int j=0;j<len;j++)
        (*array)[j]=vec[j];

    return 0;
}

//! get the size of this->vec
INT VEC::GetSize() const {
    return this->size;
}

//! scale this->vec[j] = a * this->vec[j] by a scalar
FaspErrorType VEC::Scale(DBL a) {
    if (size == 0)
        return 1; //! errortype 1 marks the empty VEC

    for (int j = 0; j < size; j++)
        vec[j] = a * vec[j];

    return 0;
}

//! copy vec_obj = this->vec
FaspErrorType VEC::Copy(VEC &vec_obj) const{
    vec_obj.vec.operator=(this->vec);
    vec_obj.size=size;
    return 0;
}

//! find max(this->vec[j])
FaspErrorType VEC::Max(DBL &max) const {
    if (size == 0)
        return 1; //! errortype 1 marks the empty VEC

    if (size == 1) {
        max = vec[0];
        return 0;
    }
    
    max = vec[0];
    for (int j = 1; j < size; j++) {
        if (max < vec[j])
            max = vec[j];
    }

    return 0;
}

//! find min(this->vec[j])
FaspErrorType VEC::Min(DBL &min) const {
    if (size == 0)
        return 1; //! errortype 1 marks the empty VEC object

    if (size == 1) {
        min = vec[0];
        return 0;
    }

    min = vec[0];
    for (int j = 1; j < size; j++) {
        if (min > vec[j])
            min = vec[j];
    }

    return 0;
}

//! shift this->vec[j] += s by a scalar
FaspErrorType VEC::Shift(DBL s) {
    if (size == 0)
        return 1; //! errortype 1 marks the empty VEC object

    for (int j = 0; j < size; j++)
        vec[j] += s;

    return 0;
}

//! compute abs(this->vec[j])
void VEC::Abs() {
    if (size == 0)
        return 1; //! errortype 1 marks the empty VEC object

    for (int j = 0; j < size; j++)
        vec[j] = fabs(vec[j]);
}

//! this->vec[j] = 1 / this->vec[j]
FaspErrorType VEC::Reciprocal() {
    if (size == 0)
        return 1; //! errortype 1 marks the empty VEC object

    for (int j = 0; j < size; j++) {
        if (fabs(vec[j]) <= 1e-43)
            return 2; //! errortype 2 marks the dividing zero error
    }

    for (int j = 0; j < size; j++)
        vec[j] = 1 / vec[j];

    return 0;
}

//! find l2-norm of this->vec
DBL VEC::Norm2() const {
    if (size == 0)
        return 0;

    norm2 = 0.0;
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
FaspErrorType VEC::Add(const VEC vec_obj, const DBL a, const DBL b) {
    int len = vec_obj.size;
    if (size != len)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (size == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < size; j++)
        vec[j] = a * vec[j] + b * vec_obj.vec[j];

    return 0;
}

//! this->vec = a * vec1_obj + b * vec2_obj
FaspErrorType VEC::Add(const DBL a, VEC vec1_obj, const DBL b, VEC vec2_obj){
    if(vec1_obj.size!=vec2_obj.size)
        return 3; //! 3 marks the mismatch of vectors' lengths

    for(int j=0;j<vec1_obj.size;j++)
        vec[j]=a*vec1_obj.vec[j]+b*vec2_obj.vec[j];

    return 0;
}

//! dot product of this->vec and vec
FaspErrorType VEC::Dot(VEC vec_obj, DBL &dot) const {
    if (size != vec_obj.size)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (size == 0)
        return 1; //! 1 marks the empty VEC

    dot = 0.0;
    for (int j = 0; j < size; j++)
        dot += vec[j] * vec_obj.vec[j];

    return 0;
}

//! scale this->vec[j] *= vec[j] by a vector
FaspErrorType VEC::PointWiseMult(const VEC vec_obj) {
    if (size != vec_obj.size)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (size == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < size; j++)
        vec[j] *= vec_obj.vec[j];

    return 0;
}

//! this->vec[j] = this->vec[j] / vec[j]
FaspErrorType VEC::PointWiseDivide(const VEC vec_obj) {
    if (size != vec_obj.size)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (size == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < size; j++)
        if (fabs(vec_obj.vec[j]) <= 1e-43)
            return 2; //! 2 marks the dividing zero error

    for (int j = 0; j < size; j++)
        this->vec[j] /= vec_obj.vec[j];

    return 0;
}