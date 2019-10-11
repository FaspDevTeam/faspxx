/*! \file MAT.hxx
 *  \brief Matrix class declaration
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

    INT nrow;   ///< number of rows
    INT ncol;   ///< number of columns
    INT nnz;    ///< number of nonzeros

    /// nonzero entries of the matrix, compressed row by row
    std::vector<DBL> values;

    /// column indices of the nonzero in values
    std::vector<INT> colInd;

    /// pointers to the beginning of each row in values
    std::vector<INT> rowPtr;

    /// pointers to diagonal entries in values
    std::vector<INT> diagPtr;

public:

    //------------------- Default Constructor Behavior ----------------------//
    // If "row == 0" or "col ==0 " or "nnz == 0", set *this as empty matrix. //
    // If these parameters can't form a CSRx data type, throw an exception.  //
    // If "values" are not assigned, the matrix is just a sparse structure.  //
    //-----------------------------------------------------------------------//

    /// Default constructor
    MAT() : nrow(0), ncol(0), nnz(0),
            values(0), colInd(0), rowPtr(0), diagPtr(0) {};

    /// Assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    /// Assign nrow, ncol, nnz, values, colInd, rowPtr to *this, generate diagPtr
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr);

    /// Assign nrow, ncol, nnz, colInd, rowPtr, diagPtr to *this (sparse structure)
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr,
        const std::vector<INT>& diagPtr);

    /// Assign nrow, ncol, nnz, colInd, rowPtr to *this (sparse structure), generate diagPtr
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr);

    /// Assign diagonal values to *this from a VEC
    explicit MAT(const VEC& v);

    /// Assign diagonal values to *this from a vector
    explicit MAT(const std::vector<DBL>& vt);

    /// Assign MAT object to *this
    MAT(const MAT& mat);

    /// Overload = operator
    MAT& operator=(const MAT& mat);

    /// Default destructor
    ~MAT() = default;

    /// Assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
    void SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    /// Assign nrow, ncol, nnz, values, rowPtr, colInd to *this
    void SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr);

    /// Get row size
    INT GetRowSize() const;

    /// Get column size
    INT GetColSize() const;

    /// Get number of nonzeros
    INT GetNNZ() const;

    /// Get (*this)[i][j]
    DBL GetValue(const INT& row, const INT& col) const;

    /// Get the whole row-th row in *this into VEC object
    void GetRow(const INT& row, std::vector<DBL>& v) const;
    
    /// Get the whole col-th column in *this into VEC object
    void GetCol(const INT& col, std::vector<DBL>& v) const;
    
    /// Get the whole diagonal entries in *this into VEC object
    void GetDiag(std::vector<DBL>& v) const;

    /// Copy *this to mat
    void CopyTo(MAT& mat) const;

    /// Scale *this *= a
    void Scale(const DBL a);

    /// Shift *this += a * I
    void Shift(const DBL a);

    /// Set all the entries to zero without changing matrix size
    void Zero();

    /// Transpose *this
    void Transpose();

    /// *this = mat * *this
    void MultLeft(const MAT& mat);

    /// *this = *this * mat
    void MultRight(const MAT& mat);

    /// Return MAT = a * mat1 + b * mat2
    friend MAT Add(const DBL a, const MAT& mat1, const DBL b, const MAT& mat2,
            MAT &mat);

    /// *this = a * *this + b * mat
    void Add(const DBL a, const DBL b, const MAT& mat);

    /// Return VEC = *this * vec
    void MultVec(const VEC& v,VEC& w) const;

    /// Return VEC = A'*v1 + v2
    VEC MultTransposeAdd(const VEC& v1, const VEC& v2) const;

    /// *this = matl * matr
    friend MAT Mult(const MAT& matl, const MAT& matr);

    /// write CSR format data to the disk
    friend void WriteCSR(char *filename,MAT mat);

private:
    /// Form diagonal pointer using colInd and rowPtr
    void FormDiagPtr(std::vector<INT>& diagPtr);

    /// Empty a matrix
    void Empty();
};

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/