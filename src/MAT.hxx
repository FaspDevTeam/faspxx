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

/*
 *  This class defines the basic MAT data structure and its basic operations. The
 *  CSRx data structure is an extension of the wellknown CSR sparse matrix format.
 *  The differences lie in the following two aspects:
 *      (1) Unlike the classical CSR format, the CSRx format requires the column
 *          indices in each row are in ascending order;
 *      (2) The CSRx format has a diagPtr array which stores the locations of the
 *          diagonal entries in each row.
 *  Note that the CSRx format stores the diagonal entries even if they are zero.
 *  Furthermore, it is compatible with all CSR subroutines!
 */

#ifndef __MAT_HEADER__      /*-- allow multiple inclusions --*/
#define __MAT_HEADER__      /**< indicate MAT.hxx has been included before */

#include <vector>
#include "faspxx.hxx"
#include "VEC.hxx"
#include "LOP.hxx"

/*! \class MAT
 *  \brief Sparse matrix class.
 */
class MAT : public LOP {

private:

    INT nnz;                  ///< number of nonzeros of the matrix.
    std::vector<DBL> values;  ///< nonzero entries, compressed row by row.
    std::vector<INT> colInd;  ///< column indices of the nonzero in values.
    std::vector<INT> rowPtr;  ///< pointers to the beginning of each row in values.
    std::vector<INT> diagPtr; ///< pointers to diagonal entries in values.

public:

    //------------------- Default Constructor Behavior -----------------------//
    // If "row == 0", "col ==0 " or "nnz == 0", set *this as an empty matrix. //
    // If these parameters can't form a CSRx data type, throw an exception.   //
    // If "values" are not assigned, the matrix is just a sparse structure.   //
    //------------------------------------------------------------------------//

    /// Default constructor.
    MAT() : nnz(0), values(0), colInd(0), rowPtr(0), diagPtr(0) {
        nrow = 0;
        mcol = 0;
    };

    /// Construct sparse matrix from a CSRx matrix.
    MAT(const INT& nrow, const INT& mcol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    /// Construct sparse matrix from a CSR matrix.
    MAT(const INT& nrow, const INT& mcol, const INT& nnz,
        const std::vector<DBL>& values, const std::vector<INT>& colInd,
        const std::vector<INT>& rowPtr);

    /// Construct sparsity structure from a CSR matrix.
    MAT(const INT& nrow, const INT& mcol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr);

    /// Construct sparsity structure from a CSRx matrix.
    MAT(const INT& nrow, const INT& mcol, const INT& nnz,
        const std::vector<INT>& colInd, const std::vector<INT>& rowPtr,
        const std::vector<INT>& diagPtr);

    /// Construct diagonal MAT matrix from a VEC object.
    explicit MAT(const VEC& v);

    /// Construct diagonal MAT matrix from a vector object.
    explicit MAT(const std::vector<DBL>& v);

    /// Clone from another MAT.
    MAT(const MAT& mat);

    /// Default destructor.
    ~MAT() = default;

    /// Overload = operator.
    MAT &operator=(const MAT& mat);

    /// Set values of the matrix with CSRx format.
    void SetValues(const INT& nrow, const INT& mcol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr);

    /// Set values of the matrix with CSR format.
    void SetValues(const INT& nrow, const INT& mcol, const INT& nnz,
                   const std::vector<DBL>& values, const std::vector<INT>& colInd,
                   const std::vector<INT>& rowPtr);

    /// Get number of nonzeros of the matrix.
    INT GetNNZ() const;

    /// Get the diagonal entries of the matrix.
    void GetDiag(std::vector<DBL>& v) const;

    /// Copy the matrix to another MAT object.
    void CopyTo(MAT& mat) const;

    /// Scale the matrix with a scalar.
    void Scale(const DBL a);

    /// Shift the matrix with a scalar matrix.
    void Shift(const DBL a);

    /// Set the matrix to a zero matrix.
    void Zero();

    /// Sparse matrix-vector multiplication.
    void Apply(const VEC& v, VEC& w) const;

    /// Transpose of the matrix.
    void Transpose();

    /// Compute transpose of A multiply by v1 plus v2.
    void MultTransposeAdd(const VEC& v1, const VEC& v2, VEC& v) const;

    /// Get the value of [i,j]-entry of the matrix
    DBL GetValue(const INT& row, const INT& col) const;

    /// Write an MAT matrix to a disk file in CSR format.
    friend void WriteCSR(char *filename, MAT mat);

    /// Write an MAT matrix to a disk file in MTX format.
    friend void WriteMTX(char *filename, MAT mat);

private:
    /// Form diagPtr according to colInd and rowPtr.
    void FormDiagPtr();

    /// Make the matrix empty.
    void Empty();

};

#endif /* end if for __MAT_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/