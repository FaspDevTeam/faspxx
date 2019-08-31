/**
 * VEC source file
 *
 * “//” marks comments left to developers.
 * "//!" marks comments left to the users.
 */

#include "vec.hxx"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>

//! developers' options
//! attention : the only one can is 1 among ASSIGN_BY_INDEX, ASSIGN_BY_PUSHBACK, ASSIGN_BY_ITERATOR
#define ASSIGN_BY_INDEX 1
#define ASSIGN_BY_PUSHBACK 0
#define ASSIGN_BY_ITERATOR 0
/*
 * some temporary information in this file
 * errortype 1 marks the empty VEC
 * errortype 2 marks the dividing zero error
 * errortype 3 marks the mismatch of vectors' lengths
 */

/*----------------------------------------------------------------------------*/

//! set the length of vec and set the same value on class object
VEC::VEC(INT length, DBL value) {
    vec.assign(length, value);
}

//! assign vector1 to class object
VEC::VEC(std::vector<DBL> vector1) {
    vec.operator=(vector1);
}

//! assign VEC object to class object
VEC::VEC(const VEC &vec1) {
    vec.operator=((vec1.vec));
}

//! random constructed function, and random DBL values ranged from begin_value to
//! end_value, and the number of random values is N
VEC::VEC(DBL begin_value, DBL end_value, INT N) {
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
}

//! random constructed function, and random INT values ranged from begin_value to
//! end_value, and the number of random values is N
VEC::VEC(INT begin_value, INT end_value, INT N) {
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
}

//! assign pointer to class object
VEC::VEC(const DBL *pointer, const INT element_size) {
    vec.assign(pointer, pointer + element_size);
}

//! get this->vec[position]
FaspErrorType VEC::get(INT position, DBL *value) {
    *value = vec.at(position);
    return 0;
}

//! array = this->vec
FaspErrorType VEC::getarr(INT *length, DBL **array) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC

    *length = vec.size();
    *array = nullptr;
    *array = new double[len];

    for (int j = 0; j < len; j++)
        (*array)[j] = vec[j];

    return 0;
}

//! get the length of this->vec
FaspErrorType VEC::len(INT *len) {
    *len = vec.size();
    return 0;
}

//! this->vec[j] = a * this->vec[j]
FaspErrorType VEC::scale(DBL a) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC

    for (int j = 0; j < len; j++)
        vec[j] = a * vec[j];

    return 0;
}

//! vec1 = this->vec
FaspErrorType VEC::copy(VEC *vec1) {
    (*vec1).vec.operator=(vec);
    return 0;
}

//! max(this->vec[j])
FaspErrorType VEC::max(DBL *max) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC

    if (len == 1) {
        *max = vec[0];
        return 0;
    }

    *max = vec[0];
    for (int j = 1; j < len; j++) {
        if (*max < vec[j])
            *max = vec[j];
    }

    return 0;
}

//! min(this->vec[j])
FaspErrorType VEC::min(DBL *min) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC object

    if (len == 1) {
        *min = vec[0];
        return 0;
    }

    *min = vec[0];
    for (int j = 1; j < len; j++) {
        if (*min > vec[j])
            *min = vec[j];
    }

    return 0;
}

//! this->vec[j] = s + this->vec[j]
FaspErrorType VEC::shift(DBL s) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC object

    for (int j = 0; j < len; j++)
        vec[j] += s;

    return 0;
}

//! abs(this->vec[j])
FaspErrorType VEC::abs() {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC object

    for (int j = 0; j < len; j++)
        vec[j] = fabs(vec[j]);

    return 0;
}

//! this->vec[j] = 1 / this->vec[j]
FaspErrorType VEC::reciprocal() {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC object

    for (int j = 0; j < len; j++) {
        if (fabs(vec[j]) <= 1e-43)
            return 2; //! errortype 2 marks the dividing zero error
    }

    for (int j = 0; j < len; j++)
        vec[j] = 1 / vec[j];

    return 0;
}

//! Norm2(this->vec)
FaspErrorType VEC::norm2(DBL *norm2) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC

    *norm2 = 0.0;
    for (int j = 0; j < len; j++)
        *norm2 += vec[j] * vec[j];

    *norm2 = sqrt(*norm2);

    return 0;
}

//! NormInifinity(this->vec)
FaspErrorType VEC::normInf(DBL *normInf) {
    int len = vec.size();
    if (len == 0)
        return 1; //! errortype 1 marks the empty VEC

    *normInf = 0.0;
    if (len == 1)
        *normInf = fabs(vec[0]);
    else {
        for (int j = 1; j < len; j++) {
            if (*normInf < fabs(vec[j]))
                *normInf = fabs(vec[j]);
        }
    }

    return 0;
}

//! this->vec = a * this->vec + b * vec1
FaspErrorType VEC::add(VEC vec1, DBL a, DBL b) {
    int len = vec.size();
    int len1 = vec1.vec.size();
    if (len != len1)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (len == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < len; j++)
        vec[j] = a * vec[j] + b * vec1.vec[j];

    return 0;
}

//! dot = this->vec dot vec1
FaspErrorType VEC::dot(VEC vec1, DBL *dot) {
    int len = vec.size();
    int len1 = vec1.vec.size();
    if (len != len1)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (len == 0)
        return 1; //! 1 marks the empty VEC

    *dot = 0.0;
    for (int j = 0; j < len; j++)
        *dot += vec[j] * vec1.vec[j];

    return 0;
}

//! this->vec[j] = this->vec[j] * vec1[j]
FaspErrorType VEC::pointwisemult(VEC vec1) {
    int len = vec.size();
    int len1 = vec1.vec.size();
    if (len != len1)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (len == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < len; j++)
        vec[j] *= vec1.vec[j];

    return 0;
}

//! this->vec[j] = this->vec[j] / vec1[j]
FaspErrorType VEC::pointwisedivide(VEC vec1) {
    int len = vec.size();
    int len1 = vec1.vec.size();
    if (len != len1)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (len == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < len; j++)
        if (fabs(vec1.vec[j]) <= 1e-43)
            return 2; //! 2 marks the dividing zero error

    for (int j = 0; j < len; j++)
        vec[j] /= vec1.vec[j];

    return 0;
}

//! this->vec[j] = vec1[j] / this->vec[j]
FaspErrorType VEC::pointwisedivided(VEC vec1) {
    int len = vec.size();
    int len1 = vec1.vec.size();
    if (len != len1)
        return 3; //! 3 marks the mismatch of vectors' lengths

    if (len == 0)
        return 1; //! 1 marks the empty VEC

    for (int j = 0; j < len; j++)
        if (fabs(vec[j]) <= 1e-43)
            return 2; //! 2 marks the dividing zero error

    for (int j = 0; j < len; j++)
        vec[j] = vec1.vec[j] / vec[j];

    return 0;
}

//! overload equals operator
//! this->vec = vec1
VEC &VEC::operator=(const VEC &vector1) {
    vec.operator=(vector1.vec);
}