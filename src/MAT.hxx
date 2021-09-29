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

/*! Important Note:
 *-----------------------------------------------------------------------------------
 *  This class defines the basic MAT data structure and its basic operations. The
 *  CSRx data structure is an extension of the wellknown CSR sparse matrix format.
 *  The differences lie in the following two aspects:
 *
 *  1. Unlike the classical CSR format, the CSRx format requires the column indices
 *  in each row are in a rough ascending order (namely, the nonzero entries should be
 *  ordered as lower trig, diag, uppper trig);
 *  2. The CSRx format has a diagPtr array which stores the locations of the diagonal
 *  entries in each row.
 *
 *  Note that the CSRx format stores the diagonal entries even if they are zero.
 *  Furthermore, it is compatible with CSR subroutines!
 */

#ifndef __MAT_HEADER__ /*-- allow multiple inclusions --*/
#define __MAT_HEADER__ /**< indicate MAT.hxx has been included before */

// FASPXX header files
#include "Faspxx.hxx"
#include "LOP.hxx"
#include "VEC.hxx"

/*! \class MAT
 *  \brief Sparse matrix class.
 */
class MAT : public LOP
{

private:
    USI              nnz;     ///< number of nonzeros of the matrix.
    std::vector<DBL> values;  ///< nonzero entries, compressed row by row.
    std::vector<USI> colInd;  ///< column indices of the nonzero in values.
    std::vector<USI> rowPtr;  ///< pointers to the beginning of each row in values.
    std::vector<USI> diagPtr; ///< pointers to diagonal entries in values.

public:
    //------------------- Default Constructor Behavior -----------------------//
    // If "nrow == 0", "mcol ==0 " or "nnz == 0", set *this as empty matrix.  //
    // If these parameters can't form a CSRx data type, throw an exception.   //
    // If "values" are not assigned, the matrix is just a sparse structure.   //
    //------------------------------------------------------------------------//

    /// Default constructor.
    MAT()
        : nnz(0)
        , values(0)
        , colInd(0)
        , rowPtr(0)
        , diagPtr(0)
    {
        nrow = 0;
        mcol = 0;
    };

    /// Construct sparse matrix from a CSRx matrix.
    explicit MAT(const USI& nrow, const USI& mcol, const USI& nnz,
                 const std::vector<DBL>& values, const std::vector<USI>& colInd,
                 const std::vector<USI>& rowPtr, const std::vector<USI>& diagPtr);

    /// Construct sparse matrix from a CSR matrix.
    explicit MAT(const USI& nrow, const USI& mcol, const USI& nnz,
                 const std::vector<DBL>& values, const std::vector<USI>& colInd,
                 const std::vector<USI>& rowPtr);

    /// Construct sparsity structure from a CSR matrix.
    explicit MAT(const USI& nrow, const USI& mcol, const USI& nnz,
                 const std::vector<USI>& colInd, const std::vector<USI>& rowPtr);

    /// Construct sparsity structure from a CSRx matrix.
    explicit MAT(const USI& nrow, const USI& mcol, const USI& nnz,
                 const std::vector<USI>& colInd, const std::vector<USI>& rowPtr,
                 const std::vector<USI>& diagPtr);

    /// Construct diagonal MAT matrix from a VEC object.
    explicit MAT(const VEC& v);

    /// Construct diagonal MAT matrix from a vector object.
    explicit MAT(const std::vector<DBL>& v);

    /// Clone from another MAT.
    MAT(const MAT& mat);

    /// Default destructor.
    ~MAT() = default;

    /// Overload = operator.
    MAT& operator=(const MAT& mat);

    /// Set values of the matrix with CSRx format.
    void SetValues(const USI& nrow, const USI& mcol, const USI& nnz,
                   const std::vector<DBL>& values, const std::vector<USI>& colInd,
                   const std::vector<USI>& rowPtr, const std::vector<USI>& diagPtr);

    /// Set values of the matrix with CSR format.
    void SetValues(const USI& nrow, const USI& mcol, const USI& nnz,
                   const std::vector<DBL>& values, const std::vector<USI>& colInd,
                   const std::vector<USI>& rowPtr);

    /// Get number of nonzeros of the matrix.
    USI GetNNZ() const;

    /// Get the diagonal entries of *this and save them in a VEC object.
    void GetDiag(VEC& v) const;

    /// Get reciprocal diagonal entries and save them in a MAT object.
    void GetDiagInv(MAT& m) const;

    /// Get the lower triangular matrix.
    void GetLowerTri(MAT& lTri) const;

    /// Get the upper triangular matrix.
    void GetUpperTri(MAT& uTri) const;

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

    /// Residual b - Ax.
    void Residual(const VEC& b, const VEC& x, VEC& r) const;

    /// Form transpose of the matrix in place.
    void TransInPlace();

    /// Compute transpose of A multiply by v1 plus v2.
    void MultTransposeAdd(const VEC& v1, const VEC& v2, VEC& v) const;

    /// Get the value of [i,j]-entry of the matrix
    DBL GetValue(const USI& row, const USI& col) const;

    /// Get the values of the matrix
    double* GetValues() const;

    /// Get the row pointer of the matrix
    int* GetRowPtr() const;

    /// Get the column indices of the matrix
    int* GetColInd() const;

    /// *this = a * mat1 + b * mat2
    void Add(const DBL a, const MAT& mat1, const DBL b, const MAT& mat2);

    /// *this = matl * matr
    void Mult(const MAT& matl, const MAT& matr);

    /// *this = *this * mat
    void MultLeft(const MAT& mat);

    /// *this = mat * *this
    void MultRight(const MAT& mat);

    /// invmat = Inverse(*this)
    void Inverse(MAT& invmat) const;

    /// Write an MAT matrix to a disk file in CSR format.
    friend void WriteCSR(char* filename, MAT mat);

    /// Write an MAT matrix to a disk file in MTX format.
    friend void WriteMTX(char* filename, MAT mat);

private:
    /// Form diagPtr according to colInd and rowPtr.
    void FormDiagPtr();

    /// Make the matrix empty.
    void Empty();

    /// LUP decomposition
    void LUPDecomp(std::vector<DBL> A, std::vector<DBL>& L, std::vector<DBL>& U,
                   std::vector<USI>& P, USI N) const;

    /// LUP solver
    void LUPSolve(std::vector<DBL> L, std::vector<DBL> U, std::vector<USI> P,
                  std::vector<DBL> b, USI N, std::vector<DBL>& x) const;

    /// successor
    USI GetNext(USI i, USI m, USI n) const;

    /// precursor
    USI GetPre(USI i, USI m, USI n) const;

    /// Handle rings starting with i
    void MoveData(std::vector<DBL>& mtx, USI i, USI m, USI n) const;

    /// Transpose, i.e. cycle all rings
    void Rtranspose(std::vector<DBL>& mtx, USI m, USI n) const;

    /// LUP inversion (assemble each column x from each column B)
    void LUPSolveInverse(std::vector<DBL> A, USI N, std::vector<DBL>& inv_A) const;
};

/*! \class IdentityMatrix
 *  \brief Identity matrix.
 */
class IdentityMatrix : public MAT
{
public:
    /// Default constructor, all size.
    IdentityMatrix() = default;

    /// Constructor with fixed problem size.
    IdentityMatrix(USI size);

    /// Default destructor.
    ~IdentityMatrix() = default;

    /// Iterator for identity.
    void Apply(const VEC& b, VEC& x) const override;
};

#endif /* end if for __MAT_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Sep/25/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file, fix Doxygen        */
/*----------------------------------------------------------------------------*/