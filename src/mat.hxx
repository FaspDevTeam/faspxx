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

/*! \class MAT
 *  \brief Basic double matrix class in the CSRx data structure
 *
 *  This class defines the basic MAT data structure and its basic operations. We
 *  give two simple examples here as follows:
 *
 *  Example 1.
 *  \[
 *  \begin{pmatrix}
 *      1 & 0 & 2 \\
 *      0 & 1 & 0 \\
 *      3 & 0 & 0 \\
 *  \end{pmatrix}
 *  \]
 *  and
 *  nnz = 5, row = 3, col = 3,
 *  values = { 1, 2, 1, 3, 0 },
 *  colInd = { 0, 2, 1, 0, 2 },
 *  rowPtr = { 0, 2, 3, 5 },
 *  diagPtr = { 0, 2, 4 }.
 *
 *  Example 2.
 *  \[
 *  \begin{pmatrix}
 *      1 & 7 & 2 \\
 *      0 & 0 & 0 \\
 *      3 & 0 & 4 \\
 *  \end{pmatrix}
 *  \]
 *  and
 *  nnz = 6, row = 3, col = 3,
 *  values = { 1, 7, 2, 0, 3, 4 },
 *  colInd = { 0, 1, 2, 1, 0, 2 },
 *  rowPtr = { 0, 3, 4, 6 },
 *  diagPtr = { 0, 3, 5 }.
 */
class MAT {

private:

    INT nnz;    //! the number of nonzeros of the matrix
    INT row;    //! the row number of the matrix
    INT col;    //! the column number of the matrix

    //! values contains the nonzero entries of the matrix, compressed row by row
    std::vector<DBL> values;

    //! colInd contains the column indices of the nonzero as standard CSR
    std::vector<INT> colInd;

    //! rowPtr contains the pointers to the beginning of each row as standard CSR
    std::vector<INT> rowPtr;

    //! diagPtr contains the pointers to diagonal entries in values
    std::vector<INT> diagPtr;

public:

    //! initial contructed function
    MAT() : row(0), col(0), nnz(0), values(0), rowPtr(0),
            colInd(0), diagPtr(0) {};

    /**
     * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
     * if these parameters can't form a CSRx data type, throw an exception. or set these
     * parameters to *this.
     */
    //! assign row, col, nnz, values, rowPtr, colInd, diagPtr to *this
    MAT(const INT row, const INT col, const INT nnz,
        const std::vector<DBL> values, const std::vector<INT> rowPtr, 
        const std::vector<INT> colInd, const std::vector<INT> diagPtr);

    /**
     * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
     * if these parameters can't form a CSRx data type, throw an exception. or set these
     * parameters to *this.
     * attention : all the CSRx data 's values are one
     */
    //! assign row, col, nnz, rowPtr, colInd, diagPtr to *this
    MAT(const INT row, const INT col, const INT nnz, const std::vector<INT>
    rowPtr, const std::vector<INT> colInd, const std::vector<INT>
        diagPtr);

    /**
     * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
     * if these parameters can't form a CSRx data type, throw an exception. or set these
     * parameters to *this.
     */
    //! assign row, col, nnz, values, rowPtr, colInd to *this
    MAT(const INT row, const INT col, const INT nnz, const std::vector<DBL>
    values, const std::vector<INT> rowPtr, const std::vector<INT>
        colInd);

    /**
     * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
     * if these parameters can't form a CSRx data type, throw an exception. or set these
     * parameters to *this.
     * attention : all the elements in the CSRx 's values are one
     */
    //! assign row, col, nnz, rowPtr, colInd to *this
    MAT(const INT row, const INT col, const INT nnz, const std::vector<INT>
    rowPtr, const std::vector<INT> colInd);

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

    //! assign row, col, nnz, values, rowPtr, colInd to *this
    void SetValues(const INT row, const INT col, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowPtr,
                   const std::vector<INT> colInd, const std::vector<INT> diagPtr);

    //! assign row, col, nnz, values, rowPtr, colInd to *this
    void SetValues(const INT row, const INT col, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowPtr,
                   const std::vector<INT> colInd);

    //! get the row or column number of *this
    void GetSizes(INT &row, INT &col) const;

    //! get row size
    INT GetRowSize() const;

    //! get column size
    INT GetColSize() const;

    //! get number of nonzeros
    INT GetNNZ() const;

    /**
     * if "row < 0" or "row > this->row" or "col < 0" or "col >=this->coliumn"
     * happens, throw an exception. In other cases,it is normally dealt.
     */
    //! get (*this)[i][j]
    DBL GetElem(const INT row, const INT col) const;

    /**
     * if "row < 0" or "row > this->row" happens,
     * throw an exception. In other cases,it is normally dealt.
     */
    //! get the whole jth-row elements in *this into VEC object
    std::vector<DBL> GetRow(const INT row) const;

    /**
     * if "col < 0" or "col > this->row" happens,
     * throw an exception. In other cases,it is normally dealt.
     */
    //! get the whole j-th column in *this into VEC object
    std::vector<DBL> GetCol(const INT j) const;

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

    /**
     * if *this 's column dimension does not match "vec" 's dimension, throw an
     * exception.
     */
    //! vec_b = *this * vec_x
    VEC MultVec(const VEC vec) const;

    //! transpose *this
    void Transpose();

    /**
     * if "this->row" 's dimension is not equal to "vec1" 's or "this->col" 's
     * dimension is not equal to "vec2" 's . throw an exception.
     */
    //! vec3 = vec2 + transpose(*this) * vec1
    VEC MultTransposeAdd(const VEC vec1, const VEC vec2) const;

    /**
     * if these matrices both 's dimensions do not match, throw an exception.
     */
    //! *this = a * *this + b * mat
    void Add(const DBL a, const DBL b, const MAT mat);

    /**
     * if these matrices both 's dimensions do not match, throw an exception.
     */
    //! mat3 = a * mat1 + b * mat2
    friend MAT
    Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2);

    //! mat3 = mat1 * mat2
    friend MAT Mult2(const MAT matl, const MAT matr);

    //! *this = *this * mat
    void MultLeft(const MAT mat);

    //! *this = mat * *this
    void MultRight(const MAT mat);

    /**
     * if "row == 0" or "col == 0" or "nnz == 0" happens, set the returned values
     * by default constructor. if these data are not CSRx, set the returned values by
     * default constructor and throw an exception.
     */
    //! convert the data CSR format to CSRx format
    friend MAT ConvertCSR(const INT row, const INT col, const INT nnz,
                          const std::vector<DBL> values,
                          const std::vector<INT> rowPtr,
                          const std::vector<INT> colInd);

};

/**
 * if "row == 0" or "col == 0" or "nnz == 0" happens, set the returned values
 * by default constructor. if these data are not CSRx, set the returned values by
 * default constructor and throw an exception.
 */
//! convert the data CSR format to CSRx format
MAT ConvertCSR(const INT row, const INT col, const INT nnz,
               const std::vector<DBL> values,
               const std::vector<INT> rowPtr,
               const std::vector<INT> colInd);

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/