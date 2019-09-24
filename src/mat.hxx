/*! \file mat.hxx
 *  \brief Header file for the FASP++ Matrix class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __MAT_HEADER__      /*-- allow multiple inclusions --*/
#define __MAT_HEADER__      /**< indicate mat.hxx has been included before */


#include "vec.hxx"

/*---------------------------------------------------------------------------------*/
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
    INT nnz_;

    //! the row number of *this
    INT row_;

    //! the column number of *this
    INT column_;

    //! values contains the non-zero entries of *this, stored row by
    //! row, its size is nnz
    std::vector<DBL> values_;

    //! rowshift contains the column indices of the non-zero and diagonal
    //! entries stored in this->values;
    //! its size row + 1, row is the *this 's row number
    std::vector<INT> rowshift_;

    //! colindex contains the indices to the beginning of each row in the
    //! vector this->values and this->rowshift; its size is nnz
    std::vector<INT> colindex_;

    //! diag contains the indices of diagonal entries in this->values,
    //! and its size is min(row, column)
    std::vector<INT> diag_;

public:

    //! initial contructed function
    MAT() : row_(0), column_(0), nnz_(0), values_(0), rowshift_(0),
            colindex_(0), diag_(0) {};

    //! assign row, column, nnz, values, rowshift, colindex, diag to *this
    MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
    values, const std::vector<INT> rowshift, const std::vector<INT> colindex,
        const std::vector<INT> diag);

    //! assign row, column, nnz, rowshift, colindex, diag to *this
    MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
    rowshift, const std::vector<INT> colindex, const std::vector<INT>
        diag);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
    values, const std::vector<INT> rowshift, const std::vector<INT>
        colindex);

    //! assign row, column, nnz, rowshift, colindex to *this
    MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
    rowshift, const std::vector<INT> colindex);

    //! assign diagonal matrix to *this
    MAT(VEC &vec);

    //! assign diagonal matrix to *this
    MAT(const std::vector<DBL> &vect);

    //! assign MAT object to *this
    MAT(const MAT &mat);

    //! overload equals operator
    MAT &operator=(const MAT &mat);

    //! destructor
    ~MAT() {};

    //! assign row, column, nnz, values, rowshift, colindex to *this
    void SetValues(const INT row, const INT column, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowshift,
                   const std::vector<INT> colindex, const std::vector<INT> diag);

    //! assign row, column, nnz, values, rowshift, colindex to *this
    void SetValues(const INT row, const INT column, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowshift,
                   const std::vector<INT> colindex);

    //! get the row or column number of *this
    void GetSizes(INT &row, INT &col) const;

    //! get the row
    INT GetRow() const;

    //! get the column
    INT GetColumn() const;

    //! get this->nnz_
    INT Getnnz() const;

    //! get (*this)[i][j]
    DBL GetElem(const INT row, const INT column) const;

    //! get the whole jth-row elements in *this into VEC object
    std::vector<DBL> GetRow(const INT row) const;

    //! get the whole jth-column elements in *this into VEC object
    std::vector<DBL> GetColumn(const INT column) const;

    //! get the whole diagonal elements in *this into VEC object
    std::vector<DBL> GetDiag() const;

    //! zero all the elements
    void Zero();

    //! copy *this into mat
    void CopyTo(MAT &mat) const;

    //! *this = a * (*this)
    void Scale(const DBL a);

    //! *this = a * I + *this
    void Shift(const DBL a);

    //! ReturnedValue = *this * vec
    VEC MultVec(const VEC vec) const;

    //! transpose *this
    void Transpose();

    //! ReturnedValues = vec2 + transpose(*this) * vec1
    VEC MultTransposeAdd(const VEC vec1, const VEC vec2) const;

    //! *this = a * *this + b * mat
    void Add(const DBL a, const DBL b, const MAT mat);

    //! ReturnedValues = a * mat1 + b * mat2
    friend MAT
    Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2);

    //! *this = matl * matr
    friend MAT Mult(const MAT matl, const MAT matr);

    //! *this = *this * mat
    void MultLeft(const MAT mat);

    //! *this = mat * *this
    void MultRight(const MAT mat);

    //! convert the data CSR format to CSRx format
    friend MAT ConvertCSR(const INT row, const INT column, const INT nnz,
                          const std::vector<DBL> values,
                          const std::vector<INT> rowshift,
                          const std::vector<INT> colindex);

};

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/