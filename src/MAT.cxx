/*! \file MAT.cxx
 *  \brief Matrix class declaration
 *  \author Kailei Zhang
 *  \date Sep/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "MAT.hxx"
#include "MATUtil.hxx"

/// Assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
MAT::MAT(const INT& nrow, const INT& ncol, const INT& nnz,
         const std::vector<DBL>& values, const std::vector<INT>& colInd,
         const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr)
{
    if ( nrow == 0 || ncol == 0 || nnz == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->ncol = ncol;
    this->nnz  = nnz;
    this->values.operator=(values);
    this->colInd.operator=(colInd);
    this->rowPtr.operator=(rowPtr);
    this->diagPtr.operator=(diagPtr);
}

/// Assign nrow, ncol, nnz, values, colInd, rowPtr to *this, generate diagPtr
MAT::MAT(const INT& nrow, const INT& ncol, const INT& nnz,
         const std::vector<INT>& colInd, const std::vector<INT>& rowPtr,
         const std::vector<INT>& diagPtr)
{
    if ( nrow == 0 || ncol == 0 || nnz == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->ncol = ncol;
    this->nnz  = nnz;
    this->values.resize(0);
    this->colInd.operator=(colInd);
    this->rowPtr.operator=(rowPtr);
    this->diagPtr.operator=(diagPtr);
}

/// Assign nrow, ncol, nnz, colInd, rowPtr, diagPtr to *this (sparse structure)
MAT::MAT(const INT& nrow, const INT& ncol, const INT& nnz,
         const std::vector<DBL>& values, const std::vector<INT>& colInd,
         const std::vector<INT>& rowPtr)
{
    if ( nrow == 0 || ncol == 0 || nnz == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->ncol = ncol;
    this->nnz  = nnz;
    this->values = values;
    this->colInd = colInd;
    this->rowPtr = rowPtr;
    this->FormDiagPtr(this->diagPtr);
}

/// Assign nrow, ncol, nnz, colInd, rowPtr to *this (sparse structure), generate diagPtr
MAT::MAT(const INT& nrow, const INT& ncol, const INT& nnz,
         const std::vector<INT>& colInd, const std::vector<INT>& rowPtr)
{
    if ( nrow == 0 || ncol == 0 || nnz == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->ncol = ncol;
    this->nnz  = nnz;
    this->values.resize(0);
    this->colInd.operator=(colInd);
    this->rowPtr.operator=(rowPtr);
    this->FormDiagPtr(this->diagPtr);
}

/// Assign diagonal values to *this from a VEC
MAT::MAT(const VEC& v)
{
    INT size = v.GetSize();

    if ( size == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = size;
    this->ncol = size;
    this->nnz  = size;

    // Set values from v
    this->values.resize(size);
    for ( INT j = 0; j < size; j++ ) this->values[j] = v[j];

    // Set colInd to {0, 1, ..., size-1}
    INT *p = new INT[size + 1];
    for ( INT j = 0; j <= size; j++ ) p[j] = j;
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

/// Assign diagonal values to *this from a vector
MAT::MAT(const std::vector<DBL>& vt)
{
    const INT size = vt.size();

    if ( size == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = size;
    this->ncol = size;
    this->nnz  = size;

    // Set values from vt
    this->values.resize(size);
    this->values.assign(vt.begin(), vt.begin() + size);

    // Set colInd to {0, 1, ..., size-1}
    INT *p = new INT[size + 1];
    for ( INT j = 0; j <= size; j++ ) p[j] = j;
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

/// Assign MAT object to *this
MAT::MAT(const MAT& mat) {
    this->nrow = mat.nrow;
    this->ncol = mat.ncol;
    this->nnz  = mat.nnz;
    this->values.operator=(mat.values);
    this->diagPtr.operator=(mat.diagPtr);
    this->colInd.operator=(mat.colInd);
    this->rowPtr.operator=(mat.rowPtr);
}

/// Overload = operator
MAT& MAT::operator=(const MAT& mat) {
    this->nrow = mat.nrow;
    this->ncol = mat.ncol;
    this->nnz  = mat.nnz;
    this->values.operator=(mat.values);
    this->colInd.operator=(mat.colInd);
    this->rowPtr.operator=(mat.rowPtr);
    this->diagPtr.operator=(mat.diagPtr);
    return *this;
}

/// Assign nrow, ncol, nnz, values, colInd, rowPtr, diagPtr to *this
void MAT::SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                    const std::vector<DBL>& values, const std::vector<INT>& colInd,
                    const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr)
{
    if ( nrow == 0 || ncol == 0 || nnz == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->ncol = ncol;
    this->nnz  = nnz;
    this->values.operator=(values);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diagPtr);
}

/// Assign nrow, ncol, nnz, values, rowPtr, colInd to *this.
/// TODO: need this member function? why not just use constructor?
/// TODO's anwser : if an object already is defined, and needs to be initialized,
/// we need this function.

void MAT::SetValues(const INT& nrow, const INT& ncol, const INT& nnz,
                    const std::vector<DBL>& values, const std::vector<INT>& colInd,
                    const std::vector<INT>& rowPtr)
{
    if ( nrow == 0 || ncol == 0 || nnz == 0 ) {
        this->Empty();
        return;
    }

    this->nrow = nrow;
    this->ncol = ncol;
    this->nnz  = nnz;
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->values.operator=(values);
    this->FormDiagPtr(this->diagPtr);
}

/// Get row size
INT MAT::GetRowSize() const {
    return this->nrow;
}

/// Get column size
INT MAT::GetColSize() const {
    return this->ncol;
}

/// Get number of nonzeros
INT MAT::GetNNZ() const {
    return this->nnz;
}

/// Get (*this)[i][j]
//! Note: If *this is a sparse structure, it will return 1.0 for nonzero entries
DBL MAT::GetValue(const INT& row, const INT& ncol) const {
    if ( this->colInd[this->rowPtr[row]] <= ncol ) {
        for ( INT j = this->rowPtr[row]; j < this->rowPtr[row + 1]; j++ ) {
            if ( ncol == this->colInd[j] ) {
                if ( this->values.size() == 0 ) {
                    return 1.0; // It is a sparse structure indicator
                } else {
                    return (this->values[j]);
                }
            }
        }
    }
    return 0.0;
}

/// Get the whole row-th row in *this into VEC object
void MAT::GetRow(const INT& row, std::vector<DBL>& v) const {
    const INT begin = this->rowPtr[row], end = this->rowPtr[row + 1];
    const INT len = end - begin; // TODO: only use 1 time, so do not define?
    std::cout << row << "  " << begin << "  " << end << "  " << len << "  " << std::endl;
    // TODO: do not print info, or there will be much info showed on screen
    v.resize(len);
    INT k = 0;
    for ( INT j = begin; j < end; j++ ) v[k++] = this->values[j];
}

/// Get the whole col-th column in *this into VEC object
void MAT::GetCol(const INT& col, std::vector<DBL>& v) const {
    std::vector<DBL> tmp;
    INT count = 0;
    tmp.resize(this->nrow);
    for ( INT j = 0; j < this->nrow; j++ ) {
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

    v.resize(count);
    v.assign(tmp.begin(),tmp.end());
}

/// Get the whole diagonal entries in *this into VEC object
void MAT::GetDiag(std::vector<DBL>& v) const {
    INT len = this->nrow > this->ncol ? this->ncol : this->nrow;
    v.resize(len);
    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < len; j++ )
            v[j] = this->values[this->diagPtr[j]];
    } else {
        v.assign(len, 1);
    }
}

/// Copy *this to mat
void MAT::CopyTo(MAT& mat) const {
    mat.operator=(*this);
}

/// Scale *this *= a
void MAT::Scale(const DBL a) {
    try {
        if ( this->values.empty() ) { // MAT is a sparse structure!!!
            auto retCode = FaspRetCode::ERROR_MAT_DATA;
            throw( FaspExcep(retCode, __FILE__, __FUNCTION__, __LINE__) );
        }
        for ( INT j = 0; j < this->nnz; j++ ) this->values[j] = a * this->values[j];
    }
    catch ( FaspExcep& ex ) {
        ex.LogExcep();
    }
}

/// Shift *this += a * I. TODO: maybe rename to DiagScale?
void MAT::Shift(const DBL a) {
    try {
        if ( this->values.empty() ) { // MAT is a sparse structure!!!
            auto retCode = FaspRetCode::ERROR_MAT_DATA;
            throw( FaspExcep(retCode, __FILE__, __FUNCTION__, __LINE__) );
        }
        for ( INT j = 0; j < this->diagPtr.size(); j++ )
            this->values[this->diagPtr[j]] = a + this->values[this->diagPtr[j]];
    }
    catch ( FaspExcep& ex ) {
        ex.LogExcep();
    }
}

/// Set all the entries to zero without changing matrix size
void MAT::Zero() {
    for ( INT j = 0; j < this->nnz; j++ ) values[j] = 0.0;
}

/// Transpose *this
void MAT::Transpose() {

    const INT n=this->nrow,m=this->ncol,nnz=this->nnz;

    INT i,j,k,p;

    MAT tmp;
    tmp.nrow=this->ncol;
    tmp.ncol=this->nrow;
    tmp.nnz=this->nnz;

    tmp.rowPtr.resize(this->ncol+1);
    tmp.colInd.resize(nnz);
    if(this->values.size()){
        tmp.values.resize(nnz);
    }else{
        tmp.values.resize(0);
    }

    for(INT j=0;j<nnz;j++){
        i=this->colInd[j];
        if(i<m-1)
            tmp.rowPtr[i+2]++;
    }

    for(i=2;i<=m;i++)
        tmp.rowPtr[i]+=tmp.rowPtr[i-1];

    if(this->values.size()){
        for(i=0;i<n;i++){
            INT begin=this->rowPtr[i];
            INT end=this->rowPtr[i+1];
            for(p=begin;p<end;p++){
                j=this->colInd[p]+1;
                k=tmp.rowPtr[j];
                tmp.colInd[k]=i;
                tmp.values[k]=this->values[p];
                tmp.rowPtr[j]=k+1;
            }
        }
    }else{
        for(i=0;i<n;i++){
            INT begin=this->rowPtr[i];
            INT end=this->rowPtr[i+1];
            for(p=begin;p<end;p++){
                j=this->colInd[p]+1;
                k=tmp.rowPtr[j];
                tmp.colInd[k]=i;
                tmp.rowPtr[j]=k+1;
            }
        }
    }

    this->operator=(tmp);
}

/// *this = *this * mat
void MAT::MultRight(const MAT& mat) {
    this->operator=(Mult(*this, mat));
}

/// *this = mat * *this
void MAT::MultLeft(const MAT& mat) {
    MAT tmp = Mult(mat, *this);
    this->operator=(tmp);
}

/// Return MAT = a * mat1 + b * mat2
MAT Add(const DBL a, const MAT &mat1, const DBL b, const MAT &mat2) {

    INT i, j, k, l;
    INT count = 0, added, countrow;
    MAT mat;

    if ( mat1.nnz == 0 ) {
        mat = mat2;
        mat.Scale(b);

        return mat;
    }

    if ( mat2.nnz == 0 ) {
        mat = mat1;
        mat.Scale(a);

        return mat;
    }

    mat.nrow = mat1.nrow;
    mat.ncol = mat1.ncol;

    mat.rowPtr.resize(mat.nrow + 1);

    mat.colInd.resize(mat1.nnz + mat2.nnz);
    mat.values.resize(mat1.nnz + mat2.nnz);

    mat.colInd.assign(mat1.nnz + mat2.nnz, -1);

    for ( i = 0; i < mat1.nrow; i++ ) {
        countrow = 0;
        for ( j = mat1.rowPtr[i]; j < mat1.rowPtr[i + 1]; j++ ) {
            mat.values[count] = a * mat1.values[j];
            mat.colInd[count] = mat1.colInd[j];
            mat.rowPtr[i + 1]++;
            count++;
            countrow++;
        }

        for ( k = mat2.rowPtr[i]; k < mat2.rowPtr[i + 1]; k++ ) {
            added = 0;

            for ( l = mat.rowPtr[i]; l < mat.rowPtr[i] + countrow + 1; l++ ) {
                if ( mat2.colInd[k] == mat.colInd[l] ) {
                    mat.values[l] = mat.values[l] + b * mat2.values[k];
                    added = 1;
                    break;
                }
            }
            if ( added == 0 ) {
                mat.values[count] = b * mat2.values[k];
                mat.colInd[count] = mat2.colInd[k];
                mat.rowPtr[i + 1]++;
                count++;
            }
        }
        mat.rowPtr[i + 1] += mat.rowPtr[i];
    }
    mat.nnz = count;
    mat.colInd.resize(count);
    mat.values.resize(count);

    SortCSRRow(mat.nrow, mat.ncol, mat.nnz, mat.rowPtr, mat.colInd, mat.values);

    return mat;
}

/// *this = a * *this + b * mat
void MAT::Add(const DBL a, const DBL b, const MAT& mat) {
    *this=::Add(a,*this,b,mat);
}

/// Return VEC = *this * vec.
VEC MAT::MultVec(const VEC& v) const {
    VEC w;
    w.SetSize(this->nrow);

    INT begin, end, i, k;
    if ( this->values.size() == 0 ) {
        for ( i = 0; i < this->nrow; i++ ) {
            begin = this->rowPtr[i];
            end   = this->rowPtr[i+1];
            for ( k = begin; k < end; k++ ) w[i] += v[this->colInd[k]];
        }
    } else {
        for ( i = 0; i < this->nrow; i++ ) {
            begin = this->rowPtr[i];
            end   = this->rowPtr[i+1];
            for ( k = begin; k < end; k++ )
                w[i] += this->values[k] * v[this->colInd[k]];
        }
    }

    return w;
}

/// Return VEC = A'*vec1 + vec2
VEC MAT::MultTransposeAdd(const VEC& v1, const VEC& v2) const {

    const INT n=this->nrow,m=this->ncol,nnz=this->nnz;
    INT i,j,k,p;

    MAT tmp;
    tmp.nrow=m;
    tmp.ncol=n;
    tmp.nnz=nnz;

    tmp.rowPtr.resize(m+1);
    tmp.colInd.resize(nnz);
    if(this->values.size()){
        tmp.values.resize(nnz);
    }else{
        tmp.values.resize(0);
    }

    for(INT j=0;j<nnz;j++){
        i=this->colInd[j];
        if(i<m-1)
            tmp.rowPtr[i+2]++;
    }

    for(i=2;i<=m;i++)
        tmp.rowPtr[i]+=tmp.rowPtr[i-1];

    if(this->values.size()){
        for(i=0;i<n;i++){
            INT begin=this->rowPtr[i];
            INT end=this->rowPtr[i+1];
            for(p=begin;p<end;p++){
                j=this->colInd[p]+1;
                k=tmp.rowPtr[j];
                tmp.colInd[k]=i;
                tmp.values[k]=this->values[p];
                tmp.rowPtr[j]=k+1;
            }
        }
    }else{
        for(i=0;i<n;i++){
            INT begin=this->rowPtr[i];
            INT end=this->rowPtr[i+1];
            for(p=begin;p<end;p++){
                j=this->colInd[p]+1;
                k=tmp.rowPtr[j];
                tmp.colInd[k]=i;
                tmp.rowPtr[j]=k+1;
            }
        }
    }

    VEC v(v2);

    INT begin,end;
    for(i=0;i<tmp.nrow;i++){
        begin=tmp.rowPtr[i];
        end=tmp.rowPtr[i+1];
        for(j=begin;j<end;j++)
            v[i]+=v1[tmp.colInd[j]]*tmp.values[j];
    }

    return v;
}

/// *this = matl * matr
MAT Mult(const MAT& matl, const MAT& matr) {
    MAT mat;

    INT l, count;
    INT *tmp = new INT[matr.ncol];

    mat.nrow = matl.nrow;
    mat.ncol = matr.ncol;
    mat.rowPtr.resize(mat.nrow + 1);

    for ( INT i = 0; i < matr.ncol; i++ )
        tmp[i] = -1;

    for ( INT i = 0; i < mat.nrow; i++ ) {
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

    for ( INT i = 0; i < mat.nrow; i++ )
        mat.rowPtr[i + 1] += mat.rowPtr[i];

    INT count_tmp;

    mat.colInd.resize(mat.rowPtr[mat.nrow]);

    for ( INT i = 0; i < mat.nrow; i++ ) {
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

    mat.values.resize(mat.rowPtr[mat.nrow]);

    for ( INT i = 0; i < mat.nrow; i++ ) {
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

    mat.nnz = mat.rowPtr[mat.nrow] - mat.rowPtr[0];

    return mat;
}

/// Form diagonal pointer using colInd and rowPtr
void MAT::FormDiagPtr(std::vector<INT>& diagPtr)
{
    this->diagPtr.resize(nrow);
    INT begin, end;
    for ( INT j = 0; j < nrow; j++ ) {
        begin = this->rowPtr[j];
        end = this->rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colInd[k] == j ) {
                this->diagPtr[j] = k; // diagonal pointer
                break; // skip the rest of Row j
            }
        }
    }
}

/// Empty a matrix
void MAT::Empty()
{
    this->nrow = 0;
    this->ncol = 0;
    this->nnz = 0;
    this->rowPtr.resize(0);
    this->diagPtr.resize(0);
    this->colInd.resize(0);
    this->values.resize(0);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/