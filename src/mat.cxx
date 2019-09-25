/*! \file vec.cxx
 *  \brief Source file for the FASP++ Matrix class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */
#include "mat.hxx"
#include "auxmat.hxx"

/*---------------------------------------------------------------------------------*/

//! assign row, col, nnz, values, rowPtr, colInd, diag to *this
MAT::MAT(const INT row, const INT col, const INT nnz, 
         const std::vector<DBL> values, const std::vector<INT> rowPtr, 
         const std::vector<INT> colInd, const std::vector<INT> diag) {

    if ( row == 0 || col == 0 || nnz == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->diagPtr.resize(0);
        this->colInd.resize(0);
        this->values.resize(0);
        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values.operator=(values);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diag);
}

//! assign row, col, nnz, rowPtr, colInd, diag to *this
MAT::MAT(const INT row, const INT col, const INT nnz, const std::vector<INT>
rowPtr, const std::vector<INT> colInd, const std::vector<INT> diag) {

    if ( row == 0 || col == 0 || nnz == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->colInd.resize(0);
        this->values.resize(0);
        this->rowPtr.resize(0);
        this->diagPtr.resize(0);
        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values.resize(0);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diag);
}

//! assign row, col, nnz, values, rowPtr, colInd to *this
MAT::MAT(const INT row, const INT col, const INT nnz, const std::vector<DBL>
values, const std::vector<INT> rowPtr, const std::vector<INT> colInd) {

    if ( row == 0 || col == 0 || nnz == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->diagPtr.resize(0);
        this->rowPtr.resize(0);
        this->values.resize(0);
        this->colInd.resize(0);
        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values = values;
    this->rowPtr = rowPtr;
    this->colInd = colInd;

    this->diagPtr.resize(row > col ? col : row);
    //! compute this->diagPtr
    INT count = 0;
    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowPtr[j];
        end = this->rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colInd[k] == j ) {
                this->diagPtr[count] = k;
                count++;
            }
        }
    }
}

//! assign row, col, nnz, rowPtr, colInd to *this
MAT::MAT(const INT row, const INT col, const INT nnz, const std::vector<INT>
rowPtr, const std::vector<INT> colInd) {

    if ( row == 0 || col == 0 || nnz == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->rowPtr.resize(0);
        this->values.resize(0);
        return;
    }

    this->row=row;
    this->col=col;
    this->nnz=nnz;
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.resize(0);

    this->diagPtr.resize(row > col ? col : row);
    //! compute this->diagPtr
    INT count = 0;
    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowPtr[j];
        end = this->rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colInd[k] == j ) {
                this->diagPtr[count] = k;
                count++;
            }
        }
    }
}

//! assign VEC object to form the diagonal matrix
MAT::MAT(VEC &vec) {
    INT size = vec.GetSize();
    if ( size == 0 ) {
        this->col = 0;
        this->row = 0;
        this->nnz = 0;
        this->values.resize(0);
        this->colInd.resize(0);
        this->rowPtr.resize(0);
        this->diagPtr.resize(0);
    } else {
        INT *array = new INT[size];
        for ( INT j = 0; j < size; j++ )
            array[j] = j;

        this->row = size;
        this->col = size;
        this->nnz = size;
        this->rowPtr.resize(size + 1);
        this->rowPtr.assign(array, array + size);
        this->rowPtr[size] = size;
        this->colInd.resize(size);
        this->colInd.assign(array, array + size);
        this->values.resize(size);

        for ( INT j = 0; j < size; j++ )
            this->values[j] = vec[j];

        this->diagPtr.resize(size);
        this->diagPtr.assign(array, array + size);

        delete[] array;
        array = nullptr;
    }
}

//! assign vector object to form the diagonal matrix
MAT::MAT(const std::vector<DBL> &vect) {
    INT size = vect.size();
    if ( size == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);
    } else {
        this->row = size;
        this->col = size;
        this->nnz = size;

        INT *p = new INT[size];
        for ( INT j = 0; j < size; j++ )
            p[j] = j;

        this->rowPtr.resize(size + 1);
        this->rowPtr.assign(p, p + size);
        this->rowPtr[size] = size;
        this->colInd.resize(size);
        this->colInd.assign(p, p + size);
        this->diagPtr.resize(size);
        this->diagPtr.assign(p, p + size);
        this->values.resize(size);
        this->values.assign(vect.begin(), vect.begin() + size);

        delete[] p;
        p = nullptr;
    }
}

//! assign MAT object to *this
MAT::MAT(const MAT &mat) {
    this->row = mat.row;
    this->col = mat.col;
    this->nnz = mat.nnz;
    this->values.operator=(mat.values);
    this->diagPtr.operator=(mat.diagPtr);
    this->colInd.operator=(mat.colInd);
    this->rowPtr.operator=(mat.rowPtr);
}

//! overload = operator
MAT &MAT::operator=(const MAT &mat) {
    this->row = mat.row;
    this->col = mat.col;
    this->nnz = mat.nnz;
    this->diagPtr.operator=(mat.diagPtr);
    this->colInd.operator=(mat.colInd);
    this->rowPtr.operator=(mat.rowPtr);
    this->values.operator=(mat.values);
    return *this;
}

//! assign row, col, nnz, values, rowPtr, colInd, diag to *this
void MAT::SetValues(const INT row, const INT col, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowPtr,
                    const std::vector<INT> colInd, const std::vector<INT> diag) {

    if ( row == 0 || col == 0 || nnz == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->diagPtr.resize(0);
        this->colInd.resize(0);
        this->values.resize(0);
        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values.operator=(values);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diag);
}

//! assign row, col, nnz, values, rowPtr, colInd to *this
void MAT::SetValues(const INT row, const INT col, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowPtr,
                    const std::vector<INT> colInd) {

    if ( row == 0 || col == 0 || nnz == 0 ) {
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->diagPtr.resize(0);
        this->colInd.resize(0);
        this->values.resize(0);
        return;
    }

    this->row=row;
    this->col=col;
    this->nnz=nnz;
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->values.operator=(values);

    this->diagPtr.resize(row > col ? col : row);
    //! compute this->diagPtr
    INT count = 0;
    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowPtr[j];
        end = this->rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colInd[k] == j ) {
                this->diagPtr[count] = k;
                count++;
            }
        }
    }
}

//! get row size
INT MAT::GetRowSize() const {
    return this->row;
}

//! get column size
INT MAT::GetColSize() const {
    return this->col;
}

//! get number of nonzeros
INT MAT::GetNNZ() const {
    return this->nnz;
}

//! get (*this)[i][j]
DBL MAT::GetElem(const INT row, const INT col) const {
    if ( this->colInd[this->rowPtr[row]] <= col ) {
        for ( INT j = this->rowPtr[row]; j < this->rowPtr[row + 1]; j++ ) {
            if ( col == this->colInd[j] ) {
                if ( this->values.size() == 0 ) {
                    return 1.0;
                } else {
                    return (this->values[j]);
                }
            }
        }
    }
    return 0;
}

//! get the whole jth-row elements in *this into vector object
std::vector<DBL> MAT::GetRow(const INT row) const {
    std::vector<DBL> vec;
    INT len = this->rowPtr[row + 1] - this->rowPtr[row];
    vec.resize(len);
    if ( this->values.size() == 0 )
        vec.assign(len, 1);

    INT k = 0;
    for ( INT j = this->rowPtr[row]; j < this->rowPtr[row + 1]; j++ ) {
        vec[k] = this->values[j];
        k++;
    }

    return vec;
}

//! get the whole jth-col elements in *this into vector object
std::vector<DBL> MAT::GetCol(const INT col) const {
    std::vector<DBL> tmp;
    INT count = 0;
    tmp.resize(this->row);
    for ( INT j = 0; j < this->row; j++ ) {
        if ( col >= this->colInd[this->rowPtr[j]] ) {
            for ( INT k = this->rowPtr[j]; k < this->rowPtr[j + 1]; k++ ) {
                if ( this->colInd[k] == col ) {
                    if ( this->values.size() == 0 ) {
                        tmp[count] = 1;
                    } else {
                        tmp[count] = this->values[k];
                    }
                    count++;
                    break;
                }
            }
        }
    }

    std::vector<DBL> vec(count);

    vec.assign(tmp.begin(),tmp.end());

    return vec;
}

//! get the whole diagonal elements in *this into VEC object
std::vector<DBL> MAT::GetDiag() const {
    std::vector<DBL> vec;
    INT len = this->row > this->col ? this->col : this->row;
    vec.resize(len);
    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < len; j++ )
            vec[j] = this->values[this->diagPtr[j]];
    } else {
        vec.assign(len, 1);
    }

    return vec;
}

//! zero all the elements
void MAT::Zero() {
    this->nnz = this->row > this->col ? this->col : this->row;
    INT *pcol = new INT[this->nnz];
    INT *prow = new INT[this->row + 1];

    for ( INT j = 0; j < this->diagPtr.size(); j++ )
        pcol[j] = j;

    for ( INT j = 0; j < this->row + 1; j++ )
        prow[j] = j;

    std::vector<INT> rowPtrtmp(this->row + 1);
    std::vector<INT> colIndtmp(this->nnz);
    std::vector<DBL> valuestmp(this->nnz);

    rowPtrtmp.assign(prow, prow + this->row);
    colIndtmp.assign(pcol, pcol + this->nnz);
    valuestmp.assign(this->nnz, 0);

    this->rowPtr.operator=(rowPtrtmp);
    this->colInd.operator=(colIndtmp);
    this->values.operator=(valuestmp);

    delete[] pcol;
    delete[] prow;
    pcol = nullptr;
    prow = nullptr;

}

//! copy *this into mat
void MAT::CopyTo(MAT &mat) const {
    mat.operator=(*this);
}

//! *this = a * (*this)
void MAT::Scale(const DBL a) {
    if ( this->values.size() == 0 ) {
        this->values.resize(this->nnz);
        for ( INT j = 0; j < this->nnz; j++ )
            this->values[j] = a;
    } else {
        for ( INT j = 0; j < this->nnz; j++ )
            this->values[j] = a * this->values[j];
    }
}

//! *this = a * I + *this
void MAT::Shift(const DBL a) {
    if ( this->values.size() == 0 ) {
        if ( a == 0 )
            return;
        for ( int j = 0; j < this->diagPtr.size(); j++ )
            this->values[j] = 1.0;
        for ( int j = 0; j < diagPtr.size(); j++ )
            this->values[this->diagPtr[j]] = 1 + a;
    } else {
        for ( INT j = 0; j < this->diagPtr.size(); j++ )
            this->values[this->diagPtr[j]] = a + this->values[this->diagPtr[j]];
    }
}

//! ReturnedValues = *this * vec
VEC MAT::MultVec(const VEC vec) const {
    VEC vec_;

    INT begin, end;
    vec_.Reserve(this->row);
    if ( this->values.size() == 0 ) {
        for ( INT j = 0; j < this->row; j++ ) {
            begin = this->rowPtr[j];
            end = this->rowPtr[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec_[j] += vec[this->colInd[k]];
            }
        }
    } else {
        for ( INT j = 0; j < this->row; j++ ) {
            begin = this->rowPtr[j];
            end = this->rowPtr[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec_[j] += this->values[k] * vec[this->colInd[k]];
            }
        }
    }

    return vec_;
}

//! transpose *this
void MAT::Transpose() {
    MAT tmp;
    tmp.row = this->col;
    tmp.col = this->row;
    tmp.nnz = this->nnz;

    tmp.rowPtr.resize(tmp.row + 1);
    tmp.colInd.resize(tmp.nnz);
    tmp.values.resize(tmp.nnz);
    tmp.diagPtr.resize(tmp.row > tmp.col ? tmp.col : tmp.row);

    INT begin, end;
    INT count = 0;
    tmp.rowPtr[0] = 0;

    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < tmp.row; j++ ) {
            for ( INT k = 0; k < this->row; k++ ) {
                begin = this->rowPtr[k];
                end = this->rowPtr[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colInd[l] == j ) {
                        tmp.rowPtr[j + 1]++;
                        tmp.colInd[count] = k;
                        tmp.values[count] = this->values[l];
                        count++;
                        break;
                    }
                }
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row; j++ ) {
            for ( INT k = 0; k < this->row; k++ ) {
                begin = this->rowPtr[k];
                end = this->rowPtr[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colInd[l] == j ) {
                        tmp.rowPtr[j + 1]++;
                        tmp.colInd[count] = k;
                        count++;
                        break;
                    }
                }
            }
        }
    }

    for ( INT j = 0; j < tmp.row; j++ )
        tmp.rowPtr[j + 1] += tmp.rowPtr[j];

    count = 0;
    for ( INT j = 0; j < tmp.row; j++ ) {
        begin = tmp.rowPtr[j];
        end = tmp.rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( tmp.colInd[k] == j ) {
                tmp.diagPtr[count] = k;
                count++;
            }
        }
    }

    this->operator=(tmp);
}

//! ReturnedValues = vec2 + transpose(*this) * vec1
VEC MAT::MultTransposeAdd(const VEC vec1, const VEC vec2) const {
    VEC vec;
    vec.operator=(vec2);

    MAT tmp;
    tmp.row = this->col;
    tmp.col = this->row;
    tmp.nnz = this->nnz;

    tmp.rowPtr.resize(tmp.row + 1);
    tmp.colInd.resize(tmp.nnz);
    tmp.values.resize(tmp.nnz);

    INT begin, end;
    INT count = 0;
    tmp.rowPtr[0] = 0;

    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < tmp.row; j++ ) {
            for ( INT k = 0; k < this->row; k++ ) {
                begin = this->rowPtr[k];
                end = this->rowPtr[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colInd[l] == j ) {
                        tmp.rowPtr[j + 1]++;
                        tmp.colInd[count] = k;
                        tmp.values[count] = this->values[l];
                        count++;
                        break;
                    }
                }
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row; j++ ) {
            for ( INT k = 0; k < this->row; k++ ) {
                begin = this->rowPtr[k];
                end = this->rowPtr[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colInd[l] == j ) {
                        tmp.rowPtr[j + 1]++;
                        tmp.colInd[count] = k;
                        count++;
                        break;
                    }
                }
            }
        }
    }

    for ( INT j = 0; j < tmp.row; j++ )
        tmp.rowPtr[j + 1] += tmp.rowPtr[j];

    if ( this->values.size()) {
        for ( INT j = 0; j < tmp.row; j++ ) {
            begin = tmp.rowPtr[j];
            end = tmp.rowPtr[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec[j] += vec1[this->colInd[k]] * tmp.values[k];
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row; j++ ) {
            begin = tmp.rowPtr[j];
            end = tmp.rowPtr[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec[j] += vec1[this->colInd[k]];
            }
        }
    }

    return vec;
}

//! *this = a * *this + b * mat
void MAT::Add(const DBL a, const DBL b, const MAT mat) {

    if ( this->nnz == 0 )
        this->operator=(mat);

    if ( mat.nnz == 0 )
        return;

    MAT tmp1, tmp2;
    tmp1.row = this->row;
    tmp1.col = this->col;
    tmp1.rowPtr.resize(this->row + 1);
    tmp1.colInd.resize(this->nnz + mat.nnz);
    tmp1.values.resize(this->nnz + mat.nnz);
    tmp1.diagPtr.resize(this->row > this->col ? this->col : this->row);

    INT begin1, end1, begin2, end2;
    INT count;

    count = 0;
    for ( int j = 0; j < this->row; j++ ) {
        begin1 = this->rowPtr[j];
        end1 = this->rowPtr[j + 1];
        begin2 = mat.rowPtr[j];
        end2 = mat.rowPtr[j + 1];
        if ( this->values.size() != 0 && mat.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colInd[count] = this->colInd[k];
                tmp1.values[count] = a * this->values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colInd[count] = mat.colInd[k];
                tmp1.values[count] = b * mat.values[k];
                count++;
            }
        }
        if ( this->values.size() == 0 && mat.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colInd[count] = this->colInd[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colInd[count] = mat.colInd[k];
                count++;
            }
        }
    }

    tmp1.rowPtr[0] = 0;
    for ( int j = 0; j < this->row; j++ )
        tmp1.rowPtr[j + 1] = this->rowPtr[j + 1] + mat.rowPtr[j + 1];

    Sort(tmp1.row, tmp1.rowPtr, tmp1.colInd, tmp1.values);

    tmp2.row = tmp1.row;
    tmp2.col = tmp1.col;
    tmp2.nnz = 0;
    tmp2.rowPtr.resize(tmp2.row + 1);
    tmp2.rowPtr[0] = 0;

    INT mem;
    INT begin, end;
    for ( INT j = 0; j < this->row; j++ ) {
        begin = tmp1.rowPtr[j];
        end = tmp1.rowPtr[j + 1];
        if ( begin == end - 1 ) {
            tmp2.nnz++;
            tmp2.rowPtr[j + 1] += 1;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp1.colInd[k] < tmp1.colInd[k + 1] ) {
                tmp2.nnz++;
                tmp2.rowPtr[j + 1] += 1;
                mem = tmp1.colInd[k];
            }
        }
        if ( mem < tmp1.colInd[end - 1] ) {
            tmp2.nnz++;
            tmp2.rowPtr[j + 1] += 1;
        }
    }

    for ( int j = 1; j < this->row + 1; j++ )
        tmp2.rowPtr[j] += tmp2.rowPtr[j - 1];

    tmp2.colInd.resize(tmp2.nnz);
    tmp2.values.resize(tmp2.nnz);
    tmp2.diagPtr.resize(this->row > this->col ? this->col : this->row);

    count = 0;
    for ( INT j = 0; j < tmp1.row; j++ ) {
        begin = tmp1.rowPtr[j];
        end = tmp1.rowPtr[j + 1];
        if ( begin == end - 1 ) {
            tmp2.colInd[count] = j;
            tmp2.values[count] = tmp1.values[begin];
            count++;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp1.colInd[k] < tmp1.colInd[k + 1] ) {
                tmp2.colInd[count] = tmp1.colInd[k];
                tmp2.values[count] += tmp1.values[k];
                count++;
            }
            if ( tmp1.colInd[k] == tmp1.colInd[k + 1] ) {
                tmp2.colInd[count] = tmp1.colInd[k];
                tmp2.values[count] += tmp1.values[k];
            }
        }

        tmp2.colInd[count] = tmp1.colInd[end - 1];
        tmp2.values[count] += tmp1.values[end - 1];
        count++;
    }

    //! compute this->diagPtr
    count = 0;

    for ( INT j = 0; j < tmp2.row; j++ ) {
        begin = tmp2.rowPtr[j];
        end = tmp2.rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( tmp2.colInd[k] == j ) {
                tmp2.diagPtr[count] = k;
                count++;
            }
        }
    }

    this->operator=(tmp2);
}

//! ReturnedValues = a * mat1 + b * mat2
MAT Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2) {
    MAT mat;

    if ( mat1.nnz == 0 ) {
        mat.row = mat2.row;
        mat.col = mat2.col;
        mat.nnz = mat2.nnz;
        mat.rowPtr.operator=(mat2.rowPtr);
        mat.colInd.operator=(mat2.colInd);
        mat.diagPtr.operator=(mat2.diagPtr);
        for ( int j = 0; j < mat2.nnz; j++ )
            mat.values[j] = b * mat2.values[j];

        return mat;
    }

    if ( mat2.nnz == 0 ) {
        mat.row = mat1.row;
        mat.col = mat1.col;
        mat.nnz = mat1.nnz;
        mat.rowPtr.operator=(mat1.rowPtr);
        mat.colInd.operator=(mat1.colInd);
        mat.diagPtr.operator=(mat1.diagPtr);
        for ( int j = 0; j < mat1.nnz; j++ )
            mat.values[j] = a * mat1.values[j];

        return mat;
    }

    MAT tmp;
    tmp.row = mat1.row;
    tmp.col = mat1.col;
    tmp.rowPtr.resize(mat1.row + 1);
    tmp.colInd.resize(mat1.nnz + mat2.nnz);
    tmp.values.resize(mat1.nnz + mat2.nnz);
    tmp.diagPtr.resize(mat1.row > mat1.col ? mat1.col : mat1.row);

    INT begin1, end1, begin2, end2;
    INT count;

    count = 0;
    for ( int j = 0; j < mat1.row; j++ ) {
        begin1 = mat1.rowPtr[j];
        end1 = mat1.rowPtr[j + 1];
        begin2 = mat2.rowPtr[j];
        end2 = mat2.rowPtr[j + 1];
        if ( mat1.values.size() != 0 && mat2.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colInd[count] = mat1.colInd[k];
                tmp.values[count] = a * mat1.values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colInd[count] = mat2.colInd[k];
                tmp.values[count] = b * mat2.values[k];
                count++;
            }
        }
        if ( mat1.values.size() == 0 && mat2.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colInd[count] = mat1.colInd[k];
                tmp.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colInd[count] = mat2.colInd[k];
                tmp.values[count] = b;
                count++;
            }
        }
    }

    tmp.rowPtr[0] = 0;
    for ( int j = 0; j < mat1.row; j++ )
        tmp.rowPtr[j + 1] = mat1.rowPtr[j + 1] + mat2.rowPtr[j + 1];

    INT begin, end;

    Sort(mat1.row, tmp.rowPtr, tmp.colInd, tmp.values);

    mat.row = tmp.row;
    mat.col = tmp.col;
    mat.nnz = 0;
    mat.rowPtr.resize(mat.row + 1);
    mat.rowPtr[0] = 0;

    INT mem;
    for ( INT j = 0; j < mat1.row; j++ ) {
        begin = tmp.rowPtr[j];
        end = tmp.rowPtr[j + 1];
        if ( begin == end - 1 ) {
            mat.nnz++;
            mat.rowPtr[j + 1] += 1;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp.colInd[k] < tmp.colInd[k + 1] ) {
                mat.nnz++;
                mat.rowPtr[j + 1] += 1;
                mem = tmp.colInd[k];
            }
        }
        if ( mem < tmp.colInd[end - 1] ) {
            mat.nnz++;
            mat.rowPtr[j + 1] += 1;
        }
    }

    for ( int j = 1; j < mat1.row + 1; j++ )
        mat.rowPtr[j] += mat.rowPtr[j - 1];

    mat.colInd.resize(mat.nnz);
    mat.values.resize(mat.nnz);
    mat.diagPtr.resize(mat1.row > mat1.col ? mat1.col : mat1.row);

    count = 0;
    for ( INT j = 0; j < tmp.row; j++ ) {
        begin = tmp.rowPtr[j];
        end = tmp.rowPtr[j + 1];
        if ( begin == end - 1 ) {
            mat.colInd[count] = j;
            mat.values[count] = tmp.values[begin];
            count++;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp.colInd[k] < tmp.colInd[k + 1] ) {
                mat.colInd[count] = tmp.colInd[k];
                mat.values[count] += tmp.values[k];
                count++;
            }
            if ( tmp.colInd[k] == tmp.colInd[k + 1] ) {
                mat.colInd[count] = tmp.colInd[k];
                mat.values[count] += tmp.values[k];
            }
        }

        mat.colInd[count] = tmp.colInd[end - 1];
        mat.values[count] += tmp.values[end - 1];
        count++;
    }

    //! compute this->diagPtr
    count = 0;

    for ( INT j = 0; j < mat.row; j++ ) {
        begin = mat.rowPtr[j];
        end = mat.rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( mat.colInd[k] == j ) {
                mat.diagPtr[count] = k;
                count++;
            }
        }
    }

    return mat;
}

//! *this = matl * matr
MAT Mult(const MAT matl, const MAT matr) {
    MAT mat;

    INT l, count;
    INT *tmp = new INT[matr.col];

    mat.row = matl.row;
    mat.col = matr.col;
    mat.rowPtr.resize(mat.row + 1);

    for ( INT i = 0; i < matr.col; i++ )
        tmp[i] = -1;

    for ( INT i = 0; i < mat.row; i++ ) {
        count = 0;
        for ( INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; k++ ) {
            for ( INT j = matr.rowPtr[matl.colInd[k]];
                  j < matr.rowPtr[matl.colInd[k] + 1]; j++ ) {
                for ( l = 0; l < count; l++ ) {
                    if ( tmp[l] == matr.colInd[j] )
                        break;
                }
                if ( l == count ) {
                    tmp[count] = matr.colInd[j];
                    count++;
                }
            }
        }
        mat.rowPtr[i + 1] = count;
        for ( INT j = 0; j < count; j++ )
            tmp[j] = -1;
    }

    for ( INT i = 0; i < mat.row; i++ )
        mat.rowPtr[i + 1] += mat.rowPtr[i];

    INT count_tmp;

    mat.colInd.resize(mat.rowPtr[mat.row]);

    for ( INT i = 0; i < mat.row; i++ ) {
        count_tmp = 0;
        count = mat.rowPtr[i];
        for ( INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; k++ ) {
            for ( INT j = matr.rowPtr[matl.colInd[k]];
                  j < matr.rowPtr[matl.colInd[k] + 1]; j++ ) {
                for ( l = 0; l < count_tmp; l++ ) {
                    if ( tmp[l] == matr.colInd[j] )
                        break;
                }

                if ( l == count_tmp ) {
                    mat.colInd[count] = matr.colInd[j];
                    tmp[count_tmp] = matr.colInd[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for ( INT j = 0; j < count_tmp; j++ )
            tmp[j] = -1;
    }

    delete[] tmp;
    tmp= nullptr;

    mat.values.resize(mat.rowPtr[mat.row]);

    for ( INT i = 0; i < mat.row; i++ ) {
        for ( INT j = mat.rowPtr[i]; j < mat.rowPtr[i + 1]; j++ ) {
            mat.values[j] = 0;
            for ( INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; k++ ) {
                for ( l = matr.rowPtr[matl.colInd[k]];
                      l < matr.rowPtr[matl.colInd[k] + 1]; l++ ) {
                    if ( matr.colInd[l] == mat.colInd[j] )
                        mat.values[j] += matl.values[k] * matr.values[l];
                }
            }
        }
    }

    mat.nnz = mat.rowPtr[mat.row] - mat.rowPtr[0];

    return mat;
}

//! *this = *this * mat
void MAT::MultLeft(const MAT mat) {
    this->operator=(Mult(*this, mat));
}

//! *this = mat * *this
void MAT::MultRight(const MAT mat) {
    MAT tmp = Mult(mat, *this);
    this->operator=(tmp);

}

//! convert the data CSR format to CSRx format
MAT ConvertFromCSR(const INT row, const INT col, const INT nnz,
                   const std::vector<DBL> values,
                   const std::vector<INT> rowPtr,
                   const std::vector<INT> colInd) {

    MAT mat;
    if ( row == 0 || col == 0 || nnz == 0 ) {
        mat.row = 0;
        mat.col = 0;
        mat.nnz = 0;
        mat.diagPtr.resize(0);
        mat.rowPtr.resize(0);
        mat.values.resize(0);
        mat.colInd.resize(0);
        return mat;
    }

    INT begin, end;
    std::vector<INT> tmprowPtr, tmpcolInd;
    std::vector<DBL> tmpvalues;
    tmprowPtr.operator=(rowPtr);
    tmpcolInd.operator=(colInd);
    tmpvalues.operator=(values);

    Sort(row, tmprowPtr, tmpcolInd, tmpvalues);

    INT flag = 0;
    mat.row = row;
    mat.col = col;
    mat.nnz = 0;
    mat.rowPtr.resize(row + 1);
    mat.rowPtr[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            mat.nnz++;
            mat.rowPtr[j + 1] = mat.rowPtr[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                mat.nnz++;
                mat.rowPtr[j + 1] = mat.rowPtr[j] + 1;
            } else {
                mat.nnz += 2;
                mat.rowPtr[j + 1] = mat.rowPtr[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k] < tmpcolInd[k + 1] ) {
                mat.nnz++;
                if ( tmpcolInd[k] == j ) {
                    flag = 1;
                }
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] ) {
            mat.nnz++;
            if ( tmpcolInd[end - 1] == j ) {
                flag = 1;
            }
        }

        if ( flag != 1 ) {
            mat.nnz++;
            mat.rowPtr[j + 1] = mat.rowPtr[j] + end - begin + 1;
        } else {
            mat.rowPtr[j + 1] = mat.rowPtr[j] + end - begin;
            flag = 0;
        }
    }

    mat.colInd.resize(mat.nnz);
    mat.values.resize(mat.nnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            mat.colInd[count] = j;
            mat.values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                mat.colInd[count] = j;
                mat.values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolInd[begin] > j ) {
                    mat.colInd[count] = j;
                    mat.values[count] = 0.0;
                    count++;
                    mat.colInd[count] = tmpcolInd[begin];
                    mat.values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolInd[begin] < j ) {
                    mat.colInd[count] = tmpcolInd[begin];
                    mat.values[count] = tmpvalues[begin];
                    count++;
                    mat.colInd[count] = j;
                    mat.values[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( tmpcolInd[begin + 1] < j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                mat.colInd[count] = tmpcolInd[begin];
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colInd[count] = tmpcolInd[begin + 1];
                mat.values[count] = tmpvalues[begin + 1];
                count++;
                mat.colInd[count] = j;
                mat.values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                mat.colInd[count] = tmpcolInd[begin];
                mat.values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                mat.colInd[count] = j;
                mat.values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j && tmpcolInd[begin + 1] == j ) {
                mat.colInd[count] = tmpcolInd[begin];
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colInd[count] = j;
                mat.values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] == j ) {
                mat.colInd[count] = j;
                mat.values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] > j ) {
                mat.colInd[count] = j;
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colInd[count] = tmpcolInd[begin + 1];
                mat.values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                mat.colInd[count] = j;
                mat.values[count] = 0.0;
                count++;
                mat.colInd[count] = tmpcolInd[begin];
                mat.values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                mat.colInd[count] = j;
                mat.values[count] = 0.0;
                count++;
                mat.colInd[count] = tmpcolInd[begin];
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colInd[count] = tmpcolInd[begin + 1];
                mat.values[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k + 1] < j && tmpcolInd[k] < tmpcolInd[k + 1] ) {
                mat.colInd[count] = tmpcolInd[k];
                mat.values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                mat.colInd[count] = tmpcolInd[k];
                mat.values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] == j ) {
                mat.colInd[count] = tmpcolInd[k];
                mat.values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] == j ) {
                mat.colInd[count] = j;
                mat.values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] > j ) {
                mat.colInd[count] = tmpcolInd[k];
                mat.values[count] += tmpvalues[k];
                count++;
                mat.colInd[count] = j;
                mat.values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] > j ) {
                mat.colInd[count] = j;
                mat.values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                mat.colInd[count] = tmpcolInd[k];
                mat.values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k + 1] > tmpcolInd[k] ) {
                mat.colInd[count] = tmpcolInd[k];
                mat.values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            mat.colInd[count] = tmpcolInd[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
            mat.colInd[count] = j;
            mat.values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            mat.colInd[count] = tmpcolInd[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
            mat.colInd[count] = j;
            mat.values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            mat.colInd[count] = j;
            mat.values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            mat.colInd[count] = j;
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] < j && tmpcolInd[end - 1] > j ) {
            mat.colInd[count] = tmpcolInd[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 1] > tmpcolInd[end - 2] ) {
            mat.colInd[count] = tmpcolInd[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] == j && tmpcolInd[end - 1] > j ) {
            mat.colInd[count] = tmpcolInd[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 2] == tmpcolInd[end - 1] ) {
            mat.colInd[count] = tmpcolInd[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    mat.diagPtr.resize(row > col ? col : row);
    //! compute mat.diagPtr
    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = mat.rowPtr[j];
        end = mat.rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( mat.colInd[k] == j ) {
                mat.diagPtr[count] = k;
                count++;
            }
        }
    }

    return mat;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/