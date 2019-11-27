/*! \file    MAT.hxx
 *  \brief   Matrix class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Sep/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __MAT_HEADER__      /*-- allow multiple inclusions --*/
#define __MAT_HEADER__      /**< indicate MAT.hxx has been included before */

#include <vector>
#include "faspxx.hxx"
#include "VEC.hxx"
#include "LOP.hxx"

/*! \class MAT
 *  \brief Basic double matrix class in the CSRx data structure
 *
 *  This class defines the basic MAT data structure and its basic operations. The
 *  CSRx data structure is an extension of the wellknown CSR sparse matrix format.
 *  The differences lie in the following two aspects:
 *      (1) Unlike the classical CSR format, the CSRx format requires the column
 *          indices in each row are in ascending order;
 *      (2) The CSRx format has a diagPtr array which stores the locations of the
 *          diagonal entries in each row.
 *
 *  We give two simple examples here:
 *
 *  Example 1.
 *  \f{equation*}{
 *  \begin{pmatrix}
 *      1  &  0  &  2 \\
 *      0  &  1  &  0 \\
 *      3  &  0  &  0 \\
 *  \end{pmatrix}
 *  }
 *  and
 *      nnz = 5, row = 3, col = 3,
 *      values = { 1, 2, 1, 3, 0 },
 *      colInd = { 0, 2, 1, 0, 2 },
 *      rowPtr = { 0, 2, 3, 5 },
 *      diagPtr = { 0, 2, 4 }.
 *
 *  Example 2.
 *  \f{equation*}{
 *  \begin{pmatrix}
 *      1&  7&  2 \\
 *      0&  0&  0 \\
 *      3&  0&  4 \\
 *  \end{pmatrix}
 *  }
 *  and
 *      nnz = 6, row = 3, col = 3,
 *      values = { 1, 7, 2, 0, 3, 4 },
 *      colInd = { 0, 1, 2, 1, 0, 2 },
 *      rowPtr = { 0, 3, 4, 6 },
 *      diagPtr = { 0, 3, 5 }.
 *
 *  Note that the CSRx format stores the diagonal entries even if they are zero.
 *  Furthermore, it is compatible with all CSR subroutines!
 */
class MAT : public LOP {

private:

    INT nnz;    ///< number of "nonzeros" of the matrix

    /// nonzero entries of the matrix, compressed row by row
    std::vector<DBL> values;

    /// column indices of the nonzero in values
    std::vector<INT> colInd;

    /// pointers to the beginning of each row in values
    std::vector<INT> rowPtr;

    /// pointers to diagonal entries in values
    std::vector<INT> diagPtr;

public:

    //------------------- Default Constructor Behavior -----------------------//
    // If "row == 0", "col ==0 " or "nnz == 0", set *this as an empty matrix. //
    // If these parameters can't form a CSRx data type, throw an exception.   //
    // If "values" are not assigned, the matrix is just a sparse structure.   //
    //------------------------------------------------------------------------//

    /// Default constructor
    MAT() : nnz(0),values(0), colInd(0), rowPtr(0), diagPtr(0) {
        nrow=0;
        ncol=0;
    };

    /// Assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    /// Assign nrow, ncol, nnz, values, colInd, rowPtr to *this and generate diagPtr
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr);

    /// Assign nrow, ncol, nnz, colInd, rowPtr to *this and generate diagPtr
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr);

    /// Assign nrow, ncol, nnz, colInd, rowPtr, diagPtr to *this
    MAT(const INT& nrow, const INT& ncol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr,
        const std::vector<INT>& diagPtr);

    /// Assign diagonal values from a VEC to *this
    explicit MAT(const VEC& v);

    /// Assign diagonal values from a vector to *this
    explicit MAT(const std::vector<DBL>& vt);

    /// Assign MAT object to *this
    MAT(const MAT& mat);

    /// Overload = operator
    MAT &operator=(const MAT& mat);

    /// Default destructor
    ~MAT() = default;

    /// Set values of nrow, ncol, nnz, values, colInd, rowPtr, diagPtr
    void SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    /// Set values of nrow, ncol, nnz, values, rowPtr, colInd
    void SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr);

    /// Get number of nonzeros
    INT GetNNZ() const;

    /// Get the diagonal entries of *this to a VEC object
    void GetDiag(std::vector<DBL>& v) const;

    /// Copy *this to another MAT object, mat
    void CopyTo(MAT& mat) const;

    /// Scale *this *= a
    void Scale(const DBL a);

    /// Shift *this += a * I
    void Shift(const DBL a);

    /// Set all the entries to zero without changing matrix size
    void Zero();

    /// Transpose *this
    void Transpose();

    /// Return VEC = *this * vec
    void Apply(const VEC& v, VEC& w) const;

    /// v= A'*v1 + v2
    void MultTransposeAdd(const VEC& v1, const VEC& v2, VEC& v) const;

    /// Write CSR format data to a disk file
    friend void WriteCSR(char *filename, MAT mat);

    /// Write MTX format data to a disk file
    friend void WriteMTX(char *filename, MAT mat);

private:
    /// Form diagonal pointer using colInd and rowPtr
    void FormDiagPtr();

    /// Empty a matrix
    void Empty();

public: // Todo: Are we going to use the following functions??? -zcs

    /// r = y - A * x: y minus A times x
    void YMAX(const VEC& y, const VEC& x, VEC& r) const;

    /// mat = a * mat1 + b * mat2
    friend void Add(const DBL a, const MAT& mat1, const DBL b, const MAT& mat2,
                    MAT& mat);

    /// *this = a * *this + b * mat
    void Add(const DBL a, const DBL b, const MAT& mat);

    /// Get (*this)[i][j]
    DBL GetValue(const INT& row, const INT& col) const;

    /// Get the whole row-th row in *this into VEC object
    void GetRow(const INT& row, std::vector<DBL>& v) const;

    /// Get the whole col-th column in *this into VEC object
    void GetCol(const INT& col, std::vector<DBL>& v) const;

    /// mat = matl * matr
    friend void Mult(const MAT& matl, const MAT& matr, MAT& mat);

    /// *this = mat * *this
    void MultLeft(const MAT& mat);

    /// *this = *this * mat
    void MultRight(const MAT& mat);

};

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/