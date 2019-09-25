/*! \file MAT.hxx
 *  \brief Header file for the FASP++ Matrix class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __MAT_HEADER__      /*-- allow multiple inclusions --*/
#define __MAT_HEADER__      /**< indicate mat.hxx has been included before */

#include "VEC.hxx"

/*! \class MAT
 *  \brief Basic double matrix class in the CSRx data structure
 *  \author Kailei Zhang
 *  \date Sep/25/2019
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

    INT row;    //! the row number of the matrix
    INT col;    //! the column number of the matrix
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
    MAT() : row(0), col(0), nnz(0),
            values(0), colInd(0), rowPtr(0), diagPtr(0) {};

    //! assign row, col, nnz, values, rowPtr, colInd, diagPtr to *this
    MAT(const INT row, const INT col, const INT nnz,
        const std::vector<DBL> values, const std::vector<INT> rowPtr, 
        const std::vector<INT> colInd, const std::vector<INT> diagPtr);

    //! assign row, col, nnz, values, rowPtr, colInd to *this, generate diagPtr
    MAT(const INT row, const INT col, const INT nnz,
        const std::vector<DBL> values, const std::vector<INT> rowPtr,
        const std::vector<INT> colInd);

    //! assign row, col, nnz, rowPtr, colInd, diagPtr to *this (sparse structure)
    MAT(const INT row, const INT col, const INT nnz,
        const std::vector<INT> rowPtr, const std::vector<INT> colInd,
        const std::vector<INT> diagPtr);

    //! assign row, col, nnz, rowPtr, colInd to *this (sparse structure), generate diagPtr
    MAT(const INT row, const INT col, const INT nnz,
        const std::vector<INT> rowPtr, const std::vector<INT> colInd);

    //! assign diagonal values to *this from a VEC
    MAT(VEC &vec);

    //! assign diagonal values to *this from a vector
    MAT(const std::vector<DBL> &vect);

    //! assign MAT object to *this
    MAT(const MAT &mat);

    //! overload equals operator
    MAT &operator=(const MAT &mat);

    //! default destructor
    ~MAT() {};

    //! assign row, col, nnz, values, colInd, rowPtr, diagPtr to *this
    void SetValues(const INT row, const INT col, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowPtr,
                   const std::vector<INT> colInd, const std::vector<INT> diagPtr);

    //! assign row, col, nnz, values, rowPtr, colInd to *this
    void SetValues(const INT row, const INT col, const INT nnz,
                   const std::vector<DBL> values, const std::vector<INT> rowPtr,
                   const std::vector<INT> colInd);

    //! get row size
    INT GetRowSize() const;

    //! get column size
    INT GetColSize() const;

    //! get number of nonzeros
    INT GetNNZ() const;

    //! get (*this)[i][j]
    DBL GetElem(const INT row, const INT col) const;

    //! get the whole i-th row in *this into VEC object
    std::vector<DBL> GetRow(const INT i) const; // Todo: Needed???
    
    //! get the whole j-th column in *this into VEC object
    std::vector<DBL> GetCol(const INT j) const; // Todo: Needed???

    //! get the whole diagonal entries in *this into VEC object
    std::vector<DBL> GetDiag() const;

    //! set all the entries to zero 
    void Zero();

    //! copy *this to mat
    void CopyTo(MAT &mat) const;

    //! *this = a * (*this)
    void Scale(const DBL a);

    //! *this += a * I
    void Shift(const DBL a);

    //! ReturnedValue = *this * vec
    VEC MultVec(const VEC vec) const;

    //! transpose *this
    void Transpose();

    //! A'*vec1 + vec2 // Todo: Check???
    VEC MultTransposeAdd(const VEC vec1, const VEC vec2) const;

    //! *this = a * *this + b * mat
    void Add(const DBL a, const DBL b, const MAT mat);

    //! a * mat1 + b * mat2 // Todo: Check???
    friend MAT Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2);

    //! *this = matl * matr
    friend MAT Mult(const MAT matl, const MAT matr);

    //! *this = *this * mat
    void MultLeft(const MAT mat);

    //! *this = mat * *this
    void MultRight(const MAT mat);

};

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/