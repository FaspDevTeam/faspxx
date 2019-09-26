/*! \file MAT.hxx
 *  \brief Header file for the FASP++ Matrix class
 *  \author Kailei Zhang
 *  \date Sep/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __MAT_HEADER__      /*-- allow multiple inclusions --*/
#define __MAT_HEADER__      /**< indicate mat.hxx has been included before */

#include <vector>
#include "faspxx.hxx"
#include "VEC.hxx"

/*! \class MAT
 *  \brief Basic double matrix class in the CSRx data structure
 *
 *  This class defines the basic MAT data structure and its basic operations. We
 *  give two simple examples here as follows:
 *
 *  Example 1.
 *  \[
 *  \begin{pmatrix}
 *      1&  0&  2 \\
 *      0&  1&  0 \\
 *      3&  0&  0 \\
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
 *      1&  7&  2 \\
 *      0&  0&  0 \\
 *      3&  0&  4 \\
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

    INT nrow;   //! the row number of the matrix
    INT ncol;   //! the column number of the matrix
    INT nnz;    //! the number of nonzeros of the matrix

    //! values contains the nonzero entries of the matrix, compressed row by row
    std::vector<DBL> values;

    //! colInd contains the column indices of the nonzero as standard CSR
    std::vector<INT> colInd;

    //! rowPtr contains the pointers to the beginning of each row as standard CSR
    std::vector<INT> rowPtr;

    //! diagPtr contains the pointers to diagonal entries in values
    std::vector<INT> diagPtr;

public:

    //------------------- Default Constructor Behavior ----------------------//
    // If "row == 0" or "col ==0 " or "nnz == 0", set *this as empty matrix. //
    // If these parameters can't form a CSRx data type, throw an exception.  //
    // If "values" are not assigned, the matrix is just a sparse structure.  //
    //-----------------------------------------------------------------------//

    //! initial constructor
    MAT() : nrow(0), ncol(0), nnz(0),
            values(0), colInd(0), rowPtr(0), diagPtr(0) {};

    //! assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    //! assign nrow, ncol, nnz, values, colInd, rowPtr to *this, generate diagPtr
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr);

    //! assign nrow, ncol, nnz, colInd, rowPtr, diagPtr to *this (sparse structure)
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr,
        const std::vector<INT>& diagPtr);

    //! assign nrow, ncol, nnz, colInd, rowPtr to *this (sparse structure), generate diagPtr
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr);

    //! assign diagonal values to *this from a VEC
    MAT(const VEC& v);

    //! assign diagonal values to *this from a vector
    MAT(const std::vector<DBL>& vt);

    //! assign MAT object to *this
    MAT(const MAT& mat);

    //! overload equals operator
    MAT& operator=(const MAT& mat);

    //! default destructor
    ~MAT() {};

    //! assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
    void SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    //! assign nrow, ncol, nnz, values, rowPtr, colInd to *this
    void SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr);

    //! get row size
    INT GetRowSize() const;

    //! get column size
    INT GetColSize() const;

    //! get number of nonzeros
    INT GetNNZ() const;

    //! get (*this)[i][j]
    DBL GetValue(const INT& row, const INT& col) const;

    //! get the whole row-th row in *this into VEC object
    void GetRow(const INT& row, std::vector<DBL>& v) const;
    
    //! get the whole col-th column in *this into VEC object
    void GetCol(const INT& col, std::vector<DBL>& v) const;
    
    //! get the whole diagonal entries in *this into VEC object
    void GetDiag(std::vector<DBL>& v) const;

    //! copy *this to mat
    void CopyTo(MAT& mat) const;

    //! *this = a * (*this)
    void Scale(const DBL a);

    //! *this += a * I
    void Shift(const DBL a);

    //! set all the entries to zero
    void Zero();

    //! Return Value = *this * vec
    VEC MultVec(const VEC& vec) const;

    //! transpose *this // Todo: Check???
    void Transpose();

    //! *this = *this * mat
    void MultLeft(const MAT& mat);

    //! *this = mat * *this
    void MultRight(const MAT& mat);

    //! *this = a * *this + b * mat
    void Add(const DBL a, const DBL b, const MAT& mat);

    //! A'*vec1 + vec2 // Todo: Check???
    VEC MultTransposeAdd(const VEC& vec1, const VEC& vec2) const;

    //! a * mat1 + b * mat2 // Todo: Check???
    friend MAT Add(const DBL a, const MAT& mat1, const DBL b, const MAT& mat2);

    //! *this = matl * matr
    friend MAT Mult(const MAT& matl, const MAT& matr);

private:
    //! form diagonal pointer from colInd and rowPtr
    void FormDiagPtr(std::vector<INT>& diagPtr);

    //! make an empty matrix
    void Empty();
};

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/