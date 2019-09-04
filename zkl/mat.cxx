/**
 * MAT source file
 */

#include "mat.hxx"
#include <cassert>
#include "fasp++.hxx"

/*----------------------------------------------------------------------------*/

//! assign row, column, nnz, values, rowshift, colindex, diag to this->mat
MAT::MAT(INT row, INT col, INT nnz, std::vector <DBL> values,
         std::vector <INT> rowshift, std::vector <INT> colindex,
         std::vector <INT> diag) {
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    assert(row == rowshift.size() - 1);
    assert(row >= 1 || col >= 1);
    assert(((row > col) ? col : row) == diag.size());
    assert(nnz == colindex.size());
    assert(nnz == values.size());
    /*-----------------  end  -----------------*/

    this->row = row;
    this->column = col;
    this->nnz = nnz;
    (this->values).operator=(values);
    (this->rowshift).operator=(rowshift);
    (this->colindex).operator=(colindex);
    (this->diag).operator=(diag);

}

//! assign row, column, nnz, values, rowshift, colindex to *this, and this
//! constructed function adapts to CSR format and CSRx format, and the most
//! important point is that it exams data format completely
MAT::MAT(INT row, INT column, INT nnz, std::vector<DBL> values,
        std::vector<INT> rowshift, std::vector<INT> colindex){
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    assert(row == rowshift.size() - 1);
    assert(row >= 1 || column >= 1);
    assert(((row > column) ? column : row) == diag.size());
    assert(nnz == colindex.size());
    assert(nnz == values.size());

    //! simple examinations
    //! check rowshift
    for(INT j=1;j<row+1;j++)
        assert(rowshift[j-1]<rowshift[j]);

    assert(rowshift[0]>=1 || rowshift[row]<=(this->row)*(this->column)+1 );
    //! check colindex
    INT max_tmp,min_tmp;
    max_tmp=colindex[0];
    min_tmp=colindex[0];
    for(int j=1;j<nnz;j++){
        if(max_tmp<colindex[j])
            max_tmp=colindex[j];
        if(min_tmp>colindex[j])
            min_tmp=colindex[j];
    }
    assert(min_tmp>=1 && max_tmp<=column);
    for(int j=0;j<nnz-2;j++)
        assert(colindex[j]<=colindex[j+1] || colindex[j+1]<=colindex[j+2]);

    /*-----------------  end  -----------------*/
    INT CSR=0, CSRx=0;
    //! judge the data format

    //! basic judgement
    for(INT j=0;j<row;j++){
        if(rowshift[j]==rowshift[j+1]){
            CSR=1;
            break;
        }
    }
    INT begin,end,count=0;
    if(CSR!=1){
        for(INT j=0;j<row;j++){
            begin=rowshift[j];
            end=rowshift[j+1];
            for(INT k=begin;k<end;k++){
                if(colindex[k-1]==(j+1)){
                    count++;
                    break;
                }
            }
        }
    }

    INT len=row>column?column:row;
    if(count!=len)
        CSR=1;
    if(CSR!=1)
        CSRx=1;

    if(CSR){
        this->row=row;
        this->column=column;
        //! change CSR format's nnz to CSRx format's
        this->nnz=nnz+len-count;
        //! change CSR format's rowshift, colindex, values to CSRx format's
        //! and generate this->diag
        (this->rowshift).operator=(rowshift);
        (this->colindex).operator=(colindex);
        (this->values).operator=(values);
        this->colindex.reserve(this->nnz);
        this->values.reserve(this->nnz);
        this->diag.reserve(len);
        std::vector<INT>::iterator iter_rowshift, iter_colindex;
        std::vector<DBL>::iterator iter_values;
        iter_rowshift=(this->rowshift).begin();
        iter_colindex=(this->colindex).begin();
        iter_values=(this->values).begin();
        this->rowshift[row]=nnz+1;

        for(int j=0;j<row;j++){
            begin=this->rowshift[j];
            end=this->rowshift[j+1];
            if(begin==end){
                this->rowshift[j]=this->rowshift[j]+1;
                this->colindex.insert(iter_colindex+this->rowshift[j],1,j+1);
                this->values.insert(iter_values+this->rowshift[j],1,0.0);
            }
            if(this->colindex[begin-1]>(j+1)){
                this->rowshift[j]=j+1;
                (this->colindex).insert(iter_colindex+begin-1,1,j+1);
                (this->values).insert(iter_values+begin-1,1,0.0);
            }

            for(int k=begin+1;k<end;k++){
                if(this->colindex[k-1]==(j+1)){
                    break;
                }


            }

        }

    }

    if(CSRx){
        this->row=row;
        this->column=column;
        this->nnz=nnz;
        (this->values).operator=(values);
        (this->rowshift).operator=(rowshift);
        (this->colindex).operator=(colindex);

        //! compute this->diag
        diag.reserve(len);
        count=0;
        for(INT j=0;j<row;j++){
            begin=rowshift[j];
            end=rowshift[j+1];
            for(INT k=begin;k<end;k++){
                if(colindex[k-1]==j+1){
                    diag[count]=k-1;
                    count++;
                }
            }
        }
    }
}
#if 0
//! assign col, nnz, values, rowshift, colindex, diag to this->mat
MAT::MAT(INT col, INT nnz, std::vector <DBL> values, std::vector <INT> rowshift,
         std::vector <INT> colindex, std::vector <INT> diag) {
    /*
     * some basic examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    row = rowshift.size() - 1;
    /*----------------  begin  ----------------*/
    assert(row != rowshift.size() - 1);
    assert(row < 1 || col < 1);
    assert(((row > col) ? col : row) != diag.size());
    assert(nnz != colindex.size());
    assert(nnz != values.size());
    /*-----------------  end  -----------------*/
    this->column = col;
    this->nnz = nnz;
    (this->rowshift).operator=(rowshift);
    (this->colindex).operator=(colindex);
    (this->diag).operator=(diag);
}

//! assign col, nnz, values, rowshift, colindex to this->mat
MAT::MAT(INT col, INT nnz, std::vector<DBL> values, std::vector<INT> rowshift,
        std::vector<INT> colindex){
    /*
 * some basic examinations about parameters
 * to judge whether they are CSRx' parameters
 */
    row = rowshift.size() - 1;
    /*----------------  begin  ----------------*/
    assert(row != rowshift.size() - 1);
    assert(row < 1 || col < 1);
    assert(nnz != colindex.size());
    assert(nnz != values.size());
    /*-----------------  end  -----------------*/
    column=col;
    this->nnz=nnz;
    (this->values).operator=(values);
    (this->rowshift).operator=(rowshift);
    (this->colindex).operator=(colindex);
}

//! assign diagonal matrix to this->mat
MAT::MAT(VEC vec) {
    DBL *array;
    vec.GetArray(row, &array);
    column = row;
    nnz = row;
    values.assign(array, array + row);
    rowshift.assign(iota_iterator(1), iota_iterator(row + 1));
    colindex.assign((iota_iterator(1), iota_iterator(row)));
    diag.assign(iota_iterator(1), iota_iterator(row));
}

//! overload equals operator
MAT MAT::&operator=(const MAT& mat){
    row=mat.row;
    column=mat.column;
    nnz=mat.nnz;
    values.operator=(mat.values);
    rowshift.operator=(mat.rowshift);
    colindex.operator=(mat.colindex);
    diag.operator=(mat.diag);
}

//! assign MAT object to this->mat
MAT::MAT(const MAT &mat) {
    this->operator=(mat);
}

//! get the row or column number of this->mat
FaspErrorType MAT::GetSizes(INT &row, INT &col) {
    row = this->row;
    col = this->col;
}

//! get (this->mat).nnz
FaspErrorType MAT::Getnnz(INT &nnz) {
    nnz = this->nnz;
}

//! get (this->mat)[i][j]
FaspErrorType MAT::GetElem(INT row, INT column, DBL &value) {
    if (row < 0 || row >= this->row)
        return 4; //! errortype 4 marks that the row index is not in 1 ~ this->row

    if (column < 0 || column >= this->column)
        return 5; //! errortype 5 marks that the column index is not in 1 ~ this->column

    int shift = this->rowshift[row - 1];
    if (colindex[shift] <= column) {
        for (int j = shift; j < this->column; j++) {
            if (column == this->colindex[shift]) {
                value = this->values[column];
                return 0;
            }
        }
    }
    value = 0.0;

    return 0;
}

//! get the whole jth-row elements in this->mat into VEC object
FaspErrorType MAT::GetRow(INT row, VEC &vec) {
    if(row<1 || row>=this->row)
        return 4; //! errortype 4 marks that the row index is not in 1 ~ this->row

    int len=rowshift[row]-rowshift[row-1];

    DBL *p=new double[len];
    INT k=0;
    for(int j=rowshift[row-1];j<rowshift[row];j++){
        p[k]=values[j];
        k++;
    }
    vec.AssignByArray(len,p);

    return 0;
}

//! get the whole jth-column elements in this->mat into VEC object
FaspErrorType MAT::GetCol(INT column, VEC &vec){
    int count=0;
    DBL *pcolumn=new DBL[row];
    int l=0;

    for(int j=0;j<row;j++){
        if(column>=colindex[rowshift[j]]){
            for(int k=rowshift[j];k<rowshift[j+1];j++){
                if(colindex[k]==column){
                    count++;
                    pcolumn[l]=values[colindex[k]];
                    break;
                }
            }
        }
    }

    DBL *array=new DBL[count];
    for(int j=0;j<count;j++)
        array[j]=pcolumn[j];

    delete[] pcolumn;
    vec.AssignByArray(count,array);
    delete[] array;
}

//! get the whole diagonal elements in this->mat into VEC object
FaspErrorType MAT::GetDiag(VEC &vec){
    INT len=row>column?column:row;
    DBL *parray=new DBL[len];
    for(int j=0;j<len;j++)
        parray[j]=values[diag[j]];

    vec.AssignByArray(len,parray);
}

//! zero all the elements
FaspErrorType MAT::Zero(){
    nnz=row>column?column:row;
    std::vector<DBL> values_tmp(nnz,0.0);
    std::vector<INT> rowshift_tmp(iota_iterator(1),iota_iterator(nnz+1));
    std::vector<INT> colindex_tmp(iota_iterator(1),iota_iterator(nnz));
    values.operator=(values_tmp);
    rowshift.operator=(rowshift_tmp);
    colindex.operator=(colindex_tmp);
    diag.operator=(colindex_tmp);
    return 0;
}

//! copy this->mat into mat
FaspErrorType MAT::Copy(MAT &mat){
    mat.operator=(*this);
    return 0;
}

//! this->mat = a * this->mat
FaspErrorType MAT::Scale(DBL a){
    for(j=0;j<nnz;j++){
        values[j]=a*values[j];
    }
    return 0;
}

//! this->mat = a * I + this->mat
FaspErrorType MAT::Shift(DBL a){
    int len=diag.size();
    for(int j=0;j<len;j++){
        values[diag[j]]=a+values[diag[j]];
    }
    return 0;
}

//! vec_b = this->mat * vec_x
FaspErrorType MAT::MultVec(VEC vec_x, VEC vec_b){
    int len;
    DBL *x_array=new double[column];
    DBL *b_array=new double[row];

    vec_x.GetArray(len,x_array);

    INT ibegin, iend;
    for(int j=0;j<row;j++){
        ibegin=rowshift[j];
        iend=rowshift[j+1];
        for(int k=ibegin;k<iend;k++){
            b_array[j]+=this->values[this->colindex[k]]*x_array[this->colindex[k]];
        }
    }

    vec_b.AssignByArray(row,b_array);

    return 0;
}

//! vec_z = vec_y + this->mat * vec_x
FaspErrorType MAT::MultAdd(VEC vec_x, VEC vec_y, VEC vec_z){
    int len;
    DBL *x_array=new double[column];
    DBL *b_array=new double[row];

    vec_x.GetArray(len,x_array);

    INT ibegin, iend;
    for(int j=0;j<row;j++){
        ibegin=rowshift[j];
        iend=rowshift[j+1];
        for(int k=ibegin;k<iend;k++){
            b_array[j]+=this->values[this->colindex[k]]*x_array[this->colindex[k]];
        }
    }

    vec_b.AssignByArray(row,b_array);

    vec_z.Add2(1.0, vec_y, 1.0, vec_b);

    return 0;
}



//! transpose this->mat
FaspErrorType MAT::Transpose(){
    INT tmp;
    tmp=row;
    row=column;
    column=tmp;

    INT i,j,k,p;
    INT count;

    INT len=row>column:column?row;

    INT *shift=new INT[column];

    std::vector<INT> rowshift_new(column+1);
    std::vector<INT> colindex_new(nnz);
    std::vector<DBL> values_new(nnz);
    std::vector<INT> diag_new(len);

    rowshift_new[0]=1;

    for(j=0;j<nnz;j++)
        rowshift_new[colindex[j]]++;

    for(j=1;j<=row;j++)
        rowshift_new[j]+=rowshift_new[j-1];

    count=0;
    for(i=0;i<row;i++){
        INT ibegin=rowshift[i],iend=rowshift[i+1];
        for(p=ibegin;p<iend;p++){
            j=colindex[p];
            k=rowshift_new[j];
            colindex[k]=i;
            values_new[k]=values[p];
            rowshift_new[j]=k;
            if(p==(i+1))
                diag_new[count]=k;
        }
    }

    return 0;
}

//! this->mat = a * this->mat + b * mat
FaspErrorType MAT::Add(MAT mat, DBL a, DBL b){
    int nnz_tmp
    mat.Getnnz(nnz_tmp);
    int nnz_sum=nnz+nnz_tmp;
    MAT mat_tmp;
    //! mat != NULL and this->mat != NULL
    mat_tmp.row=this.row;
    mat_tmp.column=this->column;
    mat_tmp.nnz=nnz_sum;
    mat_tmp.rowshift.assign(this->row+1,0);
    mat_tmp.colindex.assign(nnz_sum,-1);
    mat_tmp.values.reserve(nnz_sum);
    mat_tmp.diag.reserve(this->row>this->column?this->column:this->row);

    INT count=0, added, countrow;

    for (int i=0; i<this->row; ++i) {
        countrow = 0;
        for (int j=this->rowshift[i]; j<this->rowshift[i+1]; ++j) {
            mat_tmp->values[count] = a * this->values[j];
            mat_tmp->colindex[count] = this->colindex[j];
            mat_tmp.rowshift[i+1]++;
            count++;
            countrow++;
        } // end for js

        for (int k=mat.rowshift[i]; k<mat.rowshift[i+1]; ++k) {
            added = 0;

            for (int l=mat_tmp.rowshift[i]; l<mat_tmp.rowshift[i]+countrow+1; l++) {
                if (mat.colindex[k] == mat_tmp.colindex[l]) {
                    mat_tmp.values[l] = mat_tmp.values[l] + b * mat.values[k];
                    added = 1;
                    break;
                }
            } // end for l

            if (added == 0) {
                mat_tmp.values[count] = beta * mat.values[k];
                mat_tmp.colindex[count] = mat.colindex[k];
                mat_tmp.rowshift[i+1]++;
                count++;
            }

        } // end for k

        mat_tmp.rowshift[i+1] += mat_tmp.rowshift[i];

    }

    mat_tmp.nnz = count;

    //diag

    this->operator=(mat_tmp);

    return 0;
}
#endif