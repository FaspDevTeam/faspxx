/*! \file vec.cxx
 *  \brief Source file for the FASP++ Vector class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "vec.hxx"
#include "error.hxx"
#include <cmath>
#include <iostream>

/**
 * set VEC object 's length to "size" and all its elements ' values is "value"
 * if the input parameter "size" <= 0, and iniialize it by default constructor;
 * if the input parameter "size" > 0, the let this->size=size, and all it's elements
 * is the same as "value"
 */
//! set the size of vec and set the same value on VEC object
VEC::VEC(const INT size, const DBL value) {
    try {
        if ( size < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        this->vec.resize(0);
        this->size = 0;
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
 * if the input parameter "pointer" is null or the input parameter
 * "size" is not more than 0, the VEC object is set as null
 */
//! assign pointer to VEC object
VEC::VEC(const INT size, const DBL *pointer) {
    try {
        if ( size < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->vec.resize(0);
        this->size = 0;
        return;
    }

    if ( pointer == nullptr || size == 0 ) {
        this->vec.resize(0);
        this->size = 0;
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
DBL &VEC::operator[](const INT position) {
    return this->vec[position];
}

/**
 * attention : the overloaded function doesn't check whether
 * the index crossover
 */
//! overload [] operator
const DBL &VEC::operator[](const INT position) const {
    return this->vec[position];
}

/**
 * if there is a mismatch between "vec_obj" 's size and *this 's size
 * it throws an exception and return the *this
 */
//! overload += operator
VEC &VEC::operator+=(const VEC &vec_obj) {
    try {
        if ( vec_obj.size != this->size ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return *this;
    }
    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] += vec_obj.vec[j];

    return *this;
}

/**
 * if there is a mismatch between "vec_obj" 's size and *this 's size
 * it throws an exception and return the *this
 */
//! overload -= operator
VEC &VEC::operator-=(const VEC &vec_obj) {
    try {
        if ( vec_obj.size != this->size ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return *this;
    }
    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] -= vec_obj.vec[j];

    return *this;
}

/**
 * if the input parameter "size" < 0, throw an ecxeption.
 * Don't change the *this
 */
//! set the size of VEC object
void VEC::SetSize(const INT size) {
    try {
        if ( size < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    this->size = size;
    this->vec.resize(size);
}

/**
 * if the input parameter "size" < 0, throw an ecxeption.
 * Don't change the *this
 */
//! set the size of VEC object and set the same value on VEC object
void VEC::SetValues(const INT size, const DBL value) {
    try {
        if ( size < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
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
 * if the input parameter "size" < 0, throw an ecxeption.
 * Don't change the *this
 */
//! this->vec = array
void VEC::SetValues(const INT size, const DBL *array) {
    try {
        if ( size < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    if ( array == nullptr || size == 0 ) {
        this->size = 0;
        this->vec.resize(0);
        return;
    }
    this->vec.assign(array, array + size);
    this->size = size;
}

/**
 * if "position >= size" or "position < 0" happens , then throw an
 * exception, and return zero
 */
//! get the value of this->vec[position]
DBL VEC::Get(const INT position) const {
    try {
        if ( position >= size || position < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return 0;
    }

    return this->vec.at(position);
}

/**
 * if the input parameter "size" < 0, throw an exception.
 * if size == 0 , set "array" as nullptr, other cases are
 * normally dealt
 */
//! get array = this->vec of size = min(size, this->size)
void VEC::GetArray(const INT &size, DBL **array) const {
    try {
        if ( size < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    if ( size == 0 || this->size == 0 ) {
        *array = nullptr;
        return;
    }

    INT len = size > this->size ? this->size : size;
    *array = new DBL[len];
    for ( INT j = 0; j < len; j++ )
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
    if ( this->size == 0 )
        return;
    else {
        for ( INT j = 0; j < this->size; j++ )
            this->vec[j] = a * this->vec[j];
    }
}

/**
 * no matter what the *this is, copy it into the vec_obj
 */
//! copy vec_obj = this->vec
void VEC::CopyTo(VEC &vec_obj) const {
    vec_obj.vec.operator=(this->vec);
    vec_obj.size = this->size;
}

/**
 * if the VEC object doesn't contain any elements,
 * throw an exception and set the maximum value to zero
 */
//! find max(this->vec[j])
DBL VEC::Max() const {
    FaspErrorCode errorCode;
    try {
        if ( this->size == 0 ) {
            errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return 0;
    }

    DBL max;
    if ( this->size == 1 ) {
        max = this->vec[0];
    } else {
        max = this->vec[0];
        for ( INT j = 1; j < this->size; j++ ) {
            if ( max < this->vec[j] )
                max = this->vec[j];
        }
    }
    return max;
}

/**
 * if the VEC object doesn't contain any elements,
 * throw an exception and set the minimum value to zero
 */
//! find min(this->vec[j])
DBL VEC::Min() const {
    FaspErrorCode errorCode;
    try {
        if ( this->size == 0 ) {
            errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return 0;
    }

    DBL min = 0;
    if ( this->size == 1 ) {
        min = this->vec[0];
    } else {
        min = this->vec[0];
        for ( INT j = 1; j < this->size; j++ ) {
            if ( min > this->vec[j] )
                min = this->vec[j];
        }
    }

    return min;
}

/**
 * if the VEC object doesn't contain any elements,
 * No action on the VEC object!
 */
//! shift this->vec[j] += s by a scalar
void VEC::Shift(DBL s) {
    if ( this->size == 0 )
        this->vec.resize(0);

    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] += s;
}

/**
 * if the VEC object doesn't contain any elements,
 * No action on the VEC object!
 */
//! compute abs(this->vec[j])
void VEC::Abs() {
    if ( this->size == 0 ) {
        this->vec.resize(0);
        return;
    }

    for ( INT j = 0; j < this->size; j++ )
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
    if ( this->size == 0 ) {
        this->vec.resize(0);
        return;
    }

    for ( INT j = 0; j < this->size; j++ ) {
        try {
            if ( fabs(this->vec[j]) <= tol ) {
                FaspErrorCode errorCode = FaspErrorCode::ERROR_DIVIDE_ZERO;
                throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
            }
        } catch ( FaspException &ex ) {
            std::cerr << " ### ERROR : " << ex.what() << std::endl;
            std::cerr << " ### ERROR : Check : " << ex.getFile() << " at Line " << ex
                    .getLine() << ex.getLine() << std::endl;
            return;
        }
    }

    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] = 1 / this->vec[j];
}

/**
 * if the VEC object doesn't contain any elements, throw an exception and set
 * l2-norm to zero, other cases is normally dealt.
 */
//! find l2-norm of this->vec
DBL VEC::Norm2() const {
    try {
        if ( this->size == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return 0;
    }

    DBL norm2 = 0.0;
    for ( INT j = 0; j < this->size; j++ )
        norm2 += this->vec[j] * this->vec[j];

    return sqrt(norm2);
}

/**
 * if the VEC object doesn't contain any elements, throw an exception and set the
 * infinity norm to zero, other cases is normally dealt
 */
//! find infinity norm of this->vec
DBL VEC::NormInf() const {
    FaspErrorCode errorCode;
    try {
        if ( this->size == 0 ) {
            errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return 0;
    }

    DBL normInf = 0.0;
    if ( this->size == 1 )
        normInf = fabs(this->vec[0]);
    else {
        for ( INT j = 1; j < this->size; j++ ) {
            if ( normInf < fabs(this->vec[j]))
                normInf = fabs(this->vec[j]);
        }
    }

    return normInf;
}

/**
 * if there is a mismatch between "vec_obj" and *this, throw an exception and end
 * the function
 */
//! this->vec = a * this->vec + b * vec_obj
void VEC::Add(const VEC &vec_obj, const DBL a, const DBL b) {
    try {
        if ( this->size != vec_obj.size ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }

    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] = a * this->vec[j] + b * vec_obj.vec[j];
}

/**
 * there is a mismatch between "vec1_obj" and "vec2_obj", throw an exception and
 * end the function
 */
//! this->vec = a * vec1_obj + b * vec2_obj
void VEC::Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj) {
    try {
        if ( vec1_obj.size != vec2_obj.size ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }

    this->size = vec1_obj.size;
    this->vec.resize(this->size);
    for ( INT j = 0; j < vec1_obj.size; j++ )
        this->vec[j] = a * vec1_obj.vec[j] + b * vec2_obj.vec[j];

}

/**
 * if the input parameter "vec_obj" 's size is not equal to this->size,
 * throw an exception and end the function.
 * if the *this 's size is equal to zero, throw an exception and end the function.
 */
//! dot product of this->vec and vec_obj
void VEC::Dot(const VEC &vec_obj, DBL &dot) const {
    try {
        if ( this->size != vec_obj.size || this->size == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    try {
        if ( this->size == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }

    dot = 0.0;
    for ( INT j = 0; j < this->size; j++ )
        dot += this->vec[j] * vec_obj.vec[j];

}

/**
 * if "this->size" is not equal to "vec_obj.size" or "this->size" is equal to zero,
 * throw an exception and end the function.
 */
//! scale this->vec[j] *= vec_obj[j] by a vector
void VEC::PointwiseMult(const VEC &vec_obj) {
    try {
        if ( this->size != vec_obj.size ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    try {
        if ( this->size == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }

    for ( INT j = 0; j < this->size; j++ )
        this->vec[j] *= vec_obj.vec[j];

}

/**
 * if "this->size" is not equal to "vec_obj.size" or "this->size" is equal to zero.
 * throw an exception.
 * if there is an element whose absolute value is less than "tol", throw an
 * exception and end the function.
 */
//! this->vec[j] = this->vec[j] / vec_obj[j]
void VEC::PointwiseDivide(const VEC &vec_obj, DBL tol) {
    FaspErrorCode errorCode;
    try {
        if ( this->size != vec_obj.size ) {
            errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    try {
        if ( this->size == 0 ) {
            errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line "
                  << ex.getLine()
                  << std::endl;
        return;
    }
    for ( INT j = 0; j < this->size; j++ ) {
        try {
            if ( fabs(vec_obj[j]) <= tol ) {
                errorCode = FaspErrorCode::ERROR_DIVIDE_ZERO;
                throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
            }
        } catch ( FaspException &ex ) {
            std::cerr << " ### ERROR : " << ex.what() << std::endl;
            std::cerr << " ### ERROR : Check " << ex.getFile() << " at Line " << ex
                    .getLine() << std::endl;
            return;
        }
    }

    for ( INT j = 0; j < this->size; j++ ) {
        this->vec[j] /= vec_obj[j];
    }
}