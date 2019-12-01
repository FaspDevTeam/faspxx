/*! \file    MAT.cxx
 *  \brief   Definition of the default matrix class
 *  \author  Kailei Zhang
 *  \date    Sep/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <fstream>
#include "MAT.hxx"
#include "MATUtil.hxx"

/*!
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

/// Assign nrow, mcol, nnz, values, colInd, rowPtr, diagPtr to *this
MAT::MAT(const INT& nrow, const INT& mcol, const INT& nnz,
         const std::vector<DBL>& values, const std::vector<INT>& colInd,
         const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;
    this->nnz = nnz;

    this->values = values;
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->diagPtr = diagPtr;
}

/// Assign nrow, mcol, nnz, values, colInd, rowPtr to *this and generate diagPtr
MAT::MAT(const INT& nrow, const INT& mcol, const INT& nnz,
         const std::vector<DBL>& values, const std::vector<INT>& colInd,
         const std::vector<INT>& rowPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;
    this->nnz = nnz;

    this->values = values;
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->FormDiagPtr();
}

/// Assign nrow, mcol, nnz, colInd, rowPtr to *this and generate diagPtr
MAT::MAT(const INT& nrow, const INT& mcol, const INT& nnz,
         const std::vector<INT>& colInd, const std::vector<INT>& rowPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;
    this->nnz = nnz;

    this->values.resize(0);
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->FormDiagPtr();
}

/// Assign nrow, mcol, nnz, colInd, rowPtr, diagPtr to *this
MAT::MAT(const INT& nrow, const INT& mcol, const INT& nnz,
         const std::vector<INT>& colInd, const std::vector<INT>& rowPtr,
         const std::vector<INT>& diagPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;
    this->nnz = nnz;

    this->values.resize(0);
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->diagPtr = diagPtr;
}

/// Assign diagonal values from a VEC to *this
MAT::MAT(const VEC& v) {
    INT size = v.GetSize();

    if (size == 0) {
        this->Empty();
        return;
    }

    INT *p;
    try {
        p = new INT[size + 1];
    } catch (std::bad_alloc &ex) {
        this->nrow = 0;
        this->mcol = 0;
        this->nnz = 0;
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }

    this->nrow = size;
    this->mcol = size;
    this->nnz = size;

    this->values.resize(size);
    this->colInd.resize(size);
    this->rowPtr.resize(size + 1);
    this->diagPtr.resize(size);

    // Set values from v
    for (INT j = 0; j < size; ++j) this->values[j] = v.values[j];

    // Set colInd to {0, 1, ..., size-1}
    for (INT j = 0; j <= size; ++j) p[j] = j;
    this->colInd.assign(p, p + size);

    // Set rowPtr to {0, 1, ..., size}
    this->rowPtr.assign(p, p + size + 1);

    // Set diagPtr to {0, 1, ..., size-1}
    this->diagPtr.assign(p, p + size);

    delete[] p;
}

/// Assign diagonal values from a vector to *this
MAT::MAT(const std::vector<DBL>& vt) {
    const INT size = vt.size();

    if (size == 0) {
        this->Empty();
        return;
    }

    INT *p;
    try {
        p = new INT[size + 1];
    } catch (std::bad_alloc& ex) {
        this->nrow = 0;
        this->mcol = 0;
        this->nnz = 0;
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }

    this->nrow = size;
    this->mcol = size;
    this->nnz = size;

    this->values.resize(size);
    this->colInd.resize(size);
    this->rowPtr.resize(size + 1);
    this->diagPtr.resize(size);

    // Set values from vt
    this->values.assign(vt.begin(), vt.begin() + size);

    // Set colInd to {0, 1, ..., size-1}
    for (INT j = 0; j <= size; ++j) p[j] = j;
    this->colInd.assign(p, p + size);

    // Set rowPtr to {0, 1, ..., size}
    this->rowPtr.assign(p, p + size + 1);

    // Set diagPtr to {0, 1, ..., size-1}
    this->diagPtr.assign(p, p + size);

    delete[] p;
}

/// Assign MAT object to *this
MAT::MAT(const MAT& mat) {
    this->nrow = mat.nrow;
    this->mcol = mat.mcol;
    this->nnz = mat.nnz;

    this->values = mat.values;
    this->diagPtr = mat.diagPtr;
    this->colInd = mat.colInd;
    this->rowPtr = mat.rowPtr;
}

/// Overload = operator
MAT &MAT::operator=(const MAT& mat) {
    this->nrow = mat.nrow;
    this->mcol = mat.mcol;
    this->nnz = mat.nnz;

    this->values = mat.values;
    this->colInd = mat.colInd;
    this->rowPtr = mat.rowPtr;
    this->diagPtr = mat.diagPtr;

    return *this;
}

/// Set values of nrow, mcol, nnz, values, colInd, rowPtr, diagPtr
void MAT::SetValues(const INT& nrow, const INT& mcol, const INT& nnz,
                    const std::vector<DBL>& values, const std::vector<INT>& colInd,
                    const std::vector<INT>& rowPtr,
                    const std::vector<INT>& diagPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;
    this->nnz = nnz;

    this->values = values;
    this->rowPtr = rowPtr;
    this->colInd = colInd;
    this->diagPtr = diagPtr;
}

/// Set values of nrow, mcol, nnz, values, rowPtr, colInd
void MAT::SetValues(const INT& nrow, const INT& mcol, const INT& nnz,
                    const std::vector<DBL>& values, const std::vector<INT>& colInd,
                    const std::vector<INT>& rowPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;

    this->nnz = nnz;

    this->rowPtr = rowPtr;
    this->colInd = colInd;
    this->values = values;

    this->FormDiagPtr();

}

/// Get number of nonzeros
INT MAT::GetNNZ() const {
    return this->nnz;
}

/// Get the whole diagonal entries in *this into VEC object
void MAT::GetDiag(std::vector<DBL>& v) const {
    INT len = this->nrow > this->mcol ? this->mcol : this->nrow;
    v.resize(len);

    if (this->values.size() != 0) {
        for (INT j = 0; j < len; ++j)
            v[j] = this->values[this->diagPtr[j]];
    } else {
        v.assign(len, 1.0);
    }
}

/// Copy *this to mat
void MAT::CopyTo(MAT& mat) const {
    mat = *this;
}

/// Scale *this *= a
void MAT::Scale(const DBL a) {
    if (this->values.empty()) {
        return; // MAT is a sparse structure!!!
    }

    for (INT j = 0; j < this->nnz; ++j) this->values[j] *= a;
}

/// Shift *this += a * I.
void MAT::Shift(const DBL a) {
    if (this->values.empty()) {
        return; // MAT is a sparse structure!!!
    }

    for (INT j = 0; j < this->diagPtr.size(); ++j)
        this->values[this->diagPtr[j]] += a;
}

/// Set all the entries to zero without changing matrix size
void MAT::Zero() {
    for (INT j = 0; j < this->nnz; ++j) values[j] = 0.0;
}

/// Transpose *this
void MAT::Transpose() {
    const INT n = this->nrow, m = this->mcol, nnz = this->nnz;
    INT i, j, k, p;

    MAT tmp;
    tmp.nrow = this->mcol;
    tmp.mcol = this->nrow;
    tmp.nnz = this->nnz;

    try {
        tmp.rowPtr.resize(this->mcol + 1);
        tmp.colInd.resize(nnz);
    } catch (std::bad_alloc &ex) {
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }


    if (this->values.size()) {
        try {
            tmp.values.resize(nnz);
        } catch (std::bad_alloc &ex) {
            throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
        }
    } else {
        tmp.values.resize(0);
    }

    for (INT j = 0; j < nnz; ++j) {
        i = this->colInd[j];
        if (i < m - 1) ++tmp.rowPtr[i + 2];
    }

    for (i = 2; i <= m; ++i) tmp.rowPtr[i] += tmp.rowPtr[i - 1];

    if (this->values.size()) {
        for (i = 0; i < n; ++i) {
            INT begin = this->rowPtr[i];
            for (p = begin; p < this->rowPtr[i + 1]; ++p) {
                j = this->colInd[p] + 1;
                k = tmp.rowPtr[j];
                tmp.colInd[k] = i;
                tmp.values[k] = this->values[p];
                tmp.rowPtr[j] = k + 1;
            }
        }
    } else {
        for (i = 0; i < n; ++i) {
            INT begin = this->rowPtr[i];
            for (p = begin; p < this->rowPtr[i + 1]; ++p) {
                j = this->colInd[p] + 1;
                k = tmp.rowPtr[j];
                tmp.colInd[k] = i;
                tmp.rowPtr[j] = k + 1;
            }
        }
    }

    tmp.FormDiagPtr();
    this->operator=(tmp);
}

/// w = *this * v.
void MAT::Apply(const VEC& v, VEC& w) const {

    INT begin, i, k;

    if (this->values.size() > 0) { // Regular sparse matrix
        for (i = 0; i < this->nrow; ++i) {
            begin = this->rowPtr[i];
            switch (this->rowPtr[i + 1] - begin) {
                case 4:
                    w.values[i] = this->values[begin]
                                  * v.values[this->colInd[begin]];
                    w.values[i] += this->values[begin + 1]
                                   * v.values[this->colInd[begin + 1]];
                    w.values[i] += this->values[begin + 2]
                                   * v.values[this->colInd[begin + 2]];
                    w.values[i] += this->values[begin + 3]
                                   * v.values[this->colInd[begin + 3]];
                    break;
                case 5:
                    w.values[i] = this->values[begin]
                                  * v.values[this->colInd[begin]];
                    w.values[i] += this->values[begin + 1]
                                   * v.values[this->colInd[begin + 1]];
                    w.values[i] += this->values[begin + 2]
                                   * v.values[this->colInd[begin + 2]];
                    w.values[i] += this->values[begin + 3]
                                   * v.values[this->colInd[begin + 3]];
                    w.values[i] += this->values[begin + 4]
                                   * v.values[this->colInd[begin + 4]];
                    break;
                case 6:
                    w.values[i] = this->values[begin]
                                  * v.values[this->colInd[begin]];
                    w.values[i] += this->values[begin + 1]
                                   * v.values[this->colInd[begin + 1]];
                    w.values[i] += this->values[begin + 2]
                                   * v.values[this->colInd[begin + 2]];
                    w.values[i] += this->values[begin + 3]
                                   * v.values[this->colInd[begin + 3]];
                    w.values[i] += this->values[begin + 4]
                                   * v.values[this->colInd[begin + 4]];
                    w.values[i] += this->values[begin + 5]
                                   * v.values[this->colInd[begin + 5]];
                    break;
                default:
                    w.values[i] =
                            this->values[begin] * v.values[this->colInd[begin]];
                    for (k = begin + 1; k < this->rowPtr[i + 1]; ++k)
                        w.values[i] += this->values[k] * v.values[this->colInd[k]];
            }
        }
    } else { // Only sparse structure
        for (i = 0; i < this->nrow; ++i) {
            begin = this->rowPtr[i];
            switch (this->rowPtr[i + 1] - begin) {
                case 4:
                    w.values[i] = v.values[this->colInd[begin]];
                    w.values[i] += v.values[this->colInd[begin + 1]];
                    w.values[i] += v.values[this->colInd[begin + 2]];
                    w.values[i] += v.values[this->colInd[begin + 3]];
                    break;
                case 5:
                    w.values[i] = v.values[this->colInd[begin]];
                    w.values[i] += v.values[this->colInd[begin + 1]];
                    w.values[i] += v.values[this->colInd[begin + 2]];
                    w.values[i] += v.values[this->colInd[begin + 3]];
                    w.values[i] += v.values[this->colInd[begin + 4]];
                    break;
                case 6:
                    w.values[i] = v.values[this->colInd[begin]];
                    w.values[i] += v.values[this->colInd[begin + 1]];
                    w.values[i] += v.values[this->colInd[begin + 2]];
                    w.values[i] += v.values[this->colInd[begin + 3]];
                    w.values[i] += v.values[this->colInd[begin + 4]];
                    w.values[i] += v.values[this->colInd[begin + 5]];
                    break;
                default:
                    w.values[i] = v.values[this->colInd[begin]];
                    for (k = begin + 1; k < this->rowPtr[i + 1]; ++k)
                        w.values[i] += v.values[this->colInd[k]];
            }
        }
    } // end if values.size > 0

}

/// v = A'*v1 + v2
void MAT::MultTransposeAdd(const VEC& v1, const VEC& v2, VEC& v) const {

    const INT n = this->nrow, m = this->mcol, nnz = this->nnz;
    INT i, j, k, p;

    MAT tmp;
    tmp.nrow = m;
    tmp.mcol = n;
    tmp.nnz = nnz;

    try {
        tmp.rowPtr.resize(m + 1);
        tmp.colInd.resize(nnz);
    } catch (std::bad_alloc &ex) {
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }

    if (this->values.size()) {
        try {
            tmp.values.resize(nnz);
        } catch (std::bad_alloc &ex) {
            throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
        }
    } else {
        tmp.values.resize(0);
    }

    for (j = 0; j < nnz; ++j) {
        i = this->colInd[j];
        if (i < m - 1) ++tmp.rowPtr[i + 2];
    }

    for (i = 2; i <= m; ++i)
        tmp.rowPtr[i] += tmp.rowPtr[i - 1];

    if (this->values.size()) {
        for (i = 0; i < n; ++i) {
            INT begin = this->rowPtr[i];
            for (p = begin; p < this->rowPtr[i + 1]; ++p) {
                j = this->colInd[p] + 1;
                k = tmp.rowPtr[j];
                tmp.colInd[k] = i;
                tmp.values[k] = this->values[p];
                tmp.rowPtr[j] = k + 1;
            }
        }
    } else {
        for (i = 0; i < n; ++i) {
            INT begin = this->rowPtr[i];
            for (p = begin; p < this->rowPtr[i + 1]; ++p) {
                j = this->colInd[p] + 1;
                k = tmp.rowPtr[j];
                tmp.colInd[k] = i;
                tmp.rowPtr[j] = k + 1;
            }
        }
    }

    v = v2;

    INT begin;
    for (i = 0; i < tmp.nrow; ++i) {
        begin = tmp.rowPtr[i];
        for (j = begin; j < this->rowPtr[i + 1]; ++j)
            v.values[i] += v1.values[tmp.colInd[j]] * tmp.values[j];
    }

}

/// Write CSR format data to a disk file
void WriteCSR(char *filename, MAT mat) {
    std::ofstream out;
    out.open(filename);

    out << mat.nrow << " " << mat.mcol << " " << mat.nnz << "\n";
    for (INT j = 0; j < mat.nrow + 1; ++j) out << mat.rowPtr[j] << "\n";
    for (INT j = 0; j < mat.nnz; ++j) out << mat.colInd[j] << "\n";
    for (INT j = 0; j < mat.nnz; ++j) out << mat.values[j] << "\n";

    out.close();
}

/// Write MTX format data to a disk file
void WriteMTX(char *filename, MAT mat) {
    INT begin, end, j, k;
    std::ofstream out;
    out.open(filename);
    MAT tmp = mat;
    tmp.Transpose();

    out << tmp.nrow << " " << tmp.mcol << " " << tmp.nnz << "\n";
    for (j = 0; j < tmp.nrow; ++j) {
        begin = tmp.rowPtr[j];
        end = tmp.rowPtr[j + 1];
        for (k = begin; k < end; ++k)
            out << j << " " << tmp.colInd[j] << " " << tmp.values[j] << std::endl;
    }

    out.close();
}

/// Form diagonal pointer using colInd and rowPtr
void MAT::FormDiagPtr() {
    this->diagPtr.resize(this->nrow);
    INT begin;
    for (INT j = 0; j < this->nrow; ++j) {
        begin = this->rowPtr[j];
        for (INT k = begin; k < this->rowPtr[j + 1]; ++k) {
            if (this->colInd[k] == j) {
                this->diagPtr[j] = k; // diagonal pointer
                break; // skip the rest of Row j
            }
        }
    }
}

/// Empty a matrix
void MAT::Empty() {
    this->nrow = 0;
    this->mcol = 0;
    this->nnz = 0;
    this->rowPtr.resize(0);
    this->diagPtr.resize(0);
    this->colInd.resize(0);
    this->values.resize(0);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/

/// r = y - A * x: y minus A times x
void MAT::YMAX(const VEC& y, const VEC& x, VEC& r) const {
    INT begin, i, k;
    if (this->values.size() > 0) {
        for (i = 0; i < this->nrow; ++i) {
            begin = this->rowPtr[i];
            r.values[i] = y.values[i]
                          - this->values[begin] * x.values[this->colInd[begin]];
            for (k = begin + 1; k < this->rowPtr[i + 1]; ++k)
                r.values[i] -= this->values[k] * x.values[this->colInd[k]];
        }
    } else {
        for (i = 0; i < this->nrow; ++i) {
            begin = this->rowPtr[i];
            r.values[i] = y.values[i] - x.values[this->colInd[begin]];
            for (k = begin + 1; k < this->rowPtr[i + 1]; ++k)
                r.values[i] -= x.values[this->colInd[k]];
        }
    }
}

/// mat = a * mat1 + b * mat2
void Add(const DBL a, const MAT& mat1, const DBL b, const MAT& mat2, MAT& mat) {

    INT i, j, k, l;
    INT count = 0, added, countrow;

    if (mat1.nnz == 0) {
        mat = mat2;
        mat.Scale(b);
        return;
    }

    if (mat2.nnz == 0) {
        mat = mat1;
        mat.Scale(a);
        return;
    }

    mat.nrow = mat1.nrow;
    mat.mcol = mat1.mcol;

    mat.rowPtr.resize(mat.nrow + 1);
    mat.colInd.resize(mat1.nnz + mat2.nnz);
    mat.values.resize(mat1.nnz + mat2.nnz);

    mat.colInd.assign(mat1.nnz + mat2.nnz, -1);

    for (i = 0; i < mat1.nrow; ++i) {
        countrow = 0;
        for (j = mat1.rowPtr[i]; j < mat1.rowPtr[i + 1]; ++j) {
            mat.values[count] = a * mat1.values[j];
            mat.colInd[count] = mat1.colInd[j];
            ++mat.rowPtr[i + 1];
            ++count;
            ++countrow;
        }

        for (k = mat2.rowPtr[i]; k < mat2.rowPtr[i + 1]; ++k) {
            added = 0;
            for (l = mat.rowPtr[i]; l < mat.rowPtr[i] + countrow + 1; ++l) {
                if (mat2.colInd[k] == mat.colInd[l]) {
                    mat.values[l] = mat.values[l] + b * mat2.values[k];
                    added = 1;
                    break;
                }
            }
            if (added == 0) {
                mat.values[count] = b * mat2.values[k];
                mat.colInd[count] = mat2.colInd[k];
                ++mat.rowPtr[i + 1];
                ++count;
            }
        }
        mat.rowPtr[i + 1] += mat.rowPtr[i];
    }
    mat.nnz = count;
    mat.colInd.resize(count);
    mat.values.resize(count);

    SortCSRRow(mat.nrow, mat.mcol, mat.nnz, mat.rowPtr, mat.colInd, mat.values);
}

/// *this = a * *this + b * mat
void MAT::Add(const DBL a, const DBL b, const MAT& mat) {
    MAT tmp;
    ::Add(a, *this, b, mat, tmp);
    this->operator=(tmp);
}

/// Get (*this)[i][j]
//! Note: If *this is a sparse structure, it will return 1.0 for nonzero entries
DBL MAT::GetValue(const INT& row, const INT& mcol) const {
    if (this->colInd[this->rowPtr[row]] <= mcol ||
        this->colInd[this->rowPtr[row + 1] - 1] >= mcol) {
        for (INT j = this->rowPtr[row]; j < this->rowPtr[row + 1]; ++j) {
            if (mcol == this->colInd[j]) {
                if (this->values.size() > 0)
                    return this->values[j];
                else
                    return 1.0; // It is a sparse structure indicator
            }
        }
    }
    return 0.0;
}

/// Get the whole row-th row in *this into VEC object
void MAT::GetRow(const INT& row, std::vector<DBL>& v) const {
    const INT begin = this->rowPtr[row], end = this->rowPtr[row + 1];
    v.resize(end - begin);

    INT count = 0;
    for (INT j = begin; j < end; ++j) {
        v[count] = this->values[j];
        ++count;
    }
}

/// Get the whole col-th column in *this into VEC object
void MAT::GetCol(const INT& col, std::vector<DBL>& v) const {
    std::vector<DBL> tmp;
    INT count = 0;
    tmp.resize(this->nrow);

    for (INT j = 0; j < this->nrow; ++j) {
        if (col >= this->colInd[this->rowPtr[j]] ||
            col <= this->colInd[this->rowPtr[j + 1] - 1]) {
            for (INT k = this->rowPtr[j]; k < this->rowPtr[j + 1]; ++k) {
                if (this->colInd[k] == col) {
                    if (this->values.size() == 0) {
                        tmp[count] = 1.0;
                    } else {
                        tmp[count] = this->values[k];
                    }
                    ++count;
                    break;
                }
            }
        }
    }

    v = tmp;
}

/// mat = matl * matr
void Mult(const MAT& matl, const MAT& matr, MAT& mat) {
    INT l, count;
    INT *tmp = new INT[matr.mcol];

    mat.nrow = matl.nrow;
    mat.mcol = matr.mcol;
    mat.rowPtr.resize(mat.nrow + 1);

    for (INT i = 0; i < matr.mcol; ++i) tmp[i] = -1;

    for (INT i = 0; i < mat.nrow; ++i) {
        count = 0;
        for (INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; ++k) {
            for (INT j = matr.rowPtr[matl.colInd[k]];
                 j < matr.rowPtr[matl.colInd[k] + 1]; ++j) {
                for (l = 0; l < count; ++l) {
                    if (tmp[l] == matr.colInd[j]) break;
                }
                if (l == count) {
                    tmp[count] = matr.colInd[j];
                    ++count;
                }
            }
        }
        mat.rowPtr[i + 1] = count;
        for (INT j = 0; j < count; ++j) tmp[j] = -1;
    }

    for (INT i = 0; i < mat.nrow; ++i) mat.rowPtr[i + 1] += mat.rowPtr[i];

    INT count_tmp;

    mat.colInd.resize(mat.rowPtr[mat.nrow]);

    for (INT i = 0; i < mat.nrow; ++i) {
        count_tmp = 0;
        count = mat.rowPtr[i];
        for (INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; ++k) {
            for (INT j = matr.rowPtr[matl.colInd[k]];
                 j < matr.rowPtr[matl.colInd[k] + 1]; ++j) {
                for (l = 0; l < count_tmp; ++l) {
                    if (tmp[l] == matr.colInd[j]) break;
                }
                if (l == count_tmp) {
                    mat.colInd[count] = matr.colInd[j];
                    tmp[count_tmp] = matr.colInd[j];
                    ++count;
                    ++count_tmp;
                }
            }
        }

        for (INT j = 0; j < count_tmp; ++j) tmp[j] = -1;
    }

    delete[] tmp;
    tmp = nullptr;

    mat.values.resize(mat.rowPtr[mat.nrow]);

    for (INT i = 0; i < mat.nrow; ++i) {
        for (INT j = mat.rowPtr[i]; j < mat.rowPtr[i + 1]; ++j) {
            mat.values[j] = 0;
            for (INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; ++k) {
                for (l = matr.rowPtr[matl.colInd[k]];
                     l < matr.rowPtr[matl.colInd[k] + 1]; ++l) {
                    if (matr.colInd[l] == mat.colInd[j])
                        mat.values[j] += matl.values[k] * matr.values[l];
                }
            }
        }
    }

    mat.nnz = mat.rowPtr[mat.nrow] - mat.rowPtr[0];
}

/// *this = *this * mat
void MAT::MultLeft(const MAT& mat) {
    MAT tmp;
    tmp = *this;

    Mult(tmp, mat, *this);
}

/// *this = mat * *this
void MAT::MultRight(const MAT& mat) {
    MAT tmp;
    tmp = *this;

    Mult(mat, tmp, *this);
}
