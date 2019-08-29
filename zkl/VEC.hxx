/**
 * VEC header file
 */

#include <vector>
#include <cmath>
#include "ErrorCode.hxx"

typedef int INT;
typedef double DBL;

/*----------------------------------------------------------------------------*/

class VEC:public std::vector<DBL>{
private:

    //! attention : vec is a column vector by default
    std::vector<DBL> vec;
public:

    //! initial constructed function
    VEC(){}

    //! set the length of vector and set the same value on class object
    VEC(INT length, DBL value);

    //! assign vec to class object
    VEC(std::vector<DBL> vec);

    //! assign array to class object
    VEC(const DBL array[],INT array_size);

    //! assign pointer to class object
    VEC(DBL *pointer,INT element_size);

    //! get this->vector[position]
    FaspErrorType get(INT position, DBL value);

    //! get the length of this->vector
    FaspErrorType len(INT len);

    //! set a value on this->vector[j]
    FaspErrorType set(INT position, DBL value);

    //! reset the length of this->vector
    //! attention : perhaps lessen the length of this->vector
    FaspErrorType setlen(INT length);

    //! erase this->vector[position]
    FaspErrorType erase(INT position);

    //! erase elements from begin_position to end_position in this->vector
    FaspErrorType erase(INT begin_position, INT end_position);

    //! erase all the elements in this->vector
    FaspErrorType erase();

    //! insert a new value at the end of vector
    FaspErrorType insert(DBL value);

    //! insert a new value at the position j, and all the elements after position j
    //! need to be moved to the next position and this->vector's length needs to be
    //! changed
    FaspErrorType insert(INT position, DBL value);

    //! insert some values from begin_position to end_position, and all the elements
    //! after position begin_postion need to be moved to the next
    //! (end_postion-begin_position+1) position and this->vector's length needs to be
    //! changed
    FaspErrorType insert(INT begin_position, INT end_position, DBL array[]);
    FaspErrorType insert(INT begin_position, INT end_position, DBL *pointer);

    //! if sign = 1 ; then
    //! insert vector1 in this->vector from begin_postion position and all the elements after
    //! begin_position + vector1.len() need to be moved to the next vector1.len() position and
    //! this->vector's length needs to be changed
    //! else
    //! insert this->vector in vector1 from begin_position position and all the elements after
    //! begin_position + this->vector.len() need to be moved to the next this->vector.len()
    //! position and vector1's length needs to be changed
    FaspErrorType insert(INT begin_position, std::vector<DBL> vector1, INT sign);

    //! concatenate two vectors
    //! if sign = 1 ; then this->vector = this->vector + vector1
    //! else vector1 = this->vector + vector1
    FaspErrorType cat(std::vector<DBL> vector1);

    //! concatenate two vectors
    //! if sign = 1 ; then vector2 = this->vector + vector1
    //! else vector2 = vector1 + this->vector
    FaspErrorType cat(std::vector<DBL> vector1, std::vector<DBL> vector2);

    //! if sign = 1 ; this->vector = a * vector1 + b * this->vector
    //! else vector1 = a * vector1 + b * this->vector
    FaspErrorType add(std::vector<DBL> vector1, DBL a, DBL b);

    //! vector2 = a * this->vector + b * vector1
    FaspErrorType add(std::vector<DBL> vector1, std::vector<DBL> vector2, DBL a, DBL b);

    //! this->vector[j] = a * this->vector[j]
    FaspErrorType scale(DBL a);

    //! vector1 = this->vector
    FaspErrorType copy(std::vector<DBL> vector1);

    //! dot = this->vector dot vector1
    FaspErrorType dot(std::vector<DBL> vector1, DBL dot);

    //! if sign = 1 ; then Tdot = \bar{this->vector}^{'} dot vector1
    //! else Tdot = this->vector^{'} dot \bar{vector1}
    FaspErrorType dot(std::vector<DBL> vector1, INT sign, DBL dot);

    //! swap this->vector's values and vector1's values
    FaspErrorType swap(std::vector<DBL> vector1);

    //! if sign = 1 ; this->vector[j] = this->vector[j] * vector1[j]
    //! else vector1[j] = this->vector[j] * vector1[j]
    FaspErrorType pointwisemult(std::vector<DBL> vector1, INT sign);

    //! if sign = 1 ; this->vector[j] = this->vector[j] / vector1[j]
    //! if sign = 2 ; this->vector[j] = vector1[j] / this->vector[j]
    //! if sign = 3 ; vector1[j] = this->vector[j] / vector1[j]
    //! else vector1[j] = vector1[j] / this->vector[j]
    FaspErrorType pointwisedivide(std::vector<DBL> vector1, INT sign);

    //! max(this->vector[j])
    FaspErrorType max();

    //! min(this->vector[j])
    FaspErrorType min();

    //! this->vector[j] = s + this->vector[j]
    FaspErrorType shift(DBL s);

    //! this->vector[j] = 1 / this->vector[j]
    FaspErrorType reciprocal();

    //! abs(this->vector[j])
    FaspErrorType abs();

    //! Norm1(this->vector)
    FaspErrorType norm1(DBL norm1);

    //! Norm2(this->vector)
    FaspErrorType norm2(DBL norm2);

    //! NormInifinity(this->vector)
    FaspErrorType normInf(DBL normInf);

    //! NormP(this->vector)
    FaspErrorType normp(DBL normp);

    //! destructor function
    ~VEC(){}
};