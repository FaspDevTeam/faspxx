/**
 * MAT header file
 *
 * “//” marks comments left to developers.
 * "//!" marks comments left to the users.
 */

#include "vec.hxx"
/*----------------------------------------------------------------------------*/
/*! \class MAT
 *  \brief basic mat class
 *
 *  This class defines the basic mat data structure and some basic operations.
 *  attention :
 *  there are two simple examples
 *  the first ex :
 *  \[
 *  \begin{pmatrix}
 *  1 & 0 & 2 \\
 *  0 & 1 & 0 \\
 *  3 & 0 & 0 \\
 *  \end{pmatrix}
 *  \]
 *  and nnz = 5 ; row = 3 ; column = 3 ;
 *  values = { 1, 2, 1, 3, 0 }
 *  rowshift = { 0, 2, 3, 5 }
 *  colindex = { 0, 2, 1, 0, 2 }
 *  diag = { 0, 2, 4 }
 *  the second ex :
 *  \[
 *  \begin{pmatrix}
 *  1 & 7 & 2 \\
 *  0 & 0 & 0 \\
 *  3 & 0 & 4 \\
 *  \end{pmatrix}
 *  \]
 *  and nnz = 6 ; row = 3 ; column = 3 ;
 *  values = { 1, 7, 2, 0, 3, 4 }
 *  rowshift = { 0, 3, 4, 6 }
 *  colindex = { 0, 1, 2, 1, 0, 2 }
 *  diag = { 0, 3, 5 }
 */
class MAT {
private:

    //! implement the CSRx(CSR extension) data structure

    //! the number of entries of *this
    INT nnz;

    //! the row number of *this
    INT row;

    //! the column number of *this
    INT column;

    //! values contains the non-zero entries of *this, stored row by
    //! row, its size is nnz
    std::vector<DBL> values;

    //! rowshift contains the column indices of the non-zero and diagonal
    //! entries stored in this->values;
    //! its size row + 1, row is the *this 's row number
    std::vector<INT> rowshift;

    //! colindex contains the indices to the beginning of each row in the
    //! vector this->values and this->rowshift; its size is nnz
    std::vector<INT> colindex;

    //! diag contains the indices of diagonal entries in this->values,
    //! and its size is min(row, column)
    std::vector<INT> diag;
public:

    //! initial contructed function
    MAT(){};

    //! assign row, column, nnz, values, rowshift, colindex, diag to this->mat
    MAT(INT row, INT column, INT nnz, std::vector<DBL>  values,
            std::vector<INT> rowshift, std::vector<INT> colindex,
            std::vector<INT> diag);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    MAT(INT row, INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex);

    //! assign col, nnz, values, rowshift, colindex, diag to this->mat
    MAT(INT column, INT nnz, std::vector<DBL> values, std::vector<INT> rowshift,
            std::vector<INT> colindex, std::vector<INT> diag);

    //! assign col, nnz, values, rowshift, colindex to this->mat
    MAT(INT column, INT nnz, std::vector<DBL> values, std::vector<INT> rowshift,
        std::vector<INT> colindex);

    //! assign diagonal matrix to this->mat
    MAT(VEC vec);

    //! assign MAT object to this->mat
    MAT(const MAT& mat);

    //! overload equals operator
    MAT &operator=(const MAT& mat);

    //! destructor
    ~MAT(){};

    FaspErrorType CheckCSRx(INT row, INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex,
            std::vector<INT> diag);

    FaspErrorType CheckCSRx(INT row, INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex);

    FaspErrorType CheckCSRx(INT column,INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex,
            std::vector<INT> diag);

    FaspErrorType CheckCSRx(INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex);

    FaspErrorType CheckCSR(INT row, INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex);

    FaspErrorType CheckCSR(INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex);

    FaspErrorType ConvertCSR(INT row, INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex, MAT mat);

    FaspErrorType ConvertCSR(INT column, INT nnz, std::vector<DBL> values,
            std::vector<INT> rowshift, std::vector<INT> colindex, MAT mat);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    FaspErrorType MatAssign(INT row,INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex,
                            std::vector<INT> diag);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    FaspErrorType MatAssign(INT row,INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex);

    //! assign column, nnz, values, rowshift, colindex, diag to *this
    FaspErrorType MatAssign(INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex,
                            std::vector<INT> diag);
    //! assign column, nnz, values, rowshift, colindex to *this
    FaspErrorType MatAssign(INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex);

    //! get the row or column number of this->mat
    FaspErrorType GetSizes(INT &row, INT &col);

    //! get (this->mat).nnz
    INT Getnnz();

    //! get (this->mat)[i][j]
    FaspErrorType GetElem(INT row, INT column, DBL &value);

    //! get the whole jth-row elements in this->mat into VEC object
    FaspErrorType GetRow(INT row, VEC &vec);

    //! get the whole jth-column elements in this->mat into VEC object
    FaspErrorType GetColumn(INT column, VEC &vec);

    //! get the whole diagonal elements in this->mat into VEC object
    FaspErrorType GetDiag(VEC &vec);

    //! zero all the elements
    FaspErrorType Zero();

    //! copy this->mat into mat
    FaspErrorType Copy(MAT &mat);

    //! this->mat = a * this->mat
    FaspErrorType Scale(DBL a);

    //! this->mat = a * I + this->mat
    FaspErrorType Shift(DBL a);

    //! vec_b = this->mat * vec_x
    FaspErrorType MultVec(const VEC vec_x, VEC &vec_b);

    //! vec_z = vec_y + this->mat * vec_x
    FaspErrorType MultAdd(VEC vec_x, VEC vec_y, VEC vec_z);

    //! transpose this->mat
    FaspErrorType Transpose();

    //! this->mat = a * this->mat + b * mat
    FaspErrorType Add(MAT mat, DBL a, DBL b);

    //! this->mat = this->mat * mat
    FaspErrorType MultLeft(MAT mat);

    //! this->mat =  mat * this->mat
    FaspErrorType MultRight(MAT mat);

    //! this->mat = mat_left * this->mat * mat_right
    FaspErrorType MultLeftRight(MAT mat_left, MAT mat_right);

    //! this->mat = transpose(P) * this->mat * P
    FaspErrorType MultP(MAT P);

    //! vec_z = vec_y + transpose(this->mat) * vec_x
    FaspErrorType MultTransposeAdd(VEC vec_x, VEC vec_y, VEC vec_z);

};