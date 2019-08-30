/**
 * VEC header file
 *
 * “//” marks comments left to developers.
 * "//!" marks comments left to the users.
 */

#include <vector>
#include <cmath>
#include "errortype.hxx"

/*----------------------------------------------------------------------------*/

class VEC:public std::vector<DBL>{
private:

    // attention : vec is a column vector by default
    std::vector<DBL> vec;
public:

    //! initial constructed function
    VEC(){}

    //! set the length of vector and set the same value on class object
    VEC(INT length, DBL value);

    //! assign vec to class object
    VEC(const std::vector<DBL> vec);

    //! random constructed function, and random DBL values ranged from begin_value to
    //! end_value, and the number of random values is N
    VEC(DBL begin_value, DBL end_value, INT N);

    //! random constructed function, and random INT values ranged from begin_value to
    //! end_value, and the number of random values is N
    VEC(INT begin_value, INT end_value, INT N);

    //! assign pointer to class object
    VEC(const DBL *pointer, const INT element_size);

    //! get this->vector[position]
    FaspErrorType get(INT position, DBL *value);

    //! get the length of this->vector
    FaspErrorType len(INT *len);

    //! this->vector[j] = a * this->vector[j]
    FaspErrorType scale(DBL a);

    //! vector1 = this->vector
    FaspErrorType copy(std::vector<DBL> vector1);

    //! max(this->vector[j])
    FaspErrorType max(DBL *max);

    //! min(this->vector[j])
    FaspErrorType min(DBL *min);

    //! this->vector[j] = s + this->vector[j]
    FaspErrorType shift(DBL s);

    //! abs(this->vector[j])
    FaspErrorType abs();

    //! this->vector[j] = 1 / this->vector[j]
    FaspErrorType reciprocal();

    //! Norm2(this->vector)
    FaspErrorType norm2(DBL *norm2);

    //! NormInifinity(this->vector)
    FaspErrorType normInf(DBL *normInf);

    //! this->vector = a * vector1 + b * this->vector
    FaspErrorType add(std::vector<DBL> vector1, DBL a, DBL b);

    //! dot = this->vector dot vector1
    FaspErrorType dot(std::vector<DBL> vector1, DBL *dot);

    //! this->vector[j] = this->vector[j] * vector1[j]
    FaspErrorType pointwisemult(std::vector<DBL> vector1);

    //! this->vector[j] = this->vector[j] / vector1[j]
    FaspErrorType pointwisedivide(std::vector<DBL> vector1);

    //! this->vector[j] = vector1[j] / this->vector[j]
    FaspErrorType pointwisedivided(std::vector<DBL> vector1);

    //! overload equals operator
    VEC& operator=(const VEC& vector1);

    //! destructor function
    ~VEC(){}
};