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

/// Assign nrow, mcol, nnz, values, colInd, rowPtr, diagPtr to *this.
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

/// Assign nrow, mcol, nnz, values, colInd, rowPtr to *this and generate diagPtr.
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

/// Assign nrow, mcol, nnz, colInd, rowPtr to *this and generate diagPtr.
MAT::MAT(const INT& nrow, const INT& mcol, const INT& nnz,
         const std::vector<INT>& colInd, const std::vector<INT>& rowPtr) {
    if (nrow == 0 || mcol == 0 || nnz == 0) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->mcol = mcol;
    this->nnz = nnz;
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->values.resize(0);
    this->FormDiagPtr();
}

/// Assign nrow, mcol, nnz, colInd, rowPtr, diagPtr to *this.
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
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->diagPtr = diagPtr;
    this->values.resize(0);
}

/// Assign diagonal values from a VEC to *this.
MAT::MAT(const VEC& v) {
    INT size = v.GetSize();

    // Return an empty matrix if size==0
    if (size == 0) {
        this->Empty();
        return;
    }

    // Set MAT size
    this->nrow = size;
    this->mcol = size;
    this->nnz = size;

    INT *p;
    try {
        p = new INT[size + 1];
    } catch (std::bad_alloc &ex) {
        this->nrow = 0;
        this->mcol = 0;
        this->nnz = 0;
        throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
    }

    // Set values from v
    this->values.resize(size);
    for (INT j = 0; j < size; ++j) this->values[j] = v.values[j];

    // Set colInd to {0, 1, ..., size-1}
    for (INT j = 0; j <= size; ++j) p[j] = j;
    this->colInd.resize(size);
    this->colInd.assign(p, p + size);

    // Set rowPtr to {0, 1, ..., size}
    this->rowPtr.resize(size + 1);
    this->rowPtr.assign(p, p + size + 1);

    // Set diagPtr to {0, 1, ..., size-1}
    this->diagPtr.resize(size);
    this->diagPtr.assign(p, p + size);

    delete[] p;
}

/// Assign diagonal values from a vector to *this.
MAT::MAT(const std::vector<DBL>& vt) {
    const INT size = vt.size();

    // Return an empty matrix if size==0
    if (size == 0) {
        this->Empty();
        return;
    }

    // Set MAT size
    this->nrow = size;
    this->mcol = size;
    this->nnz = size;

    INT *p;
    try {
        p = new INT[size + 1];
    } catch (std::bad_alloc &ex) {
        this->nrow = 0;
        this->mcol = 0;
        this->nnz = 0;
        throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
    }

    // Set values from vt
    this->values.resize(size);
    this->values.assign(vt.begin(), vt.begin() + size);

    // Set colInd to {0, 1, ..., size-1}
    for (INT j = 0; j <= size; ++j) p[j] = j;
    this->colInd.resize(size);
    this->colInd.assign(p, p + size);

    // Set rowPtr to {0, 1, ..., size}
    this->rowPtr.resize(size + 1);
    this->rowPtr.assign(p, p + size + 1);

    // Set diagPtr to {0, 1, ..., size-1}
    this->diagPtr.resize(size);
    this->diagPtr.assign(p, p + size);

    delete[] p;
}

/// Assign MAT object to *this.
MAT::MAT(const MAT& mat) {
    this->nrow = mat.nrow;
    this->mcol = mat.mcol;
    this->nnz = mat.nnz;
    this->values = mat.values;
    this->colInd = mat.colInd;
    this->rowPtr = mat.rowPtr;
    this->diagPtr = mat.diagPtr;
}

/// Assignment for the MAT object.
MAT& MAT::operator=(const MAT& mat) {
    this->nrow = mat.nrow;
    this->mcol = mat.mcol;
    this->nnz = mat.nnz;
    this->values = mat.values;
    this->colInd = mat.colInd;
    this->rowPtr = mat.rowPtr;
    this->diagPtr = mat.diagPtr;
    return *this;
}

/// Set values of nrow, mcol, nnz, values, colInd, rowPtr, diagPtr.
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

/// Set values of nrow, mcol, nnz, values, rowPtr, colInd.
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

/// Return this->nnz.
INT MAT::GetNNZ() const {
    return this->nnz;
}

/// Get the diagonal entries of *this and save them in a VEC object.
void MAT::GetDiag(VEC& v) const {
    INT len = this->nrow;
    v.size = len;
    v.values.resize(len);

    if (this->values.size() > 0) {
        for (INT j = 0; j < len; ++j) v.values[j] = this->values[this->diagPtr[j]];
    } else {
        v.values.assign(len, 1.0);
    }

}

/// Get the diagonal entries 's reciprocal of *this and save them in a MAT object.
void MAT::GetDiagInv(MAT& m) const {
    m.nrow = this->nrow;
    m.mcol = this->mcol;
    m.nnz = this->nrow;

    m.rowPtr.resize(m.nrow + 1);
    for (INT j = 0; j < m.nrow + 1; ++j)
        m.rowPtr[j] = j;

    m.diagPtr.resize(m.nrow);
    for (INT j = 0; j < m.nrow; ++j)
        m.diagPtr[j] = j;

    m.colInd.resize(m.nnz);
    for (INT j = 0; j < m.nnz; ++j)
        m.colInd[j] = j;

    m.values.resize(m.nnz);
    for (INT j = 0; j < m.nnz; ++j)
        m.values[j] = 1.0 / this->values[this->diagPtr[j]];

}

// Get the lower triangular matrix
void MAT::GetLowerTri(MAT& lTri) const {
    lTri.nrow = this->nrow;
    lTri.mcol = this->mcol;
    lTri.rowPtr.resize(this->nrow + 1);

    lTri.rowPtr[0] = 0;
    for (INT j = 1; j < this->nrow + 1; ++j)
        lTri.rowPtr[j] = this->diagPtr[j - 1] - this->rowPtr[j - 1] + 1;

    for (INT j = 1; j < this->nrow + 1; ++j)
        lTri.rowPtr[j] += lTri.rowPtr[j - 1];

    lTri.nnz = lTri.rowPtr[this->nrow];

    lTri.values.resize(lTri.nnz);
    lTri.colInd.resize(lTri.nnz);

    INT count = 0;
    for (INT j = 0; j < this->nrow; ++j) {
        for (INT k = this->rowPtr[j]; k <= this->diagPtr[j]; ++k) {
            lTri.values[count] = this->values[k];
            lTri.colInd[count] = this->colInd[k];
            count++;
        }
    }

    lTri.FormDiagPtr();
}

// Get the upper triangular matrix
void MAT::GetUpperTri(MAT& uTri) const {
    uTri.nrow = this->nrow;
    uTri.mcol = this->mcol;
    uTri.rowPtr.resize(this->nrow + 1);

    uTri.rowPtr[0] = 0;
    for (INT j = 1; j < this->nrow + 1; ++j)
        uTri.rowPtr[j] = this->rowPtr[j] - this->diagPtr[j - 1];

    for (INT j = 1; j < this->nrow + 1; ++j)
        uTri.rowPtr[j] += uTri.rowPtr[j - 1];

    uTri.nnz = uTri.rowPtr[this->nrow];

    uTri.values.resize(uTri.nnz);
    uTri.colInd.resize(uTri.nnz);

    INT count = 0;
    for (INT j = 0; j < this->nrow; ++j) {
        for (INT k = this->diagPtr[j]; k <= this->rowPtr[j + 1] - 1; ++k) {
            uTri.values[count] = this->values[k];
            uTri.colInd[count] = this->colInd[k];
            count++;
        }
    }

    uTri.FormDiagPtr();
}

/// Copy *this to mat.
void MAT::CopyTo(MAT& mat) const {
    mat = *this;
}

/// Scale *this *= a
void MAT::Scale(const DBL a) {
    if (this->values.empty()) return; // MAT is a sparse structure!!!

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

/// Set all the entries to zero, without changing matrix size.
void MAT::Zero() {
    for (INT j = 0; j < this->nnz; ++j) values[j] = 0.0;
}

/// Compute w = *this * v.
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

/// Transpose *this in place
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
        throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
    }

    if (!this->values.empty()) {
        try {
            tmp.values.resize(nnz);
        } catch (std::bad_alloc &ex) {
            throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
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

/// Compute v = A'*v1 + v2.
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
        throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
    }

    if (this->values.size()) {
        try {
            tmp.values.resize(nnz);
        } catch (std::bad_alloc &ex) {
            throw( FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__) );
        }
    } else {
        tmp.values.resize(0);
    }

    for (j = 0; j < nnz; ++j) {
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

    v = v2;

    INT begin;
    for (i = 0; i < tmp.nrow; ++i) {
        begin = tmp.rowPtr[i];
        for (j = begin; j < this->rowPtr[i + 1]; ++j)
            v.values[i] += v1.values[tmp.colInd[j]] * tmp.values[j];
    }
}

/// Get (*this)[i][j].
/// \note If *this is a sparse structure, it will return 1.0 for nonzero entries.
DBL MAT::GetValue(const INT& irow, const INT& jcol) const {
    if ( this->colInd[this->rowPtr[irow]] <= jcol &&
         this->colInd[this->rowPtr[irow + 1] - 1] >= jcol ) {
        for ( INT j = this->rowPtr[irow]; j < this->rowPtr[irow + 1]; ++j ) {
            if ( jcol == this->colInd[j] ) {
                if ( this->values.size() > 0 )
                    return this->values[j];
                else
                    return 1.0; // sparse structure indicator
            }
        }
    }
    return 0.0;
}

/// *this = a * mat1 + b * mat2.
void MAT::Add(const DBL a, const MAT& mat1, const DBL b, const MAT& mat2) {

    MAT tmpMat;
    INT i, j, k, l;
    INT count = 0, added, countrow;

    if (mat1.nnz == 0) {
        tmpMat = mat2;
        tmpMat.Scale(b);
        return;
    }

    if (mat2.nnz == 0) {
        tmpMat = mat1;
        tmpMat.Scale(a);
        return;
    }

    tmpMat.nrow = mat1.nrow;
    tmpMat.mcol = mat1.mcol;

    tmpMat.rowPtr.resize(tmpMat.nrow + 1);
    tmpMat.colInd.resize(mat1.nnz + mat2.nnz);
    tmpMat.values.resize(mat1.nnz + mat2.nnz);

    tmpMat.colInd.assign(mat1.nnz + mat2.nnz, -1);

    for (i = 0; i < mat1.nrow; ++i) {
        countrow = 0;
        for (j = mat1.rowPtr[i]; j < mat1.rowPtr[i + 1]; ++j) {
            tmpMat.values[count] = a * mat1.values[j];
            tmpMat.colInd[count] = mat1.colInd[j];
            ++tmpMat.rowPtr[i + 1];
            ++count;
            ++countrow;
        }

        for (k = mat2.rowPtr[i]; k < mat2.rowPtr[i + 1]; ++k) {
            added = 0;
            for (l = tmpMat.rowPtr[i]; l < tmpMat.rowPtr[i] + countrow + 1; ++l) {
                if (mat2.colInd[k] == tmpMat.colInd[l]) {
                    tmpMat.values[l] = tmpMat.values[l] + b * mat2.values[k];
                    added = 1;
                    break;
                }
            }
            if (added == 0) {
                tmpMat.values[count] = b * mat2.values[k];
                tmpMat.colInd[count] = mat2.colInd[k];
                ++tmpMat.rowPtr[i + 1];
                ++count;
            }
        }
        tmpMat.rowPtr[i + 1] += tmpMat.rowPtr[i];
    }
    tmpMat.nnz = count;
    tmpMat.colInd.resize(count);
    tmpMat.values.resize(count);
    tmpMat.colInd.shrink_to_fit();
    tmpMat.values.shrink_to_fit();

    SortCSRRow(tmpMat.nrow, tmpMat.mcol, tmpMat.nnz, tmpMat.rowPtr, tmpMat.colInd,
               tmpMat.values);

    tmpMat.FormDiagPtr();
    *this = tmpMat;
}

/// *this = matl * matr.
void MAT::Mult(const MAT& matl, const MAT& matr) {
    INT l, count;
    INT *tmp = new INT[matr.mcol];

    MAT mat;

    this->nrow = matl.nrow;
    this->mcol = matr.mcol;
    this->rowPtr.resize(this->nrow + 1);

    for (INT i = 0; i < matr.mcol; ++i) tmp[i] = -1;

    for (INT i = 0; i < this->nrow; ++i) {
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
        this->rowPtr[i + 1] = count;
        for (INT j = 0; j < count; ++j) tmp[j] = -1;
    }

    for (INT i = 0; i < this->nrow; ++i) this->rowPtr[i + 1] += this->rowPtr[i];

    INT count_tmp;

    this->colInd.resize(this->rowPtr[this->nrow]);

    for (INT i = 0; i < this->nrow; ++i) {
        count_tmp = 0;
        count = this->rowPtr[i];
        for (INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; ++k) {
            for (INT j = matr.rowPtr[matl.colInd[k]];
                 j < matr.rowPtr[matl.colInd[k] + 1]; ++j) {
                for (l = 0; l < count_tmp; ++l) {
                    if (tmp[l] == matr.colInd[j]) break;
                }
                if (l == count_tmp) {
                    this->colInd[count] = matr.colInd[j];
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

    this->values.resize(this->rowPtr[this->nrow]);

    for (INT i = 0; i < this->nrow; ++i) {
        for (INT j = this->rowPtr[i]; j < this->rowPtr[i + 1]; ++j) {
            this->values[j] = 0;
            for (INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; ++k) {
                for (l = matr.rowPtr[matl.colInd[k]];
                     l < matr.rowPtr[matl.colInd[k] + 1]; ++l) {
                    if (matr.colInd[l] == this->colInd[j])
                        this->values[j] += matl.values[k] * matr.values[l];
                }
            }
        }
    }

    this->nnz = this->rowPtr[this->nrow] - this->rowPtr[0];

    this->FormDiagPtr();

}

/// *this = *this * mat.
void MAT::MultLeft(const MAT& mat) {
    MAT tmp;
    tmp = *this;

    Mult(tmp, mat);
}

/// *this = mat * *this.
void MAT::MultRight(const MAT& mat) {
    MAT tmp;
    tmp = *this;

    Mult(mat, tmp);
}

/// Write data to a disk file in CSR format.
void WriteCSR(char *filename, MAT mat) {
    std::ofstream out;
    out.open(filename);

    out << mat.nrow << " " << mat.mcol << " " << mat.nnz << "\n";
    for (INT j = 0; j < mat.nrow + 1; ++j) out << mat.rowPtr[j] << "\n";
    for (INT j = 0; j < mat.nnz; ++j) out << mat.colInd[j] << "\n";
    for (INT j = 0; j < mat.nnz; ++j) out << mat.values[j] << "\n";

    out.close();
}

/// Write data to a disk file in MTX format.
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

/// Form diagPtr by using colInd and rowPtr.
void MAT::FormDiagPtr() {
    this->diagPtr.resize(this->nrow);
    for (INT j = 0; j < this->nrow; ++j) {
        for (INT k = this->rowPtr[j]; k < this->rowPtr[j + 1]; ++k) {
            if (this->colInd[k] == j) {
                this->diagPtr[j] = k; // set diagonal pointer
                break; // skip the rest of Row j
            }
        }
    }
}

/// Empty *this.
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

#if 0

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

#endif