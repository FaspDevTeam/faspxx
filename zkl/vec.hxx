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
/*! \class VEC
 *  \brief basic vec class
 *
 *  This class defines the basic vec data structure and some basic operations.
 */
class VEC{
private:

    // attention : vec is a column vector by default
    std::vector<DBL> vec;
public:

    //! initial constructed function
    VEC(){}

    //! set the length of vec and set the same value on class object
    VEC(INT length, DBL value);

    //! assign vector to class object
    VEC(const std::vector<DBL> vector_object);

    //! assign VEC object to class object
    VEC(const VEC& vec);

    //! random constructed function, and random DBL values ranged from begin_value to
    //! end_value, and the number of random values is N
    VEC(DBL begin_value, DBL end_value, INT N);

    //! random constructed function, and random INT values ranged from begin_value to
    //! end_value, and the number of random values is N
    VEC(INT begin_value, INT end_value, INT N);

    //! assign pointer to class object
    VEC(const DBL *pointer, const INT element_size);

    //! get this->vec[position]
    FaspErrorType Get(INT position, DBL &value);

    //! array = this->vec
    FaspErrorType GetArray(INT &length, DBL **array);

    //! get the length of this->vec
    FaspErrorType Len(INT &len);

    //! this->vec[j] = a * this->vec[j]
    FaspErrorType Scale(DBL a);

    //! vec = this->vec
    FaspErrorType Copy(VEC &vec);

    //! max(this->vec[j])
    FaspErrorType Max(DBL &max);

    //! min(this->vec[j])
    FaspErrorType Min(DBL &min);

    //! this->vec[j] = s + this->vec[j]
    FaspErrorType Shift(DBL s);

    //! abs(this->vec[j])
    FaspErrorType Abs();

    //! this->vec[j] = 1 / this->vec[j]
    FaspErrorType Reciprocal();

    //! Norm2(this->vec)
    FaspErrorType Norm2(DBL &norm2);

    //! NormInifinity(this->vec)
    FaspErrorType NormInf(DBL &normInf);

    //! this->vec = a * vec + b * this->vec
    FaspErrorType Add(VEC vec, DBL a, DBL b);

    //! dot = this->vec dot vec
    FaspErrorType Dot(VEC vec, DBL &dot);

    //! this->vec[j] = this->vec[j] * vec[j]
    FaspErrorType PointWiseMult(VEC vec);

    //! this->vec[j] = this->vec[j] / vec[j]
    FaspErrorType PointWiseDivide(VEC vec);

    //! this->vec[j] = vec[j] / this->vec[j]
    FaspErrorType PointWiseDivided(VEC vec);

    //! overload equals operator
    VEC& operator=(const VEC& vec);

    //! destructor function
    ~VEC(){}
};