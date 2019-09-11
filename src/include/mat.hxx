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
    MAT() : row(0), column(0), nnz(0), values(0), rowshift(0),
            colindex(0), diag(0) {};

    //! assign row, column, nnz, values, rowshift, colindex, diag to this->mat
    MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
    values, const std::vector<INT> rowshift, const std::vector<INT> colindex,
        const std::vector<INT> diag);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
    values, const std::vector<INT> rowshift, const std::vector<INT>
        colindex);

    //! assign diagonal matrix to this->mat
    MAT(VEC &vec_obj);

    //! assign diagonal matrix to this->mat
    MAT(const std::vector<DBL> &vector_obj);

    //! assign MAT object to this->mat
    MAT(const MAT &mat);

    //! overload equals operator
    MAT &operator=(const MAT &mat);

    //! destructor
    ~MAT() {};

    FaspErrorType CheckCSRx(const INT row, const INT column, const INT nnz,
                            const std::vector<DBL> values,
                            const std::vector<INT> rowshift,
                            const std::vector<INT> colindex,
                            const std::vector<INT> diag);

    FaspErrorType CheckCSR(const INT row, const INT column, const INT nnz,
                           const std::vector<DBL> values,
                           const std::vector<INT> rowshift,
                           const std::vector<INT> colindex);

    FaspErrorType ConvertCSR(const INT row, const INT column, const INT nnz,
                             const std::vector<DBL> values,
                             const std::vector<INT> rowshift,
                             const std::vector<INT> colindex, MAT &mat);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    void SetValues(const INT row, const INT column, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowshift,
                   const std::vector<INT> colindex, const std::vector<INT> diag);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    void SetValues(const INT row, const INT column, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowshift,
                   const std::vector<INT> colindex);

    //! get the row or column number of this->mat
    void GetSizes(INT &row, INT &col) const;

    //! get (this->mat).nnz
    INT Getnnz() const;

    //! get (this->mat)[i][j]
    FaspErrorType GetElem(const INT row, const INT column, DBL &value) const;

    //! get the whole jth-row elements in this->mat into VEC object
    FaspErrorType GetRow(const INT row, std::vector<DBL> &vector_obj) const;

    //! get the whole jth-column elements in this->mat into VEC object
    FaspErrorType GetColumn(const INT column, std::vector<DBL> &vec_obj) const;

    //! get the whole diagonal elements in this->mat into VEC object
    void GetDiag(std::vector<DBL> &vector_obj) const;

    //! zero all the elements
    void Zero();

    //! copy this->mat into mat
    void Copy(MAT &mat) const;

    //! this->mat = a * this->mat
    void Scale(const DBL a);

    //! this->mat = a * I + this->mat
    void Shift(const DBL a);

    //! vec_b = this->mat * vec_x
    FaspErrorType MultVec(VEC vec_x, VEC &vec_b) const;

    //! transpose this->mat
    void Transpose();

    //! vec3 = vec2 + transpose(this->mat) * vec1
    FaspErrorType MultTransposeAdd(const VEC vec1, const VEC vec2, VEC &vec3) const;

    //! *this = a * *this + b * mat
    FaspErrorType Add(const DBL a, const DBL b, const MAT mat);

    //! *this = *this * mat
    FaspErrorType MultLeft(const MAT mat);

    //! *this = mat * *this
    FaspErrorType MultRight(const MAT mat);

    //! mat3 = a * mat1 + b * mat2
    friend FaspErrorType
    Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2,
        MAT &mat3);

    //! mat3 = mat1 * mat2
    friend FaspErrorType Mult2(const MAT mat1, const MAT mat2, MAT &mat3);

    //! mat4 = mat1 * mat2 * mat3
    friend FaspErrorType Mult3(const MAT mat1, const MAT mat2, const MAT mat3, MAT
    &mat4);

    //! mat3 = transpose(mat1) * mat2 * mat1
    friend FaspErrorType MultP(const MAT mat1, const MAT mat2, MAT &mat3);

};
