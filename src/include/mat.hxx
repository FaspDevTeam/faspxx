/**
 * MAT header file
 *
 * “//” marks comments left to developers.
 * "//!" marks comments left to the users.
 */

#include "vec.hxx"

class MAT:public VEC{
private:

    //! implement the CSRx(CSR extension) data structure
    //! the CSRx is only applicable in the SPD matrix here

    //! the number of entries of this->mat
    INT nnz;

    //! AA contains the non-zero entries of this->mat, stored row by row, its size is nnz
    VEC AA;

    //! IA contains the column indices of the non-zero entries stored in AA;
    //! its size row + 1, row is the this->mat's row number
    VEC IA;

    //! JA contains the indices to the beginning of each row in the vector AA and JA;
    //! its size is nnz
    VEC JA;

    //! DIA contains the indices of diagonal entries in AA, and its size is
    VEC DIA;
public:

    //! initial contructed function
    MAT(){};

    //! assign MAT object to this->mat
    MAT(const MAT& mat1);

    //! assign nnz, AA, IA, JA, DIA to this->mat
    MAT(INT nnz, VEC AA, VEC IA, VEC JA, VEC DIA);

    //! assign nnz, AA, IA, JA to this->mat
    MAT(INT nnz, VEC AA, VEC IA, VEC JA);

    //! assign scalar matrix to this->mat
    MAT(DBL value, INT size);

    //! assign diagonal matrix to this->mat
    MAT(VEC vec1);

    //! assign tridiagonal matrix to this->mat
    //! attention : whether the tridiagonal matrix is needed to judge a spd matrix?
    MAT(DBL down, DBL diag, DBL up, INT size);

    //! get the row or column number of this->mat
    FaspErrorType getsize(INT *row);

    //! get (this->mat).nnz
    FaspErrorType getelemsize(INT *nnz);

    //! get (this->mat)[i][j]
    FaspErrorType getelem(INT row, INT column);

    //! get the whole jth-row elements in this->mat into VEC object
    FaspErrorType getrowvec(INT row, VEC *vec1);

    //! get the whole jth-row elements in this->mat into Array object
    FaspErrorType getrowarr(INT row, DBL *array);

    //! get the whole jth-column elements in this->mat into VEC object
    FaspErrorType getcolvec(INT column, VEC *vec1);

    //! get the whole jth-column elements in this->mat into Array object
    FaspErrorType getcolarr(INT column, DBL *array);

    //! get the whole diagonal elements in this->mat into VEC object
    FaspErrorType getdiagvec(VEC *vec1);

    //! get the whole diagonal elements in this->mat into Array object
    FaspErrorType getdiagarr(DBL *array);

    //! get the whole elements into VEC object
    FaspErrorType getwholevec(VEC vec1);

    //! get the whole elements into Array object
    FaspErrorType getwholearr(DBL *array);

    //! zero all the elements
    FaspErrorType zero();

    //! copy this->mat into mat1
    FaspErrorType copy(MAT *mat1);

    //! this->mat = a * this->mat
    FaspErrorType scale(DBL a);

    //! this->mat = a + this->mat
    FaspErrorType shift(DBL a);

    //! this->mat[j] = a + this->mat[j] and j is the only diagonal elements' indices
    FaspErrorType shiftdiag(DBL a);

    //! transpose this->mat
    FaspErrorType transpose();

    //! this->mat = a * this->mat + b * mat1
    FaspErrorType add(MAT mat1, DBL a, DBL b);

    //! this->mat = this->mat * mat1
    FaspErrorType multl(MAT mat1);

    //! this->mat =  mat1 * this->mat
    FaspErrorType multr(MAT mat1);

    //! this->mat = mat1 * this->mat * mat2
    FaspErrorType multlr(MAT mat1, MAt mat2);

    //! vec2 = this->mat * vec1
    FaspErrorType multvec(VEC vec1, VEC vec2);

    //! vec3 = vec2 + this->mat * vec1
    FaspErrorType multadd(VEC vec1, VEC vec2, VEC vec3);

    //! vec3 = vec2 + transpose(this->mat) * vec1
    FaspErrorType multtransposeadd(VEC vec1, VEC vec2, VEC vec3);

    //! norm1(this->mat)
    FaspErrorType norm1(DBL *norm1);

    //! norm2(this->mat)
    //! programing is very hard
    FaspErrorType norm2(DBL *norm2);

    //! normInf(this->mat)
    FaspErrorType normInf(DBL *normInf);

    //! destructor
    ~MAT(){};
};