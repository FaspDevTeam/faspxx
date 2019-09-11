/**
 * a part of source code about mat.hxx
 */

#include "mat.hxx"
#include "fasp++.hxx"
#include <iostream>

/*---------------------------------------------------------------------------------*/


//! assign row, column, nnz, values, rowshift, colindex, diag to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
        values, const std::vector<INT> rowshift, const std::vector<INT>
                colindex, const std::vector<INT> diag) {

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if (row != rowshift.size() - 1)
        goto Return;

    if (row <= 0 || column <= 0)
        goto Return;

    if (((row > column) ? column : row) != diag.size())
        goto Return;


    if (nnz != colindex.size())
        goto Return;

    if (nnz != values.size())
        goto Return;

    if (nnz != rowshift[rowshift.size() - 1])
        goto Return;

    //! simple examinations
    for (INT j = 0; j < row; j++) {
        if (rowshift[j] >= rowshift[j + 1]) {
            goto Return;
        }
    }

    if (rowshift[0] < 0 || rowshift[row] > nnz)
        goto Return;

    for (INT j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if (begin == end)
            goto Return;

        if (end == begin + 1) {
            if (colindex[begin] != j)
                goto Return;
        }

        if (end > begin + 1) {
            for (INT k = begin; k < end - 1; k++) {
                if (colindex[k] >= colindex[k + 1])
                    goto Return;
            }
            if (0 > colindex[begin])
                goto Return;

            if (colindex[end - 1] >= column)
                goto Return;
        }
    }

    //! exam diag and colindex
    for (INT j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        for (INT k = begin; k < end; k++) {
            if (colindex[k] == j) {
                if (diag[count] != k)
                    goto Return;
                else
                    count++;
            }
        }
    }
    if (count != diag.size())
        goto Return;

    mark = 1;

    if (mark == 0) {
        Return:
        std::cout << "----------**********----------" << std::endl;
        std::cout << "     data type isn't CSRx     " << std::endl;
        std::cout << "----------**********----------" << std::endl;
        this->row=0;
        this->column=0;
        this->nnz=0;
        this->values.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->rowshift.resize(0);
        return;
    }

    this->row = row;
    this->column = column;
    this->nnz = nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->diag.operator=(diag);
}

//! assign row, column, nnz, values, rowshift, colindex to *this
MAT::MAT(INT row, INT column, INT nnz, std::vector<DBL> values,
         std::vector<INT> rowshift, std::vector<INT> colindex) {
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if (row != rowshift.size() - 1)
        goto Return;

    if (row <= 0 || column <= 0)
        goto Return;

    if (nnz != colindex.size())
        goto Return;

    if (nnz != values.size())
        goto Return;

    if (nnz != rowshift[rowshift.size() - 1])
        goto Return;

    //! simple examinations
    for (int j = 0; j < row; j++) {
        if (rowshift[j] > rowshift[j + 1])
            goto Return;
    }

    if (rowshift[0] < 0 || rowshift[row] > nnz)
        goto Return;

    INT begin, end;
    for (int j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if (begin == end)
            continue;

        if (end == begin + 1) {
            if (0 > colindex[begin] || colindex[begin] >= column)
                goto Return;
        }

        if (end > begin + 1) {
            for (int k = begin; k < end; k++) {
                if (0 > colindex[k] || colindex[k] >= column) {
                    goto Return;
                }
            }
        }
    }

    mark = 1;

    if (mark == 0) {
        Return:
        std::cout << "------------************------------" << std::endl;
        std::cout << "      data type isn't CSR(CSRx)     " << std::endl;
        std::cout << "------------************------------" << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->values.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->rowshift.resize(0);
        return;
    }

    MAT mat;

    INT index;
    DBL data;
    std::vector<INT> tmprowshift, tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.operator=(values);
    for (int j = 0; j < row; j++) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if (end <= begin + 1) {
            continue;
        }
        for (int k = begin + 1; k < end; k++) {
            index = tmpcolindex[k];
            data = tmpvalues[k];
            for (int j = k - 1; j >= begin; j--) {
                if (index < tmpcolindex[j]) {
                    tmpcolindex[j + 1] = tmpcolindex[j];
                    tmpvalues[j + 1] = tmpvalues[j];
                } else {
                    break;
                }
            }
            tmpcolindex[j + 1] = index;
            tmpvalues[j + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->column = column;
    this->nnz = 0;
    this->rowshift.resize(row + 1);
    this->rowshift[0] = 0;

    for (int j = 0; j < row; j++) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if (begin == end) {
            this->nnz++;
            this->rowshift[j + 1] = this->rowshift[j] + 1;
            continue;
        }
        if (begin == end - 1) {
            if (tmpcolindex[begin] == j) {
                this->nnz++;
                this->rowshift[j + 1] = this->rowshift[j] + 1;
            } else {
                this->nnz += 2;
                this->rowshift[j + 1] = this->rowshift[j] + 2;
            }
            continue;
        }
        for (int k = begin; k < end - 1; k++) {
            if (tmpcolindex[k] < tmpcolindex[k + 1]) {
                this->nnz++;
                if (tmpcolindex[k] == j) {
                    mark = 1;
                }
            }
        }
        if (tmpcolindex[end - 2] < tmpcolindex[end - 1]) {
            this->nnz++;
            if (tmpcolindex[end - 1] == j) {
                mark = 1;
            }
        }

        if (mark != 1) {
            this->nnz++;
            this->rowshift[j + 1] = this->rowshift[j] + end - begin + 1;
        } else {
            this->rowshift[j + 1] = this->rowshift[j] + end - begin;
            mark = 0;
        }
    }

    this->colindex.resize(this->nnz);
    this->values.resize(this->nnz);

    INT count = 0;
    for (int j = 0; j < row; j++) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if (begin == end) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if (begin == end - 1) {
            if (tmpcolindex[begin] == j) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if (tmpcolindex[begin] > j) {
                    this->colindex[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colindex[count] = tmpcolindex[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if (tmpcolindex[begin] < j) {
                    this->colindex[count] = tmpcolindex[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                    this->colindex[count] = j;
                    this->values[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if (begin == end - 2) {
            if (tmpcolindex[begin + 1] < j && tmpcolindex[begin] < tmpcolindex[begin + 1]) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = tmpcolindex[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if (tmpcolindex[begin] < j && tmpcolindex[begin] == tmpcolindex[begin + 1]) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if (tmpcolindex[begin] < j && tmpcolindex[begin + 1] == j) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if (tmpcolindex[begin] == j && tmpcolindex[begin + 1] == j) {
                this->colindex[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if (tmpcolindex[begin] == j && tmpcolindex[begin + 1] > j) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = tmpcolindex[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if (tmpcolindex[begin] > j && tmpcolindex[begin] == tmpcolindex[begin + 1]) {
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if (tmpcolindex[begin] > j && tmpcolindex[begin] < tmpcolindex[begin + 1]) {
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = tmpcolindex[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for (int k = begin; k < end - 1; k++) {
            if (tmpcolindex[k + 1] < j && tmpcolindex[k] < tmpcolindex[k + 1]) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if (tmpcolindex[k] < j && tmpcolindex[k] == tmpcolindex[k + 1]) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if (tmpcolindex[k] < j && tmpcolindex[k + 1] == j) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if (tmpcolindex[k] == j && tmpcolindex[k + 1] == j) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if (tmpcolindex[k] < j && tmpcolindex[k + 1] > j) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if (tmpcolindex[k] == j && tmpcolindex[k + 1] > j) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if (tmpcolindex[k] > j && tmpcolindex[k] == tmpcolindex[k + 1]) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if (tmpcolindex[k] > j && tmpcolindex[k + 1] > tmpcolindex[k]) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if (tmpcolindex[end - 2] < tmpcolindex[end - 1] && tmpcolindex[end - 1] < j) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if (tmpcolindex[end - 2] == tmpcolindex[end - 1] && tmpcolindex[end - 1] < j) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if (tmpcolindex[end - 2] < tmpcolindex[end - 1] && tmpcolindex[end - 1] == j) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if (tmpcolindex[end - 2] == tmpcolindex[end - 1] && tmpcolindex[end - 1] == j) {
            this->colindex[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if (tmpcolindex[end - 2] < j && tmpcolindex[end - 1] > j) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if (tmpcolindex[end - 2] > j && tmpcolindex[end - 1] > tmpcolindex[end - 2]) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if (tmpcolindex[end - 2] == j && tmpcolindex[end - 1] > j) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if (tmpcolindex[end - 2] > j && tmpcolindex[end - 2] == tmpcolindex[end - 1]) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diag.resize(row > column ? column : row);
    //! compute this->diag
    count = 0;
    for (INT j = 0; j < row; j++) {
        begin = this->rowshift[j];
        end = this->rowshift[j + 1];
        for (INT k = begin; k < end; k++) {
            if (this->colindex[k] == j) {
                this->diag[count] = k;
                count++;
            }
        }
    }

}

//! assign VEC object to form the diagonal matrix
MAT::MAT(VEC &vec_obj){
    INT size=vec_obj.GetSize();
    if(size==0){
        this->column=0;
        this->row=0;
        this->nnz=0;
        this->values.reserve(0);
        this->colindex.reserve(0);
        this->rowshift.reserve(0);
        this->diag.reserve(0);
    }else{
        INT *array=new INT[size];
        for(int j=0;j<size;j++)
            array[j]=j;
        this->row=size;
        this->column = size;
        this->nnz = size;
        this->rowshift.resize(size+1);
        this->rowshift.assign(array,array+size);
        this->rowshift[size]=size;
        this->colindex.resize(size);
        this->colindex.assign(array,array+size);
        this->values.resize(size);
        for(int j=0;j<size;j++)
            this->values[j]=vec_obj[j];
        this->diag.resize(size);
        this->diag.assign(array,array+size);

        delete[] array;
    }
}

//! assign vector object to form the diagonal matrix
MAT::MAT(std::vector<DBL> &vector_obj){
    INT size=vector_obj.size();
    if(size==0){
        this->row=0;
        this->column=0;
        this->nnz=0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);
    }else{
        this->row=size;
        this->column=size;
        this->nnz=size;
        INT *p=new INT[size];
        for(int j=0;j<size;j++)
            p[j]=j;
        this->rowshift.resize(size+1);
        this->rowshift.assign(p,p+size);
        this->rowshift[size]=size;
        this->colindex.resize(size);
        this->colindex.assign(p,p+size);
        this->diag.resize(size);
        this->diag.assign(p,p+size);
        this->values.resize(size);
        this->values.assign(vector_obj.begin(),vector_obj.begin()+size);

        delete[] p;
    }
}

//! assign MAT object to *this
MAT::MAT(const MAT& mat){
    this->row=mat.row;
    this->column=mat.column;
    this->nnz=mat.nnz;
    this->values.operator=(mat.values);
    this->diag.operator=(mat.diag);
    this->colindex.operator=(mat.colindex);
    this->rowshift.operator=(mat.rowshift);
}

//! overload = operator
MAT& MAT::operator=(const MAT &mat) {
    this->row=mat.row;
    this->column=mat.column;
    this->nnz=mat.nnz;
    this->diag.operator=(mat.diag);
    this->colindex.operator=(mat.colindex);
    this->rowshift.operator=(mat.rowshift);
    this->values.operator=(mat.values);
}

//! check whether the input data are CSRx
FaspErrorType MAT::CheckCSRx(INT row, INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex,
                             std::vector<INT> diag) {

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT count = 0;
    INT mark = 0;
    INT begin, end;
    if (row != rowshift.size() - 1) {
        mark = 2;
        goto Return;
    }

    if (row <= 0 || column <= 0) {
        mark = 3;
        goto Return;
    }

    if (((row > column) ? column : row) != diag.size()) {
        mark = 4;
        goto Return;
    }

    if (nnz != colindex.size()) {
        mark = 5;
        goto Return;
    }

    if (nnz != values.size()) {
        mark = 6;
        goto Return;
    }

    if (nnz != rowshift[rowshift.size() - 1]) {
        mark = 7;
        goto Return;
    }

    //! simple examinations
    for (INT j = 0; j < row; j++) {
        if (rowshift[j] >= rowshift[j + 1]) {
            mark = 8;
            goto Return;
        }
    }

    if (rowshift[0] < 0 || rowshift[row] > nnz) {
        mark = 9;
        goto Return;
    }

    for (INT j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if (begin == end) {
            mark = 10;
            goto Return;
        }

        if (end == begin + 1) {
            if (colindex[begin] != j) {
                mark = 11;
                goto Return;
            }
        }

        if (end > begin + 1) {
            for (INT k = begin; k < end - 1; k++) {
                if (colindex[k] >= colindex[k + 1]) {
                    mark = 12;
                    goto Return;
                }
            }
            if (0 > colindex[begin]) {
                mark = 13;
                goto Return;
            }

            if (colindex[end - 1] >= column) {
                mark = 13;
                goto Return;
            }
        }
    }

    //! exam diag and colindex
    for (INT j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        for (INT k = begin; k < end; k++) {
            if (colindex[k] == j) {
                if (diag[count] != k) {
                    mark = 14;
                    goto Return;
                } else
                    count++;
            }
        }
    }
    if (count != diag.size()) {
        mark = 14;
        goto Return;
    }

    Return:
    switch (mark) {
        case 2:
            std::cout << "the mismatch of 'row' and 'the dimension of "
                         "rowshift.size()-1'" << std::endl;
            return 1;
        case 3:
            std::cout << "the row or column index error" << std::endl;
            return 1;
        case 4:
            std::cout << "min(row, column) is not equal to the size of"
                         " 'diag'" << std::endl;
            return 1;
        case 5:
            std::cout << "the mismatch of 'nnz' and the dimension of "
                         "'colindex'" << std::endl;
            return 1;
        case 6:
            std::cout << "the mismatch of 'nnz' and the dimension of "
                         "'values'" << std::endl;
            return 1;
        case 7:
            std::cout << "the mismatch of 'nnz' and the end element of"
                         " 'rowshift'" << std::endl;
            return 1;
        case 8:
            std::cout << "the order error of rowshift 's elements" << std::endl;
            return 1;
        case 9:
            std::cout << "the range error of rowshift 's elements" << std::endl;
            return 1;
        case 10:
            std::cout << "No element exists in a row" << std::endl;
            return 1;
        case 11:
            std::cout << "No diagonal element exists in a row" << std::endl;
            return 1;
        case 12:
            std::cout << "the order error of colindex 's elements" << std::endl;
            return 1;
        case 13:
            std::cout << "the range error of colindex 's elements" << std::endl;
            return 1;
        case 14:
            std::cout << "Not all diagonal elements exist" << std::endl;
            return 1;
        default:
            return 0;

    }
}

//! check whether the input data are CSR
FaspErrorType MAT::CheckCSR(INT row, INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex){
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
    */
    /*----------------  begin  ----------------*/
    INT mark=0;
    //! basic examinations
    if(row!=rowshift.size()-1){
        mark=2;
        goto Return;
    }

    if(row<=0 || column <=0){
        mark=3;
        goto Return;
    }

    if(nnz!=colindex.size()){
        mark=4;
        goto Return;
    }

    if(nnz!=values.size()){
        mark=5;
        goto Return;
    }

    if(nnz!=rowshift[rowshift.size()-1]){
        mark=6;
        goto Return;
    }

    //! simple examinations
    for(int j=0;j<row;j++){
        if(rowshift[j]>rowshift[j+1]){
            mark=7;
            goto Return;
        }
    }

    if(rowshift[0]<0 || rowshift[row]>nnz){
        mark=8;
        goto Return;
    }

    INT begin,end;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            continue;

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column){
                mark=9;
                goto Return;
            }
        }

        if(end>begin+1){
            for(int k=begin;k<end;k++){
                if(0>colindex[k] || colindex[k]>=column){
                    mark=9;
                    goto Return;
                }
            }
        }
    }

    Return:
    switch(mark){
        case 2:
            std::cout<<"the mismatch of 'row' and 'the dimension of "
                       "rowshift.size()-1'"<<std::endl;
            return 1;
        case 3:
            std::cout<<"the row or column index error"<<std::endl;
            return 1;
        case 4:
            std::cout<<"the mismatch of 'nnz' and the dimension of "
                       "'colindex'"<<std::endl;
            return 1;
        case 5:
            std::cout<<"the mismatch of 'nnz' and the dimension of "
                       "'values'"<<std::endl;
            return 1;
        case 6:
            std::cout<<"the mismatch of 'nnz' and the end element of"
                       " 'rowshift'"<<std::endl;
            return 1;
        case 7:
            std::cout<<"the order error of rowshift 's elements"<<std::endl;
            return 1;
        case 8:
            std::cout<<"the range error of rowshift 's elements"<<std::endl;
            return 1;
        case 9:
            std::cout<<"the range error of colindex 's elements"<<std::endl;
            return 1;
        default:
            return 0;
    }
}


//! assign row, column, nnz, values, rowshift, colindex, diag to *this
void MAT::SetValues(const INT row, const INT column, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowshift,
                    const std::vector<INT> colindex, const std::vector<INT> diag) {

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if (row != rowshift.size() - 1)
        goto Return;

    if (row <= 0 || column <= 0)
        goto Return;

    if (((row > column) ? column : row) != diag.size())
        goto Return;


    if (nnz != colindex.size())
        goto Return;

    if (nnz != values.size())
        goto Return;

    if (nnz != rowshift[rowshift.size() - 1])
        goto Return;

    //! simple examinations
    for (INT j = 0; j < row; j++) {
        if (rowshift[j] >= rowshift[j + 1]) {
            goto Return;
        }
    }

    if (rowshift[0] < 0 || rowshift[row] > nnz)
        goto Return;

    for (INT j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if (begin == end)
            goto Return;

        if (end == begin + 1) {
            if (colindex[begin] != j)
                goto Return;
        }

        if (end > begin + 1) {
            for (INT k = begin; k < end - 1; k++) {
                if (colindex[k] >= colindex[k + 1])
                    goto Return;
            }
            if (0 > colindex[begin])
                goto Return;

            if (colindex[end - 1] >= column)
                goto Return;
        }
    }

    //! exam diag and colindex
    for (INT j = 0; j < row; j++) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        for (INT k = begin; k < end; k++) {
            if (colindex[k] == j) {
                if (diag[count] != k)
                    goto Return;
                else
                    count++;
            }
        }
    }
    if (count != diag.size())
        goto Return;

    mark = 1;

    if (mark == 0) {
        Return:
        std::cout << "----------**********----------" << std::endl;
        std::cout << "     data type isn't CSRx     " << std::endl;
        std::cout << "----------**********----------" << std::endl;
        this->row=0;
        this->column=0;
        this->nnz=0;
        this->values.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->rowshift.resize(0);
        return;
    }

    this->row = row;
    this->column = column;
    this->nnz = nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->diag.operator=(diag);
}

//! assign row, column, nnz, values, rowshift, colindex to *this
void MAT::SetValues(INT row, INT column, INT nnz, std::vector<DBL> values,
                    std::vector<INT> rowshift, std::vector<INT> colindex){
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark=0;
    //! basic examinations
    if(row!=rowshift.size()-1)
        goto Return;

    if(row<=0 || column <=0)
        goto Return;

    if(nnz!=colindex.size())
        goto Return;

    if(nnz!=values.size())
        goto Return;

    if(nnz!=rowshift[rowshift.size()-1])
        goto Return;

    //! simple examinations
    for(int j=0;j<row;j++){
        if(rowshift[j]>rowshift[j+1])
            goto Return;
    }

    if(rowshift[0]<0 || rowshift[row]>nnz)
        goto Return;

    INT begin,end;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            continue;

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                goto Return;
        }

        if(end>begin+1){
            for(int k=begin;k<end;k++){
                if(0>colindex[k] || colindex[k]>=column){
                    goto Return;
                }
            }
        }
    }

    mark=1;

    if (mark == 0) {
        Return:
        std::cout << "------------************------------" << std::endl;
        std::cout << "      data type isn't CSR(CSRx)     " << std::endl;
        std::cout << "------------************------------" << std::endl;
        this->row=0;
        this->column=0;
        this->nnz=0;
        this->values.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->rowshift.resize(0);
        return;
    }

    MAT mat;

    INT index;
    DBL data;
    std::vector<INT> tmprowshift,tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.operator=(values);
    for(int j=0;j<row;j++){
        begin=tmprowshift[j];
        end=tmprowshift[j+1];
        if(end<=begin+1){
            continue;
        }
        for(int k=begin+1;k<end;k++){
            index=tmpcolindex[k];
            data=tmpvalues[k];
            for(int j=k-1;j>=begin;j--){
                if(index<tmpcolindex[j]){
                    tmpcolindex[j+1]=tmpcolindex[j];
                    tmpvalues[j+1]=tmpvalues[j];
                }else{
                    break;
                }
            }
            tmpcolindex[j+1]=index;
            tmpvalues[j+1]=data;
        }
    }


    mark=0;
    this->row=row;
    this->column=column;
    this->nnz=0;
    this->rowshift.resize(row+1);
    this->rowshift[0]=0;

    for(int j=0;j<row;j++){
        begin=tmprowshift[j];
        end=tmprowshift[j+1];
        if(begin==end){
            this->nnz++;
            this->rowshift[j+1]=this->rowshift[j]+1;
            continue;
        }
        if(begin==end-1){
            if(tmpcolindex[begin]==j){
                this->nnz++;
                this->rowshift[j+1]=this->rowshift[j]+1;
            }else{
                this->nnz+=2;
                this->rowshift[j+1]=this->rowshift[j]+2;
            }
            continue;
        }
        for(int k=begin;k<end-1;k++){
            if(tmpcolindex[k]<tmpcolindex[k+1]){
                this->nnz++;
                if(tmpcolindex[k]==j){
                    mark=1;
                }
            }
        }
        if(tmpcolindex[end-2]<tmpcolindex[end-1]){
            this->nnz++;
            if(tmpcolindex[end-1]==j){
                mark=1;
            }
        }

        if(mark!=1){
            this->nnz++;
            this->rowshift[j+1]=this->rowshift[j]+end-begin+1;
        }else{
            this->rowshift[j+1]=this->rowshift[j]+end-begin;
            mark=0;
        }
    }

    this->colindex.resize(this->nnz);
    this->values.resize(this->nnz);

    INT count=0;
    for(int j=0;j<row;j++){
        begin=tmprowshift[j];
        end=tmprowshift[j+1];
        if(begin==end){
            this->colindex[count]=j;
            this->values[count]=0.0;
            count++;
            continue;
        }
        if(begin==end-1){
            if(tmpcolindex[begin]==j){
                this->colindex[count]=j;
                this->values[count]=tmpvalues[begin];
                count++;
            }else{
                if(tmpcolindex[begin]>j){
                    this->colindex[count]=j;
                    this->values[count]=0.0;
                    count++;
                    this->colindex[count]=tmpcolindex[begin];
                    this->values[count]=tmpvalues[begin];
                    count++;
                }
                if(tmpcolindex[begin]<j){
                    this->colindex[count]=tmpcolindex[begin];
                    this->values[count]=tmpvalues[begin];
                    count++;
                    this->colindex[count]=j;
                    this->values[count]=0.0;
                    count++;
                }
            }
            continue;
        }
        if(begin==end-2){
            if(tmpcolindex[begin+1]<j && tmpcolindex[begin]<tmpcolindex[begin+1]){
                this->colindex[count]=tmpcolindex[begin];
                this->values[count]=tmpvalues[begin];
                count++;
                this->colindex[count]=tmpcolindex[begin+1];
                this->values[count]=tmpvalues[begin+1];
                count++;
                this->colindex[count]=j;
                this->values[count]=0.0;
                count++;
            }
            if(tmpcolindex[begin]<j && tmpcolindex[begin]==tmpcolindex[begin+1]){
                this->colindex[count]=tmpcolindex[begin];
                this->values[count]+=(tmpvalues[begin]+tmpvalues[begin+1]);
                count++;
                this->colindex[count]=j;
                this->values[count]=0.0;
                count++;
            }
            if(tmpcolindex[begin]<j && tmpcolindex[begin+1]==j){
                this->colindex[count]=tmpcolindex[begin];
                this->values[count]=tmpvalues[begin];
                count++;
                this->colindex[count]=j;
                this->values[count]=tmpvalues[begin+1];
                count++;
            }
            if(tmpcolindex[begin]==j && tmpcolindex[begin+1]==j){
                this->colindex[count]=j;
                this->values[count]+=(tmpvalues[begin]+tmpvalues[begin+1]);
                count++;
            }
            if(tmpcolindex[begin]==j && tmpcolindex[begin+1]>j){
                this->colindex[count]=j;
                this->values[count]=tmpvalues[begin];
                count++;
                this->colindex[count]=tmpcolindex[begin+1];
                this->values[count]=tmpvalues[begin+1];
                count++;
            }
            if(tmpcolindex[begin]>j && tmpcolindex[begin]==tmpcolindex[begin+1]){
                this->colindex[count]=j;
                this->values[count]=0.0;
                count++;
                this->colindex[count]=tmpcolindex[begin];
                this->values[count]+=(tmpvalues[begin]+tmpvalues[begin+1]);
                count++;
            }
            if(tmpcolindex[begin]>j && tmpcolindex[begin]<tmpcolindex[begin+1]){
                this->colindex[count]=j;
                this->values[count]=0.0;
                count++;
                this->colindex[count]=tmpcolindex[begin];
                this->values[count]=tmpvalues[begin];
                count++;
                this->colindex[count]=tmpcolindex[begin+1];
                this->values[count]=tmpvalues[begin+1];
                count++;
            }
            continue;
        }
        for(int k=begin;k<end-1;k++){
            if(tmpcolindex[k+1]<j && tmpcolindex[k]<tmpcolindex[k+1]){
                this->colindex[count]=tmpcolindex[k];
                this->values[count]+=tmpvalues[k];
                count++;
            }
            if(tmpcolindex[k]<j && tmpcolindex[k]==tmpcolindex[k+1]){
                this->colindex[count]=tmpcolindex[k];
                this->values[count]+=tmpvalues[k];
            }
            if(tmpcolindex[k]<j && tmpcolindex[k+1]==j){
                this->colindex[count]=tmpcolindex[k];
                this->values[count]+=tmpvalues[k];
                count++;
            }
            if(tmpcolindex[k]==j && tmpcolindex[k+1]==j){
                this->colindex[count]=j;
                this->values[count]+=tmpvalues[k];
            }
            if(tmpcolindex[k]<j && tmpcolindex[k+1]>j){
                this->colindex[count]=tmpcolindex[k];
                this->values[count]+=tmpvalues[k];
                count++;
                this->colindex[count]=j;
                this->values[count]=0.0;
                count++;
            }
            if(tmpcolindex[k]==j && tmpcolindex[k+1]>j){
                this->colindex[count]=j;
                this->values[count]+=tmpvalues[k];
                count++;
            }
            if(tmpcolindex[k]>j && tmpcolindex[k]==tmpcolindex[k+1]){
                this->colindex[count]=tmpcolindex[k];
                this->values[count]+=tmpvalues[k];
            }
            if(tmpcolindex[k]>j && tmpcolindex[k+1]>tmpcolindex[k]){
                this->colindex[count]=tmpcolindex[k];
                this->values[count]+=tmpvalues[k];
                count++;
            }
        }
        if(tmpcolindex[end-2]<tmpcolindex[end-1] && tmpcolindex[end-1]<j){
            this->colindex[count]=tmpcolindex[end-1];
            this->values[count]+=tmpvalues[end-1];
            count++;
            this->colindex[count]=j;
            this->values[count]=0.0;
            count++;
        }
        if(tmpcolindex[end-2]==tmpcolindex[end-1] && tmpcolindex[end-1]<j){
            this->colindex[count]=tmpcolindex[end-1];
            this->values[count]+=tmpvalues[end-1];
            count++;
            this->colindex[count]=j;
            this->values[count]=0.0;
            count++;
        }
        if(tmpcolindex[end-2]<tmpcolindex[end-1] && tmpcolindex[end-1]==j){
            this->colindex[count]=j;
            this->values[count]=0.0;
            count++;
        }
        if(tmpcolindex[end-2]==tmpcolindex[end-1] && tmpcolindex[end-1]==j){
            this->colindex[count]=j;
            this->values[count]+=tmpvalues[end-1];
            count++;
        }
        if(tmpcolindex[end-2]<j && tmpcolindex[end-1]>j){
            this->colindex[count]=tmpcolindex[end-1];
            this->values[count]+=tmpvalues[end-1];
            count++;
        }
        if(tmpcolindex[end-2]>j && tmpcolindex[end-1]>tmpcolindex[end-2]){
            this->colindex[count]=tmpcolindex[end-1];
            this->values[count]+=tmpvalues[end-1];
            count++;
        }
        if(tmpcolindex[end-2]==j && tmpcolindex[end-1]>j){
            this->colindex[count]=tmpcolindex[end-1];
            this->values[count]=tmpvalues[end-1];
            count++;
        }
        if(tmpcolindex[end-2]>j && tmpcolindex[end-2]==tmpcolindex[end-1]){
            this->colindex[count]=tmpcolindex[end-1];
            this->values[count]+=tmpvalues[end-1];
            count++;
        }
    }

    this->diag.resize(row>column?column:row);
    //! compute this->diag
    count=0;
    for(INT j=0;j<row;j++){
        begin=this->rowshift[j];
        end=this->rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(this->colindex[k]==j){
                this->diag[count]=k;
                count++;
            }
        }
    }
}

//! get the row or column number of this->mat
void MAT::GetSizes(INT &row, INT &column){
    row=this->row;
    column=this->column;
}

//! get (this->mat).nnz
INT MAT::Getnnz(){
    return this->nnz;
}

//! get (this->mat)[i][j]
FaspErrorType MAT::GetElem(INT row, INT column, DBL &value){
    if (row < 0 || row >= this->row)
        return 2; //! 2 marks the row range error

    if (column < 0 || column >= this->column)
        return 3; //! 3 marks the column range error

    if (this->colindex[this->rowshift[row-1]] <= column) {
        for (int j = this->colindex[this->rowshift[row-1]];
             j < this->colindex[this->rowshift[row]]; j++) {
            if (column == this->colindex[j]) {
                value = this->values[j];
                return 0;
            }
        }
    }
    value = 0.0;

    return 0;
}

//! get the whole jth-row elements in *this into vector object
FaspErrorType MAT::GetRow(INT row, std::vector<DBL> &vector_obj){
    if(row<0 || row>=this->row)
        return 2; //! 2 marks the row range error

    int len=this->rowshift[row]-this->rowshift[row-1];
    vector_obj.resize(len);
    INT k=0;
    for(int j=this->rowshift[row-1];j<this->rowshift[row];j++){
        vector_obj[k]=this->values[j];
        k++;
    }

    return 0;
}

//! get the whole jth-column elements in *this into vector object
FaspErrorType MAT::GetColumn(INT column, std::vector<DBL> &vector_obj){
    INT count=0;
    if(column<0 || column>=this->column)
        return 3; //! 3 marks the column range error

    vector_obj.resize(row);
    for(int j=0;j<row;j++){
        if(column>=colindex[rowshift[j]]){
            for(int k=rowshift[j];k<rowshift[j+1];j++){
                if(colindex[k]==column){
                    vector_obj[count]=values[k];
                    count++;
                    break;
                }
            }
        }
    }
    vector_obj.resize(count);

    return 0;
}

//! get the whole diagonal elements in *this into VEC object
void MAT::GetDiag(std::vector<DBL> &vector_obj){
    INT len=this->row>this->column?this->column:this->row;
    vector_obj.resize(len);
    for(int j=0;j<len;j++)
        vector_obj[j]=this->values[this->diag[j]];

}

//! zero all the elements
void MAT::Zero(){
    this->nnz=this->diag.size();
    INT *pcol=new INT[this->diag.size()];
    INT *prow=new INT[this->row+1];

    for(int j=0;j<this->diag.size();j++)
        pcol[j]=j;

    for(int j=0;j<this->row+1;j++)
        prow[j]=j;

    std::vector<INT> rowshift_tmp(this->row+1);
    std::vector<INT> colindex_tmp(this->nnz);
    std::vector<DBL> values_tmp(this->nnz);

    rowshift_tmp.assign(prow,prow+this->row);
    colindex_tmp.assign(pcol,pcol+this->nnz);
    values_tmp.assign(this->nnz,0);

    this->rowshift.operator=(rowshift_tmp);
    this->colindex.operator=(colindex_tmp);
    this->values.operator=(values_tmp);

}

//! copy this->mat into mat
void MAT::Copy(MAT &mat){
    mat.operator=(*this);
}

//! this->mat = a * this->mat
void MAT::Scale(DBL a){
    for(INT j=0;j<nnz;j++){
        values[j]=a*values[j];
    }
}

//! this->mat = a * I + this->mat
void MAT::Shift(DBL a){
    for(int j=0;j<this->diag.size();j++){
        this->values[this->diag[j]]=a+this->values[this->diag[j]];
    }
}

//! vec_b = this->mat * vec_x
FaspErrorType MAT::MultVec(VEC vec_x, VEC &vec_b){
    INT begin,end;
    if(this->column!=vec_x.GetSize())
        return 4; //! 4 marks the mismatch of VEC object and MAT object 's dimensions

    for(int j=0;j<this->row;j++){
        begin=this->rowshift[j];
        end=this->rowshift[j+1];
        for(int k=begin;k<end;k++){
            vec_b[j]+=this->values[k]*vec_x[this->colindex[k]];
        }
    }

    return 0;
}

//! transpose this->mat
void MAT::Transpose(){

    MAT tmp;
    tmp.row=column;
    tmp.column=row;
    tmp.nnz=nnz;

    tmp.rowshift.reserve(tmp.row+1);
    tmp.colindex.reserve(tmp.nnz);
    tmp.values.reserve(tmp.nnz);
    tmp.diag.reserve(tmp.row>tmp.column?tmp.column:tmp.row);

    INT count,begin,end;

    tmp.rowshift[0]=0;

    for(int j=0;j<this->nnz;j++){
        count=this->rowshift[j];
        if(count<this->column-1){
            tmp.rowshift[count+2]++;
        }
    }

    for(int j=2;j<=this->column;j++)
        tmp.rowshift[j]+=tmp.rowshift[j-1];

    int l,k;
    if(this->values.size()){
        for(int j=0;j<this->row;j++){
            begin=this->rowshift[j];
            end=this->rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=this->colindex[p]+1;
                k=tmp.rowshift[l];
                tmp.colindex[k]=j;
                tmp.values[k]=this->values[p];
                tmp.rowshift[l]=k+1;
            }
        }
    }else{
        for(int j=0;j<this->row;j++){
            begin=this->rowshift[j];
            end=this->rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=this->colindex[p]+1;
                k=tmp.rowshift[l];
                tmp.colindex[k]=j;
                tmp.rowshift[l]=k+1;
            }
        }
    }
    //! compute this->diag
    count=0;
    tmp.diag.reserve(tmp.row>tmp.column?tmp.column:tmp.row);
    for(INT j=0;j<tmp.row;j++){
        begin=tmp.rowshift[j];
        end=tmp.rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(tmp.colindex[k]==j){
                tmp.diag[count]=k;
                count++;
            }
        }
    }
    (*this).operator=(tmp);

}

//! vec3 = vec2 + transpose(*this) * vec1
FaspErrorType MAT::MultTransposeAdd(VEC vec1,VEC vec2,VEC vec3){
    if(this->row!=vec1.GetSize())
        return 4; //! 4 marks the mismatch of VEC object and MAT object 's dimensions

    if(this->column!=vec2.GetSize())
        return 4; //! 4 marks the mismatch of VEC object and MAT object 's dimensions

    vec3.operator=(vec2);

    MAT tmp;
    tmp.row=this->column;
    tmp.column=this->row;
    tmp.nnz=this->nnz;

    tmp.rowshift.reserve(tmp.row+1);
    tmp.colindex.reserve(tmp.nnz);
    tmp.values.reserve(tmp.nnz);
    tmp.diag.reserve(tmp.row>tmp.column?tmp.column:tmp.row);

    INT count,begin,end;

    tmp.rowshift[0]=0;

    for(int j=0;j<this->nnz;j++){
        count=this->rowshift[j];
        if(count<this->column-1){
            tmp.rowshift[count+2]++;
        }
    }

    for(int j=2;j<=this->column;j++)
        tmp.rowshift[j]+=tmp.rowshift[j-1];

    int l,k;
    if(this->values.size()){
        for(int j=0;j<this->row;j++){
            begin=this->rowshift[j];
            end=this->rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=this->colindex[p]+1;
                k=tmp.rowshift[l];
                tmp.colindex[k]=j;
                tmp.values[k]=this->values[p];
                tmp.rowshift[l]=k+1;
            }
        }
    }else{
        for(int j=0;j<this->row;j++){
            begin=this->rowshift[j];
            end=this->rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=this->colindex[p]+1;
                k=tmp.rowshift[l];
                tmp.colindex[k]=j;
                tmp.rowshift[l]=k+1;
            }
        }
    }

    if(this->values.size()){
        for(int j=0;j<tmp.row;j++){
            begin=tmp.rowshift[j];
            end=tmp.rowshift[j+1];
            for(int k=begin;k<end;k++){
                vec3[j]+=vec1[this->colindex[k]]*tmp.values[k];
            }
        }
    }else{
        for(int j=0;j<tmp.row;j++){
            begin=tmp.rowshift[j];
            end=tmp.rowshift[j+1];
            for(int k=begin;k<end;k++){
                vec3[j]+=vec1[this->colindex[k]];
            }
        }
    }
}

//! *this = a * *this + b * mat
FaspErrorType MAT::Add(DBL a, DBL b,MAT mat){
    if(this->row!=mat.row || this->column!=mat.column)
        return 5; //! 5 marks the mismatch of matrices' rows and columns

    if(mat.row==0 || mat.column==0)
        return 0;

    int nnz_sum=this->nnz+mat.Getnnz();

    MAT mat_tmp;
    mat_tmp.row=this->row;
    mat_tmp.column=this->column;
    mat_tmp.nnz=nnz_sum;
    mat_tmp.rowshift.assign(this->row+1,0);
    mat_tmp.colindex.assign(nnz_sum,-1);
    mat_tmp.values.reserve(nnz_sum);
    mat_tmp.diag.reserve(this->row>this->column?this->column:this->row);

    INT count=0, added, countrow;

    for (int i=0; i<this->row; i++) {
        countrow = 0;
        for (int j=this->rowshift[i]; j<this->rowshift[i+1]; j++) {
            mat_tmp.values[count] = a * this->values[j];
            mat_tmp.colindex[count] = this->colindex[j];
            mat_tmp.rowshift[i+1]++;
            count++;
            countrow++;
        }

        for (int k=mat.rowshift[i]; k<mat.rowshift[i+1]; k++) {
            added = 0;

            for (int l=mat_tmp.rowshift[i]; l<mat_tmp.rowshift[i]+countrow+1; l++) {
                if (mat.colindex[k] == mat_tmp.colindex[l]) {
                    mat_tmp.values[l] = mat_tmp.values[l] + b * mat.values[k];
                    added = 1;
                    break;
                }
            }

            if (added == 0) {
                mat_tmp.values[count] = b * mat.values[k];
                mat_tmp.colindex[count] = mat.colindex[k];
                mat_tmp.rowshift[i+1]++;
                count++;
            }

        }

        mat_tmp.rowshift[i+1] += mat_tmp.rowshift[i];

    }

    mat_tmp.nnz = count;

    INT begin,end;
    count=0;
    for(INT j=0;j<this->row;j++){
        begin=mat_tmp.rowshift[j];
        end=mat_tmp.rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(mat_tmp.colindex[k-1]==j+1){
                mat_tmp.diag[count]=k-1;
                count++;
            }
        }
    }
    this->operator=(mat_tmp);
    return 0;
}

//! *this = *this * mat
FaspErrorType MAT::MultLeft(MAT mat){
    if(this->column!=mat.row)
        return 5; //! 5 marks the mismatch of rows or columns between matrices

    INT count,l;
    MAT tmp_mat;
    INT *tmp = new INT[mat.column];

    tmp_mat.row=this->row;

    tmp_mat.column=mat.column;
    tmp_mat.rowshift.reserve(tmp_mat.row+1);

    for (int i=0;i<mat.column;i++)
        tmp[i]=-1;

    for (int i=0;i<tmp_mat.row;i++) {
        count=0;
        for (int k=this->rowshift[i];k<this->rowshift[i+1];k++) {
            for (int j=mat.rowshift[this->colindex[k]];
            j<mat.rowshift[this->colindex[k]+1];j++) {
                for (l=0;l<count;l++) {
                    if (tmp[l]==mat.colindex[j])
                        break;
                }

                if (l==count) {
                    tmp[count]=mat.colindex[j];
                    count++;
                }
            }
        }
        tmp_mat.rowshift[i+1]=count;
        for (int j=0;j<count;j++) {
            tmp[j]=-1;
        }
    }

    for (int i=0;i<tmp_mat.row;i++)
        tmp_mat.rowshift[i+1]+=tmp_mat.rowshift[i];

    INT count_tmp;

    tmp_mat.colindex.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;++i) {
        count_tmp=0;
        count=tmp_mat.rowshift[i];
        for (int k=this->rowshift[i];k<this->rowshift[i+1];++k) {
            for (int j=mat.rowshift[this->colindex[k]];
            j<mat.rowshift[this->colindex[k]+1];++j) {
                for (l=0;l<count_tmp;l++){
                    if (tmp[l]==mat.colindex[j])
                        break;
                }

                if (l==count_tmp) {
                    tmp_mat.colindex[count]=mat.colindex[j];
                    tmp[count_tmp]=mat.colindex[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for (int j=0;j<count_tmp;j++)
            tmp[j]=-1;
    }

    delete[] tmp;

    tmp_mat.values.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;i++) {
        for (int j=tmp_mat.rowshift[i];j<tmp_mat.rowshift[i+1];j++) {
            tmp_mat.values[j]=0;
            for (int k=this->rowshift[i];k<this->rowshift[i+1];k++) {
                for (l=mat.rowshift[this->colindex[k]];
                l<mat.rowshift[this->colindex[k]+1];l++) {
                    if (mat.colindex[l]==tmp_mat.colindex[j])
                        tmp_mat.values[j]+=this->values[k]*mat.values[l];
                }
            }
        }
    }

    tmp_mat.nnz = tmp_mat.rowshift[tmp_mat.row]-tmp_mat.rowshift[0];
    this->operator=(tmp_mat);
    return 0;
}

//! *this = mat * *this
FaspErrorType MAT::MultRight(MAT mat){
    if(mat.column!=this->row)
        return 5; //! 5 marks the mismatch of rows or columns between matrices

    INT l,count;
    MAT tmp_mat;
    INT *tmp = new INT[this->column];

    tmp_mat.row=mat.row;
    tmp_mat.column=this->column;
    tmp_mat.rowshift.reserve(tmp_mat.row+1);

    for (int i=0;i<this->column;i++)
        tmp[i]=-1;

    for (int i=0;i<tmp_mat.row;i++) {
        count=0;
        for (int k=mat.rowshift[i];k<mat.rowshift[i+1];k++) {
            for (int j=this->rowshift[mat.colindex[k]];
            j<this->rowshift[mat.colindex[k]+1];j++) {
                for (l=0;l<count;l++) {
                    if (tmp[l]==this->colindex[j])
                        break;
                }
                if (l==count) {
                    tmp[count]=this->colindex[j];
                    count++;
                }
            }
        }
        tmp_mat.rowshift[i+1]=count;
        for (int j=0;j<count;j++)
            tmp[j]=-1;
    }

    for (int i=0;i<tmp_mat.row;i++)
        tmp_mat.rowshift[i+1]+=tmp_mat.rowshift[i];

    INT count_tmp;

    tmp_mat.colindex.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;i++) {
        count_tmp=0;
        count=tmp_mat.rowshift[i];
        for (int k=mat.rowshift[i];k<mat.rowshift[i+1];k++) {
            for (int j=this->rowshift[mat.colindex[k]];
            j<this->rowshift[mat.colindex[k]+1];j++) {
                for (l=0;l<count_tmp;l++) {
                    if (tmp[l]==this->colindex[j])
                        break;
                }

                if (l==count_tmp) {
                    tmp_mat.colindex[count]=this->colindex[j];
                    tmp[count_tmp]=this->colindex[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for (int j=0;j<count_tmp;++j)
            tmp[j]=-1;
    }

    delete[] tmp;

    tmp_mat.values.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;++i) {
        for (int j=tmp_mat.rowshift[i];j<tmp_mat.rowshift[i+1];j++) {
            tmp_mat.values[j]=0;
            for (int k=mat.rowshift[i];k<mat.rowshift[i+1];k++) {
                for (l=this->rowshift[mat.colindex[k]];
                l<this->rowshift[mat.colindex[k]+1];l++) {
                    if (this->colindex[l]==tmp_mat.colindex[j])
                        tmp_mat.values[j]+=mat.values[k]*this->values[l];
                }
            }
        }
    }

    tmp_mat.nnz = tmp_mat.rowshift[tmp_mat.row]-tmp_mat.rowshift[0];

    this->operator=(tmp_mat);
    return 0;
}

//! mat3 = a * mat1 + b * mat2
FaspErrorType Add(DBL a,MAT mat1,DBL b,MAT mat2,MAT mat3){
    if(mat1.row!=mat2.row || mat1.column!=mat2.column)
        return 5; //! 5 marks the mismatch of matrices' rows and columns

    if(mat1.row==0 || mat1.column==0){
        mat3.row=0;
        mat3.column=0;
        mat3.nnz=0;
        mat3.values.resize(0);
        mat3.rowshift.resize(0);
        mat3.colindex.resize(0);
        mat3.diag.resize(0);
        return 0;
    }

    int nnz_sum=mat1.nnz+mat1.Getnnz();

    mat3.row=mat1.row;
    mat3.column=mat1.column;
    mat3.nnz=nnz_sum;
    mat3.rowshift.assign(mat1.row+1,0);
    mat3.colindex.assign(nnz_sum,-1);
    mat3.values.reserve(nnz_sum);
    mat3.diag.reserve(mat1.row>mat1.column?mat1.column:mat1.row);

    INT count=0, added, countrow;

    for (int i=0; i<mat1.row; i++) {
        countrow = 0;
        for (int j=mat1.rowshift[i]; j<mat1.rowshift[i+1]; j++) {
            mat3.values[count] = a * mat1.values[j];
            mat3.colindex[count] = mat1.colindex[j];
            mat3.rowshift[i+1]++;
            count++;
            countrow++;
        }

        for (int k=mat2.rowshift[i]; k<mat2.rowshift[i+1]; k++) {
            added = 0;

            for (int l=mat3.rowshift[i]; l<mat3.rowshift[i]+countrow+1; l++) {
                if (mat2.colindex[k] == mat3.colindex[l]) {
                    mat3.values[l] = mat3.values[l] + b * mat2.values[k];
                    added = 1;
                    break;
                }
            }

            if (added == 0) {
                mat3.values[count] = b * mat2.values[k];
                mat3.colindex[count] = mat2.colindex[k];
                mat3.rowshift[i+1]++;
                count++;
            }

        }

        mat3.rowshift[i+1] += mat3.rowshift[i];

    }

    mat3.nnz = count;

    INT begin,end;
    count=0;
    for(INT j=0;j<mat1.row;j++){
        begin=mat3.rowshift[j];
        end=mat3.rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(mat3.colindex[k-1]==j+1){
                mat3.diag[count]=k-1;
                count++;
            }
        }
    }
    return 0;
}

//! mat3 = mat1 * mat2
FaspErrorType Mult2(MAT mat1, MAT mat2, MAT mat3) {
    if (mat1.column != mat2.row)
        return 5; //! 5 marks the mismatch of rows or columns between matrices

    INT l, count;
    INT *tmp = new INT[mat2.column];

    mat3.row = mat1.row;
    mat3.column = mat2.column;
    mat3.rowshift.reserve(mat3.row + 1);

    for (int i = 0; i < mat2.column; i++)
        tmp[i] = -1;

    for (int i = 0; i < mat3.row; i++) {
        count = 0;
        for (int k = mat1.rowshift[i]; k < mat1.rowshift[i + 1]; k++) {
            for (int j = mat2.rowshift[mat1.colindex[k]];
                 j < mat2.rowshift[mat1.colindex[k] + 1]; j++) {
                for (l = 0; l < count; l++) {
                    if (tmp[l] == mat2.colindex[j])
                        break;
                }
                if (l == count) {
                    tmp[count] = mat2.colindex[j];
                    count++;
                }
            }
        }
        mat3.rowshift[i + 1] = count;
        for (int j = 0; j < count; j++)
            tmp[j] = -1;
    }

    for (int i = 0; i < mat3.row; i++)
        mat3.rowshift[i + 1] += mat3.rowshift[i];

    INT count_tmp;

    mat3.colindex.reserve(mat3.rowshift[mat3.row]);

    for (int i = 0; i < mat3.row; i++) {
        count_tmp = 0;
        count = mat3.rowshift[i];
        for (int k = mat1.rowshift[i]; k < mat1.rowshift[i + 1]; k++) {
            for (int j = mat2.rowshift[mat1.colindex[k]];
                 j < mat2.rowshift[mat1.colindex[k] + 1]; j++) {
                for (l = 0; l < count_tmp; l++) {
                    if (tmp[l] == mat2.colindex[j])
                        break;
                }

                if (l == count_tmp) {
                    mat3.colindex[count] = mat2.colindex[j];
                    tmp[count_tmp] = mat2.colindex[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for (int j = 0; j < count_tmp; ++j)
            tmp[j] = -1;
    }

    delete[] tmp;

    mat3.values.reserve(mat3.rowshift[mat3.row]);

    for (int i = 0; i < mat3.row; ++i) {
        for (int j = mat3.rowshift[i]; j < mat3.rowshift[i + 1]; j++) {
            mat3.values[j] = 0;
            for (int k = mat1.rowshift[i]; k < mat1.rowshift[i + 1]; k++) {
                for (l = mat2.rowshift[mat1.colindex[k]];
                     l < mat2.rowshift[mat1.colindex[k] + 1]; l++) {
                    if (mat2.colindex[l] == mat3.colindex[j])
                        mat3.values[j] += mat1.values[k] * mat2.values[l];
                }
            }
        }
    }

    mat3.nnz = mat3.rowshift[mat3.row] - mat3.rowshift[0];

    return 0;
}

//! mat4 = mat1 * mat2 * mat3
FaspErrorType Mult3(MAT mat1,MAT mat2,MAT mat3,MAT mat4) {

    mat4.row = matl.row;
    mat4.column = matr.column;

    INT tmpi, tmpj, tmpk;
    INT count, begin;
    DBL tmp, tmp_pro, final_pro;

    INT total = matl.row + mat2.row + matl.row + 1 + 1;

    INT *ptotal = new INT[total];
    INT *phalf = ptotal + matl.row;

    /*------------------------------------------------------*
     *  First Pass: Determine size of RAP and set up tmp.rowshift  *
     *------------------------------------------------------*/
    mat4.rowshift.reserve(matl.row + 1);

    for (int j = 0; j < matl.row + mat2.row; j++) {
        ptotal[j] = -1;
    }

    count = 0;
    for (int j = 0; j < matl.row; j++) {
        ptotal[j] = count;
        begin = count;
        count++;

        for (int k = matl.rowshift[j]; k < matl.rowshift[j + 1]; k++) {
            tmpi = matl.colindex[k];

            for (int l = mat2.rowshift[tmpi]; l < mat2.rowshift[tmpi + 1]; l++) {
                tmpj = mat2.colindex[l];
                if (phalf[tmpj] != j) {
                    phalf[tmpj] = j;
                    for (int p = matr.rowshift[tmpj]; p < matr.rowshift[tmpj + 1]; p++) {
                        tmpk = matr.colindex[p];
                        if (ptotal[tmpk] < begin) {
                            ptotal[tmpk] = count;
                            count++;
                        }
                    }
                }
            }
        }

        mat4.rowshift[j] = begin;
    }

    mat4.rowshift[matl.row] = count;
    mat4.nnz = count;

    mat4.colindex.reserve(mat4.nnz);
    mat4.values.reserve(mat4.nnz);

    for (int j = 0; j < matl.row + mat2.row; j++) {
        ptotal[j] = -1;
    }

    count = 0;
    for (int j = 0; j < matl.row; j++) {
        ptotal[j] = count;
        begin = count;
        mat4.colindex[count] = j;
        mat4.values[count] = 0.0;
        count++;

        for (int k = matl.rowshift[j]; k < matl.rowshift[j + 1]; k++) {
            tmp = matl.values[k];

            tmpi = matl.colindex[k];
            for (int l = mat2.rowshift[tmpi]; l < mat2.rowshift[tmpi + 1]; l++) {
                tmp_pro = tmp * mat2.values[l];

                tmpj = mat2.colindex[l];
                if (phalf[tmpj] != j) {
                    phalf[tmpj] = j;
                    for (int p = matr.rowshift[tmpj]; p < matr.rowshift[tmpj + 1]; p++) {
                        final_pro = tmp_pro * matr.values[p];

                        tmpk = matr.colindex[p];
                        if (ptotal[tmpk] < begin) {
                            ptotal[tmpk] = count;
                            mat4.values[count] = final_pro;
                            mat4.colindex[count] = tmpk;
                            count++;
                        } else {
                            mat4.values[ptotal[tmpk]] += final_pro;
                        }
                    }
                } else {
                    for (int p = matr.rowshift[tmpj]; p < matr.rowshift[tmpj + 1]; p++) {
                        tmpk = matr.colindex[p];
                        final_pro = tmp_pro * matr.values[p];
                        mat4.values[ptotal[tmpk]] += final_pro;
                    }
                }
            }
        }
    }

    delete[] ptotal;

    return 0;
}