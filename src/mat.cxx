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

//! assign row, column, nnz, values, rowshift, colindex, diag to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
values, const std::vector<INT> rowshift, const std::vector<INT> colindex, const
         std::vector<INT> diag) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->rowshift_.resize(0);
        this->diag_.resize(0);
        this->colindex_.resize(0);
        this->values_.resize(0);
        return;
    }

    this->row_ = row;
    this->column_ = column;
    this->nnz_ = nnz;
    this->values_.operator=(values);
    this->rowshift_.operator=(rowshift);
    this->colindex_.operator=(colindex);
    this->diag_.operator=(diag);
}

//! assign row, column, nnz, rowshift, colindex, diag to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
rowshift, const std::vector<INT> colindex, const std::vector<INT> diag) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->colindex_.resize(0);
        this->values_.resize(0);
        this->rowshift_.resize(0);
        this->diag_.resize(0);
        return;
    }

    this->row_ = row;
    this->column_ = column;
    this->nnz_ = nnz;
    this->values_.resize(0);
    this->rowshift_.operator=(rowshift);
    this->colindex_.operator=(colindex);
    this->diag_.operator=(diag);
}

//! assign row, column, nnz, values, rowshift, colindex to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
values, const std::vector<INT> rowshift, const std::vector<INT> colindex) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->diag_.resize(0);
        this->rowshift_.resize(0);
        this->values_.resize(0);
        this->colindex_.resize(0);
        return;
    }

    this->row_ = row;
    this->column_ = column;
    this->nnz_ = nnz;
    this->values_ = values;
    this->rowshift_ = rowshift;
    this->colindex_ = colindex;

    this->diag_.resize(row > column ? column : row);
    //! compute this->diag_
    INT count = 0;
    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowshift_[j];
        end = this->rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colindex_[k] == j ) {
                this->diag_[count] = k;
                count++;
            }
        }
    }
}

//! assign row, column, nnz, rowshift, colindex to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
rowshift, const std::vector<INT> colindex) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->colindex_.resize(0);
        this->diag_.resize(0);
        this->rowshift_.resize(0);
        this->values_.resize(0);
        return;
    }

    this->row_=row;
    this->column_=column;
    this->nnz_=nnz;
    this->rowshift_.operator=(rowshift);
    this->colindex_.operator=(colindex);
    this->diag_.resize(0);

    this->diag_.resize(row > column ? column : row);
    //! compute this->diag_
    INT count = 0;
    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowshift_[j];
        end = this->rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colindex_[k] == j ) {
                this->diag_[count] = k;
                count++;
            }
        }
    }
}

//! assign VEC object to form the diagonal matrix
MAT::MAT(VEC &vec) {
    INT size = vec.GetSize();
    if ( size == 0 ) {
        this->column_ = 0;
        this->row_ = 0;
        this->nnz_ = 0;
        this->values_.resize(0);
        this->colindex_.resize(0);
        this->rowshift_.resize(0);
        this->diag_.resize(0);
    } else {
        INT *array = new INT[size];
        for ( INT j = 0; j < size; j++ )
            array[j] = j;

        this->row_ = size;
        this->column_ = size;
        this->nnz_ = size;
        this->rowshift_.resize(size + 1);
        this->rowshift_.assign(array, array + size);
        this->rowshift_[size] = size;
        this->colindex_.resize(size);
        this->colindex_.assign(array, array + size);
        this->values_.resize(size);

        for ( INT j = 0; j < size; j++ )
            this->values_[j] = vec[j];

        this->diag_.resize(size);
        this->diag_.assign(array, array + size);

        delete[] array;
        array = nullptr;
    }
}

//! assign vector object to form the diagonal matrix
MAT::MAT(const std::vector<DBL> &vect) {
    INT size = vect.size();
    if ( size == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->rowshift_.resize(0);
        this->colindex_.resize(0);
        this->diag_.resize(0);
        this->values_.resize(0);
    } else {
        this->row_ = size;
        this->column_ = size;
        this->nnz_ = size;

        INT *p = new INT[size];
        for ( INT j = 0; j < size; j++ )
            p[j] = j;

        this->rowshift_.resize(size + 1);
        this->rowshift_.assign(p, p + size);
        this->rowshift_[size] = size;
        this->colindex_.resize(size);
        this->colindex_.assign(p, p + size);
        this->diag_.resize(size);
        this->diag_.assign(p, p + size);
        this->values_.resize(size);
        this->values_.assign(vect.begin(), vect.begin() + size);

        delete[] p;
        p = nullptr;
    }
}

//! assign MAT object to *this
MAT::MAT(const MAT &mat) {
    this->row_ = mat.row_;
    this->column_ = mat.column_;
    this->nnz_ = mat.nnz_;
    this->values_.operator=(mat.values_);
    this->diag_.operator=(mat.diag_);
    this->colindex_.operator=(mat.colindex_);
    this->rowshift_.operator=(mat.rowshift_);
}

//! overload = operator
MAT &MAT::operator=(const MAT &mat) {
    this->row_ = mat.row_;
    this->column_ = mat.column_;
    this->nnz_ = mat.nnz_;
    this->diag_.operator=(mat.diag_);
    this->colindex_.operator=(mat.colindex_);
    this->rowshift_.operator=(mat.rowshift_);
    this->values_.operator=(mat.values_);
}

//! assign row, column, nnz, values, rowshift, colindex, diag to *this
void MAT::SetValues(const INT row, const INT column, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowshift,
                    const std::vector<INT> colindex, const std::vector<INT> diag) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->rowshift_.resize(0);
        this->diag_.resize(0);
        this->colindex_.resize(0);
        this->values_.resize(0);
        return;
    }

    this->row_ = row;
    this->column_ = column;
    this->nnz_ = nnz;
    this->values_.operator=(values);
    this->rowshift_.operator=(rowshift);
    this->colindex_.operator=(colindex);
    this->diag_.operator=(diag);
}

//! assign row, column, nnz, values, rowshift, colindex to *this
void MAT::SetValues(const INT row, const INT column, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowshift,
                    const std::vector<INT> colindex) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row_ = 0;
        this->column_ = 0;
        this->nnz_ = 0;
        this->rowshift_.resize(0);
        this->diag_.resize(0);
        this->colindex_.resize(0);
        this->values_.resize(0);
        return;
    }

    this->row_=row;
    this->column_=column;
    this->nnz_=nnz;
    this->rowshift_.operator=(rowshift);
    this->colindex_.operator=(colindex);
    this->values_.operator=(values);

    this->diag_.resize(row > column ? column : row);
    //! compute this->diag_
    INT count = 0;
    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowshift_[j];
        end = this->rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colindex_[k] == j ) {
                this->diag_[count] = k;
                count++;
            }
        }
    }
}

//! get the row or column number of *this
void MAT::GetSizes(INT &row, INT &column) const {
    row = this->row_;
    column = this->column_;
}

//! get the row
INT MAT::GetRow() const {
    return this->row_;
}

//! get the column
INT MAT::GetColumn() const {
    return this->column_;
}

//! get this->nnz_
INT MAT::Getnnz() const {
    return this->nnz_;
}

//! get (*this)[i][j]
DBL MAT::GetElem(const INT row, const INT column) const {
    if ( this->colindex_[this->rowshift_[row]] <= column ) {
        for ( INT j = this->rowshift_[row]; j < this->rowshift_[row + 1]; j++ ) {
            if ( column == this->colindex_[j] ) {
                if ( this->values_.size() == 0 ) {
                    return 1.0;
                } else {
                    return (this->values_[j]);
                }
            }
        }
    }
    return 0;
}

//! get the whole jth-row elements in *this into vector object
std::vector<DBL> MAT::GetRow(const INT row) const {
    std::vector<DBL> vec;
    INT len = this->rowshift_[row + 1] - this->rowshift_[row];
    vec.resize(len);
    if ( this->values_.size() == 0 )
        vec.assign(len, 1);

    INT k = 0;
    for ( INT j = this->rowshift_[row]; j < this->rowshift_[row + 1]; j++ ) {
        vec[k] = this->values_[j];
        k++;
    }

    return vec;
}

//! get the whole jth-column elements in *this into vector object
std::vector<DBL> MAT::GetColumn(const INT column) const {
    std::vector<DBL> tmp;
    INT count = 0;
    tmp.resize(this->row_);
    for ( INT j = 0; j < this->row_; j++ ) {
        if ( column >= this->colindex_[this->rowshift_[j]] ) {
            for ( INT k = this->rowshift_[j]; k < this->rowshift_[j + 1]; k++ ) {
                if ( this->colindex_[k] == column ) {
                    if ( this->values_.size() == 0 ) {
                        tmp[count] = 1;
                    } else {
                        tmp[count] = this->values_[k];
                    }
                    count++;
                    break;
                }
            }
        }
    }

    std::vector<DBL> vec(count);

    for(int j=0;j<count;j++)
        vec[j]=tmp[j];

    return vec;
}

//! get the whole diagonal elements in *this into VEC object
std::vector<DBL> MAT::GetDiag() const {
    std::vector<DBL> vec;
    INT len = this->row_ > this->column_ ? this->column_ : this->row_;
    vec.resize(len);
    if ( this->values_.size() != 0 ) {
        for ( INT j = 0; j < len; j++ )
            vec[j] = this->values_[this->diag_[j]];
    } else {
        vec.assign(len, 1);
    }

    return vec;
}

//! zero all the elements
void MAT::Zero() {
    this->nnz_ = this->row_ > this->column_ ? this->column_ : this->row_;
    INT *pcol = new INT[this->nnz_];
    INT *prow = new INT[this->row_ + 1];

    for ( INT j = 0; j < this->diag_.size(); j++ )
        pcol[j] = j;

    for ( INT j = 0; j < this->row_ + 1; j++ )
        prow[j] = j;

    std::vector<INT> rowshift_tmp(this->row_ + 1);
    std::vector<INT> colindex_tmp(this->nnz_);
    std::vector<DBL> values_tmp(this->nnz_);

    rowshift_tmp.assign(prow, prow + this->row_);
    colindex_tmp.assign(pcol, pcol + this->nnz_);
    values_tmp.assign(this->nnz_, 0);

    this->rowshift_.operator=(rowshift_tmp);
    this->colindex_.operator=(colindex_tmp);
    this->values_.operator=(values_tmp);

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
    if ( this->values_.size() == 0 ) {
        this->values_.resize(this->nnz_);
        for ( INT j = 0; j < this->nnz_; j++ )
            this->values_[j] = a;
    } else {
        for ( INT j = 0; j < this->nnz_; j++ )
            this->values_[j] = a * this->values_[j];
    }
}

//! *this = a * I + *this
void MAT::Shift(const DBL a) {
    if ( this->values_.size() == 0 ) {
        if ( a == 0 )
            return;
        for ( int j = 0; j < this->diag_.size(); j++ )
            this->values_[j] = 1.0;
        for ( int j = 0; j < diag_.size(); j++ )
            this->values_[this->diag_[j]] = 1 + a;
    } else {
        for ( INT j = 0; j < this->diag_.size(); j++ )
            this->values_[this->diag_[j]] = a + this->values_[this->diag_[j]];
    }
}

//! ReturnedValues = *this * vec
VEC MAT::MultVec(const VEC vec) const {
    VEC vec_;

    INT begin, end;
    vec_.Reserve(this->row_);
    if ( this->values_.size() == 0 ) {
        for ( INT j = 0; j < this->row_; j++ ) {
            begin = this->rowshift_[j];
            end = this->rowshift_[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec_[j] += vec[this->colindex_[k]];
            }
        }
    } else {
        for ( INT j = 0; j < this->row_; j++ ) {
            begin = this->rowshift_[j];
            end = this->rowshift_[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec_[j] += this->values_[k] * vec[this->colindex_[k]];
            }
        }
    }

    return vec_;
}

//! transpose *this
void MAT::Transpose() {
    MAT tmp;
    tmp.row_ = this->column_;
    tmp.column_ = this->row_;
    tmp.nnz_ = this->nnz_;

    tmp.rowshift_.resize(tmp.row_ + 1);
    tmp.colindex_.resize(tmp.nnz_);
    tmp.values_.resize(tmp.nnz_);
    tmp.diag_.resize(tmp.row_ > tmp.column_ ? tmp.column_ : tmp.row_);

    INT begin, end;
    INT count = 0;
    tmp.rowshift_[0] = 0;

    if ( this->values_.size() != 0 ) {
        for ( INT j = 0; j < tmp.row_; j++ ) {
            for ( INT k = 0; k < this->row_; k++ ) {
                begin = this->rowshift_[k];
                end = this->rowshift_[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex_[l] == j ) {
                        tmp.rowshift_[j + 1]++;
                        tmp.colindex_[count] = k;
                        tmp.values_[count] = this->values_[l];
                        count++;
                        break;
                    }
                }
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row_; j++ ) {
            for ( INT k = 0; k < this->row_; k++ ) {
                begin = this->rowshift_[k];
                end = this->rowshift_[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex_[l] == j ) {
                        tmp.rowshift_[j + 1]++;
                        tmp.colindex_[count] = k;
                        count++;
                        break;
                    }
                }
            }
        }
    }

    for ( INT j = 0; j < tmp.row_; j++ )
        tmp.rowshift_[j + 1] += tmp.rowshift_[j];

    count = 0;
    for ( INT j = 0; j < tmp.row_; j++ ) {
        begin = tmp.rowshift_[j];
        end = tmp.rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( tmp.colindex_[k] == j ) {
                tmp.diag_[count] = k;
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
    tmp.row_ = this->column_;
    tmp.column_ = this->row_;
    tmp.nnz_ = this->nnz_;

    tmp.rowshift_.resize(tmp.row_ + 1);
    tmp.colindex_.resize(tmp.nnz_);
    tmp.values_.resize(tmp.nnz_);

    INT begin, end;
    INT count = 0;
    tmp.rowshift_[0] = 0;

    if ( this->values_.size() != 0 ) {
        for ( INT j = 0; j < tmp.row_; j++ ) {
            for ( INT k = 0; k < this->row_; k++ ) {
                begin = this->rowshift_[k];
                end = this->rowshift_[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex_[l] == j ) {
                        tmp.rowshift_[j + 1]++;
                        tmp.colindex_[count] = k;
                        tmp.values_[count] = this->values_[l];
                        count++;
                        break;
                    }
                }
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row_; j++ ) {
            for ( INT k = 0; k < this->row_; k++ ) {
                begin = this->rowshift_[k];
                end = this->rowshift_[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex_[l] == j ) {
                        tmp.rowshift_[j + 1]++;
                        tmp.colindex_[count] = k;
                        count++;
                        break;
                    }
                }
            }
        }
    }

    for ( INT j = 0; j < tmp.row_; j++ )
        tmp.rowshift_[j + 1] += tmp.rowshift_[j];

    if ( this->values_.size()) {
        for ( INT j = 0; j < tmp.row_; j++ ) {
            begin = tmp.rowshift_[j];
            end = tmp.rowshift_[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec[j] += vec1[this->colindex_[k]] * tmp.values_[k];
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row_; j++ ) {
            begin = tmp.rowshift_[j];
            end = tmp.rowshift_[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec[j] += vec1[this->colindex_[k]];
            }
        }
    }

    return vec;
}

//! *this = a * *this + b * mat
void MAT::Add(const DBL a, const DBL b, const MAT mat) {

    if ( this->nnz_ == 0 )
        this->operator=(mat);

    if ( mat.nnz_ == 0 )
        return;

    MAT tmp1, tmp2;
    tmp1.row_ = this->row_;
    tmp1.column_ = this->column_;
    tmp1.rowshift_.resize(this->row_ + 1);
    tmp1.colindex_.resize(this->nnz_ + mat.nnz_);
    tmp1.values_.resize(this->nnz_ + mat.nnz_);
    tmp1.diag_.resize(this->row_ > this->column_ ? this->column_ : this->row_);

    INT begin1, end1, begin2, end2;
    INT count;

    count = 0;
    for ( int j = 0; j < this->row_; j++ ) {
        begin1 = this->rowshift_[j];
        end1 = this->rowshift_[j + 1];
        begin2 = mat.rowshift_[j];
        end2 = mat.rowshift_[j + 1];
        if ( this->values_.size() != 0 && mat.values_.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colindex_[count] = this->colindex_[k];
                tmp1.values_[count] = a * this->values_[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colindex_[count] = mat.colindex_[k];
                tmp1.values_[count] = b * mat.values_[k];
                count++;
            }
        }
        if ( this->values_.size() == 0 && mat.values_.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colindex_[count] = this->colindex_[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colindex_[count] = mat.colindex_[k];
                count++;
            }
        }
    }

    tmp1.rowshift_[0] = 0;
    for ( int j = 0; j < this->row_; j++ )
        tmp1.rowshift_[j + 1] = this->rowshift_[j + 1] + mat.rowshift_[j + 1];

    Sort(tmp1.row_, tmp1.rowshift_, tmp1.colindex_, tmp1.values_);

    tmp2.row_ = tmp1.row_;
    tmp2.column_ = tmp1.column_;
    tmp2.nnz_ = 0;
    tmp2.rowshift_.resize(tmp2.row_ + 1);
    tmp2.rowshift_[0] = 0;

    INT mem;
    INT begin, end;
    for ( INT j = 0; j < this->row_; j++ ) {
        begin = tmp1.rowshift_[j];
        end = tmp1.rowshift_[j + 1];
        if ( begin == end - 1 ) {
            tmp2.nnz_++;
            tmp2.rowshift_[j + 1] += 1;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp1.colindex_[k] < tmp1.colindex_[k + 1] ) {
                tmp2.nnz_++;
                tmp2.rowshift_[j + 1] += 1;
                mem = tmp1.colindex_[k];
            }
        }
        if ( mem < tmp1.colindex_[end - 1] ) {
            tmp2.nnz_++;
            tmp2.rowshift_[j + 1] += 1;
        }
    }

    for ( int j = 1; j < this->row_ + 1; j++ )
        tmp2.rowshift_[j] += tmp2.rowshift_[j - 1];

    tmp2.colindex_.resize(tmp2.nnz_);
    tmp2.values_.resize(tmp2.nnz_);
    tmp2.diag_.resize(this->row_ > this->column_ ? this->column_ : this->row_);

    count = 0;
    for ( INT j = 0; j < tmp1.row_; j++ ) {
        begin = tmp1.rowshift_[j];
        end = tmp1.rowshift_[j + 1];
        if ( begin == end - 1 ) {
            tmp2.colindex_[count] = j;
            tmp2.values_[count] = tmp1.values_[begin];
            count++;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp1.colindex_[k] < tmp1.colindex_[k + 1] ) {
                tmp2.colindex_[count] = tmp1.colindex_[k];
                tmp2.values_[count] += tmp1.values_[k];
                count++;
            }
            if ( tmp1.colindex_[k] == tmp1.colindex_[k + 1] ) {
                tmp2.colindex_[count] = tmp1.colindex_[k];
                tmp2.values_[count] += tmp1.values_[k];
            }
        }

        tmp2.colindex_[count] = tmp1.colindex_[end - 1];
        tmp2.values_[count] += tmp1.values_[end - 1];
        count++;
    }

    //! compute this->diag_
    count = 0;

    for ( INT j = 0; j < tmp2.row_; j++ ) {
        begin = tmp2.rowshift_[j];
        end = tmp2.rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( tmp2.colindex_[k] == j ) {
                tmp2.diag_[count] = k;
                count++;
            }
        }
    }

    this->operator=(tmp2);
}

//! ReturnedValues = a * mat1 + b * mat2
MAT Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2) {
    MAT mat;

    if ( mat1.nnz_ == 0 ) {
        mat.row_ = mat2.row_;
        mat.column_ = mat2.column_;
        mat.nnz_ = mat2.nnz_;
        mat.rowshift_.operator=(mat2.rowshift_);
        mat.colindex_.operator=(mat2.colindex_);
        mat.diag_.operator=(mat2.diag_);
        for ( int j = 0; j < mat2.nnz_; j++ )
            mat.values_[j] = b * mat2.values_[j];

        return mat;
    }

    if ( mat2.nnz_ == 0 ) {
        mat.row_ = mat1.row_;
        mat.column_ = mat1.column_;
        mat.nnz_ = mat1.nnz_;
        mat.rowshift_.operator=(mat1.rowshift_);
        mat.colindex_.operator=(mat1.colindex_);
        mat.diag_.operator=(mat1.diag_);
        for ( int j = 0; j < mat1.nnz_; j++ )
            mat.values_[j] = a * mat1.values_[j];

        return mat;
    }

    MAT tmp;
    tmp.row_ = mat1.row_;
    tmp.column_ = mat1.column_;
    tmp.rowshift_.resize(mat1.row_ + 1);
    tmp.colindex_.resize(mat1.nnz_ + mat2.nnz_);
    tmp.values_.resize(mat1.nnz_ + mat2.nnz_);
    tmp.diag_.resize(mat1.row_ > mat1.column_ ? mat1.column_ : mat1.row_);

    INT begin1, end1, begin2, end2;
    INT count;

    count = 0;
    for ( int j = 0; j < mat1.row_; j++ ) {
        begin1 = mat1.rowshift_[j];
        end1 = mat1.rowshift_[j + 1];
        begin2 = mat2.rowshift_[j];
        end2 = mat2.rowshift_[j + 1];
        if ( mat1.values_.size() != 0 && mat2.values_.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colindex_[count] = mat1.colindex_[k];
                tmp.values_[count] = a * mat1.values_[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colindex_[count] = mat2.colindex_[k];
                tmp.values_[count] = b * mat2.values_[k];
                count++;
            }
        }
        if ( mat1.values_.size() == 0 && mat2.values_.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colindex_[count] = mat1.colindex_[k];
                tmp.values_[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colindex_[count] = mat2.colindex_[k];
                tmp.values_[count] = b;
                count++;
            }
        }
    }

    tmp.rowshift_[0] = 0;
    for ( int j = 0; j < mat1.row_; j++ )
        tmp.rowshift_[j + 1] = mat1.rowshift_[j + 1] + mat2.rowshift_[j + 1];

    INT begin, end;

    Sort(mat1.row_, tmp.rowshift_, tmp.colindex_, tmp.values_);

    mat.row_ = tmp.row_;
    mat.column_ = tmp.column_;
    mat.nnz_ = 0;
    mat.rowshift_.resize(mat.row_ + 1);
    mat.rowshift_[0] = 0;

    INT mem;
    for ( INT j = 0; j < mat1.row_; j++ ) {
        begin = tmp.rowshift_[j];
        end = tmp.rowshift_[j + 1];
        if ( begin == end - 1 ) {
            mat.nnz_++;
            mat.rowshift_[j + 1] += 1;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp.colindex_[k] < tmp.colindex_[k + 1] ) {
                mat.nnz_++;
                mat.rowshift_[j + 1] += 1;
                mem = tmp.colindex_[k];
            }
        }
        if ( mem < tmp.colindex_[end - 1] ) {
            mat.nnz_++;
            mat.rowshift_[j + 1] += 1;
        }
    }

    for ( int j = 1; j < mat1.row_ + 1; j++ )
        mat.rowshift_[j] += mat.rowshift_[j - 1];

    mat.colindex_.resize(mat.nnz_);
    mat.values_.resize(mat.nnz_);
    mat.diag_.resize(mat1.row_ > mat1.column_ ? mat1.column_ : mat1.row_);

    count = 0;
    for ( INT j = 0; j < tmp.row_; j++ ) {
        begin = tmp.rowshift_[j];
        end = tmp.rowshift_[j + 1];
        if ( begin == end - 1 ) {
            mat.colindex_[count] = j;
            mat.values_[count] = tmp.values_[begin];
            count++;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp.colindex_[k] < tmp.colindex_[k + 1] ) {
                mat.colindex_[count] = tmp.colindex_[k];
                mat.values_[count] += tmp.values_[k];
                count++;
            }
            if ( tmp.colindex_[k] == tmp.colindex_[k + 1] ) {
                mat.colindex_[count] = tmp.colindex_[k];
                mat.values_[count] += tmp.values_[k];
            }
        }

        mat.colindex_[count] = tmp.colindex_[end - 1];
        mat.values_[count] += tmp.values_[end - 1];
        count++;
    }

    //! compute this->diag_
    count = 0;

    for ( INT j = 0; j < mat.row_; j++ ) {
        begin = mat.rowshift_[j];
        end = mat.rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( mat.colindex_[k] == j ) {
                mat.diag_[count] = k;
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
    INT *tmp = new INT[matr.column_];

    mat.row_ = matl.row_;
    mat.column_ = matr.column_;
    mat.rowshift_.resize(mat.row_ + 1);

    for ( INT i = 0; i < matr.column_; i++ )
        tmp[i] = -1;

    for ( INT i = 0; i < mat.row_; i++ ) {
        count = 0;
        for ( INT k = matl.rowshift_[i]; k < matl.rowshift_[i + 1]; k++ ) {
            for ( INT j = matr.rowshift_[matl.colindex_[k]];
                  j < matr.rowshift_[matl.colindex_[k] + 1]; j++ ) {
                for ( l = 0; l < count; l++ ) {
                    if ( tmp[l] == matr.colindex_[j] )
                        break;
                }
                if ( l == count ) {
                    tmp[count] = matr.colindex_[j];
                    count++;
                }
            }
        }
        mat.rowshift_[i + 1] = count;
        for ( INT j = 0; j < count; j++ )
            tmp[j] = -1;
    }

    for ( INT i = 0; i < mat.row_; i++ )
        mat.rowshift_[i + 1] += mat.rowshift_[i];

    INT count_tmp;

    mat.colindex_.resize(mat.rowshift_[mat.row_]);

    for ( INT i = 0; i < mat.row_; i++ ) {
        count_tmp = 0;
        count = mat.rowshift_[i];
        for ( INT k = matl.rowshift_[i]; k < matl.rowshift_[i + 1]; k++ ) {
            for ( INT j = matr.rowshift_[matl.colindex_[k]];
                  j < matr.rowshift_[matl.colindex_[k] + 1]; j++ ) {
                for ( l = 0; l < count_tmp; l++ ) {
                    if ( tmp[l] == matr.colindex_[j] )
                        break;
                }

                if ( l == count_tmp ) {
                    mat.colindex_[count] = matr.colindex_[j];
                    tmp[count_tmp] = matr.colindex_[j];
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

    mat.values_.resize(mat.rowshift_[mat.row_]);

    for ( INT i = 0; i < mat.row_; i++ ) {
        for ( INT j = mat.rowshift_[i]; j < mat.rowshift_[i + 1]; j++ ) {
            mat.values_[j] = 0;
            for ( INT k = matl.rowshift_[i]; k < matl.rowshift_[i + 1]; k++ ) {
                for ( l = matr.rowshift_[matl.colindex_[k]];
                      l < matr.rowshift_[matl.colindex_[k] + 1]; l++ ) {
                    if ( matr.colindex_[l] == mat.colindex_[j] )
                        mat.values_[j] += matl.values_[k] * matr.values_[l];
                }
            }
        }
    }

    mat.nnz_ = mat.rowshift_[mat.row_] - mat.rowshift_[0];

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
MAT ConvertCSR(const INT row, const INT column, const INT nnz,
               const std::vector<DBL> values,
               const std::vector<INT> rowshift,
               const std::vector<INT> colindex) {

    MAT mat;
    if ( row == 0 || column == 0 || nnz == 0 ) {
        mat.row_ = 0;
        mat.column_ = 0;
        mat.nnz_ = 0;
        mat.diag_.resize(0);
        mat.rowshift_.resize(0);
        mat.values_.resize(0);
        mat.colindex_.resize(0);
        return mat;
    }

    INT begin, end;
    std::vector<INT> tmprowshift, tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.operator=(values);

    Sort(row, tmprowshift, tmpcolindex, tmpvalues);

    INT flag = 0;
    mat.row_ = row;
    mat.column_ = column;
    mat.nnz_ = 0;
    mat.rowshift_.resize(row + 1);
    mat.rowshift_[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            mat.nnz_++;
            mat.rowshift_[j + 1] = mat.rowshift_[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                mat.nnz_++;
                mat.rowshift_[j + 1] = mat.rowshift_[j] + 1;
            } else {
                mat.nnz_ += 2;
                mat.rowshift_[j + 1] = mat.rowshift_[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k] < tmpcolindex[k + 1] ) {
                mat.nnz_++;
                if ( tmpcolindex[k] == j ) {
                    flag = 1;
                }
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] ) {
            mat.nnz_++;
            if ( tmpcolindex[end - 1] == j ) {
                flag = 1;
            }
        }

        if ( flag != 1 ) {
            mat.nnz_++;
            mat.rowshift_[j + 1] = mat.rowshift_[j] + end - begin + 1;
        } else {
            mat.rowshift_[j + 1] = mat.rowshift_[j] + end - begin;
            flag = 0;
        }
    }

    mat.colindex_.resize(mat.nnz_);
    mat.values_.resize(mat.nnz_);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            mat.colindex_[count] = j;
            mat.values_[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                mat.colindex_[count] = j;
                mat.values_[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolindex[begin] > j ) {
                    mat.colindex_[count] = j;
                    mat.values_[count] = 0.0;
                    count++;
                    mat.colindex_[count] = tmpcolindex[begin];
                    mat.values_[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolindex[begin] < j ) {
                    mat.colindex_[count] = tmpcolindex[begin];
                    mat.values_[count] = tmpvalues[begin];
                    count++;
                    mat.colindex_[count] = j;
                    mat.values_[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( tmpcolindex[begin + 1] < j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
                mat.colindex_[count] = tmpcolindex[begin];
                mat.values_[count] = tmpvalues[begin];
                count++;
                mat.colindex_[count] = tmpcolindex[begin + 1];
                mat.values_[count] = tmpvalues[begin + 1];
                count++;
                mat.colindex_[count] = j;
                mat.values_[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                mat.colindex_[count] = tmpcolindex[begin];
                mat.values_[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                mat.colindex_[count] = j;
                mat.values_[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j && tmpcolindex[begin + 1] == j ) {
                mat.colindex_[count] = tmpcolindex[begin];
                mat.values_[count] = tmpvalues[begin];
                count++;
                mat.colindex_[count] = j;
                mat.values_[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] == j ) {
                mat.colindex_[count] = j;
                mat.values_[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] > j ) {
                mat.colindex_[count] = j;
                mat.values_[count] = tmpvalues[begin];
                count++;
                mat.colindex_[count] = tmpcolindex[begin + 1];
                mat.values_[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                mat.colindex_[count] = j;
                mat.values_[count] = 0.0;
                count++;
                mat.colindex_[count] = tmpcolindex[begin];
                mat.values_[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
                mat.colindex_[count] = j;
                mat.values_[count] = 0.0;
                count++;
                mat.colindex_[count] = tmpcolindex[begin];
                mat.values_[count] = tmpvalues[begin];
                count++;
                mat.colindex_[count] = tmpcolindex[begin + 1];
                mat.values_[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k + 1] < j && tmpcolindex[k] < tmpcolindex[k + 1] ) {
                mat.colindex_[count] = tmpcolindex[k];
                mat.values_[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                mat.colindex_[count] = tmpcolindex[k];
                mat.values_[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] == j ) {
                mat.colindex_[count] = tmpcolindex[k];
                mat.values_[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] == j ) {
                mat.colindex_[count] = j;
                mat.values_[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] > j ) {
                mat.colindex_[count] = tmpcolindex[k];
                mat.values_[count] += tmpvalues[k];
                count++;
                mat.colindex_[count] = j;
                mat.values_[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] > j ) {
                mat.colindex_[count] = j;
                mat.values_[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                mat.colindex_[count] = tmpcolindex[k];
                mat.values_[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k + 1] > tmpcolindex[k] ) {
                mat.colindex_[count] = tmpcolindex[k];
                mat.values_[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            mat.colindex_[count] = tmpcolindex[end - 1];
            mat.values_[count] += tmpvalues[end - 1];
            count++;
            mat.colindex_[count] = j;
            mat.values_[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            mat.colindex_[count] = tmpcolindex[end - 1];
            mat.values_[count] += tmpvalues[end - 1];
            count++;
            mat.colindex_[count] = j;
            mat.values_[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            mat.colindex_[count] = j;
            mat.values_[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            mat.colindex_[count] = j;
            mat.values_[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] < j && tmpcolindex[end - 1] > j ) {
            mat.colindex_[count] = tmpcolindex[end - 1];
            mat.values_[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 1] > tmpcolindex[end - 2] ) {
            mat.colindex_[count] = tmpcolindex[end - 1];
            mat.values_[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] == j && tmpcolindex[end - 1] > j ) {
            mat.colindex_[count] = tmpcolindex[end - 1];
            mat.values_[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 2] == tmpcolindex[end - 1] ) {
            mat.colindex_[count] = tmpcolindex[end - 1];
            mat.values_[count] += tmpvalues[end - 1];
            count++;
        }
    }

    mat.diag_.resize(row > column ? column : row);
    //! compute mat.diag_
    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = mat.rowshift_[j];
        end = mat.rowshift_[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( mat.colindex_[k] == j ) {
                mat.diag_[count] = k;
                count++;
            }
        }
    }
}