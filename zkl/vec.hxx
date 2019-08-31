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

    //! set the length of vec and set the same value on class object
    VEC(INT length, DBL value);

    //! assign vector to class object
    VEC(const std::vector<DBL> vec1);

    //! assign VEC object to class object
    VEC(const VEC& vec1);

    //! random constructed function, and random DBL values ranged from begin_value to
    //! end_value, and the number of random values is N
    VEC(DBL begin_value, DBL end_value, INT N);

    //! random constructed function, and random INT values ranged from begin_value to
    //! end_value, and the number of random values is N
    VEC(INT begin_value, INT end_value, INT N);

    //! assign pointer to class object
    VEC(const DBL *pointer, const INT element_size);

    //! get this->vec[position]
    FaspErrorType get(INT position, DBL *value);

    //! get the length of this->vec
    FaspErrorType len(INT *len);

    //! this->vec[j] = a * this->vec[j]
    FaspErrorType scale(DBL a);

    //! vec1 = this->vec
    FaspErrorType copy(VEC *vec1);

    //! max(this->vec[j])
    FaspErrorType max(DBL *max);

    //! min(this->vec[j])
    FaspErrorType min(DBL *min);

    //! this->vec[j] = s + this->vec[j]
    FaspErrorType shift(DBL s);

    //! abs(this->vec[j])
    FaspErrorType abs();

    //! this->vec[j] = 1 / this->vec[j]
    FaspErrorType reciprocal();

    //! Norm2(this->vec)
    FaspErrorType norm2(DBL *norm2);

    //! NormInifinity(this->vec)
    FaspErrorType normInf(DBL *normInf);

    //! this->vec = a * vec1 + b * this->vec
    FaspErrorType add(VEC vec1, DBL a, DBL b);

    //! dot = this->vec dot vec1
    FaspErrorType dot(VEC vec1, DBL *dot);

    //! this->vec[j] = this->vec[j] * vec1[j]
    FaspErrorType pointwisemult(VEC vec1);

    //! this->vec[j] = this->vec[j] / vec1[j]
    FaspErrorType pointwisedivide(VEC vec1);

    //! this->vec[j] = vec1[j] / this->vec[j]
    FaspErrorType pointwisedivided(VEC vec1);

    //! overload equals operator
    VEC& operator=(const VEC& vec1);

    //! destructor function
    ~VEC(){}
};