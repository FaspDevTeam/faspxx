/**
 * MAT source file
 */

#include "mat.hxx"
#include "fasp++.hxx"

/*----------------------------------------------------------------------------*/

//! assign row, column, nnz, values, rowshift, colindex, diag to this->mat
MAT::MAT(INT row, INT column, INT nnz, std::vector<DBL>  values,
         std::vector<INT> rowshift, std::vector<INT> colindex,
         std::vector<INT> diag){
    this->row=row;
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->diag.operator=(diag);
}

//! assign row, column, nnz, values, rowshift, colindex to *this
MAT::MAT(INT row, INT column, INT nnz, std::vector<DBL> values,
         std::vector<INT> rowshift, std::vector<INT> colindex){
    this->row=row;
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    //! compute this->diag
    INT count=0;
    INT begin,end;
    for(INT j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(colindex[k]==j){
                diag[count]=k;
                count++;
            }
        }
    }
}

//! assign col, nnz, values, rowshift, colindex, diag to this->mat
MAT::MAT(INT column, INT nnz, std::vector<DBL> values, std::vector<INT> rowshift,
         std::vector<INT> colindex, std::vector<INT> diag){
    this->row=rowshift.size()-1;
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->diag.operator=(diag);

}

//! assign col, nnz, values, rowshift, colindex to this->mat
MAT::MAT(INT column, INT nnz, std::vector<DBL> values, std::vector<INT> rowshift,
         std::vector<INT> colindex){
    this->row=rowshift.size();
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);

    //! compute this->diag
    INT count=0;
    INT begin,end;
    for(INT j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(colindex[k]==j){
                diag[count]=k;
                count++;
            }
        }
    }
}

//! assign diagonal matrix to this->mat
MAT::MAT(VEC vec_obj){
    INT size=vec_obj.GetSize();
    if(size==0){
        column=0;
        row=0;
        nnz=0;
        this->values.reserve(0);
        this->colindex.reserve(0);
        this->rowshift.reserve(0);
        this->diag.reserve(0);
    }else{
        DBL *array;
        vec_obj.GetArray(row, &array);
        column = row;
        nnz = row;
        values.assign(array, array + row);
        rowshift.assign(iota_iterator(1), iota_iterator(row + 1));
        colindex.assign(iota_iterator(1), iota_iterator(row));
        diag.assign(iota_iterator(1), iota_iterator(row));
    }
}

//! assign MAT object to this->mat
MAT::MAT(const MAT& mat){
    this->row=mat.row;
    this->column=mat.column;
    this->nnz=mat.nnz;
    this->values.operator=(mat.values);
    this->rowshift.operator=(mat.rowshift);
    this->colindex.operator=(mat.colindex);
    this->diag.operator=(mat.diag);
}

//! overload equals operator
MAT& MAT::operator=(const MAT& mat){
    this->row=mat.row;
    this->column=mat.column;
    this->nnz=mat.nnz;
    this->values.operator=(mat.values);
    this->rowshift.operator=(mat.rowshift);
    this->colindex.operator=(mat.colindex);
    this->diag.operator=(mat.diag);
}

FaspErrorType MAT::CheckCSRx(INT row, INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex,
                             std::vector<INT> diag){
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    if(row!=rowshift.size()-1)
        return 10; //! errortype 10 marks the mismatch of row and (the dimension of rowshift minus 1)

    if(row<=0 || column <=0)
        return 11; //! errortype 11 marks the row or column index error

    if(((row>column)?column:row)==diag.size())
        return 12; //! errortype 12 marks the mismatch of min(row,column) and diagonal elements' number

    if(nnz!=colindex.size())
        return 13; //! errortype 13 marks the mismatch of nnz and column indices' size

    if(nnz!=values.size())
        return 14; //! errortype 14 marks the mismatch of nnz and values' size

    if(nnz!=rowshift[rowshift.size()-1])
        return 20; //! errortype 15 marks the mismatch of nnz and rowshift.size

    //! simple examinations
    for(int j=0;j<row;j++){
        if(rowshift[j]>rowshift[j+1])
            return 15; //! errortype 15 marks the rowshift elements' order error
    }

    if(rowshift[0]<0 || rowshift[row]>nnz)
        return 16; //! errortype 16 marks the rowshift elements' range error

    INT begin,end;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            continue;

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error
        }

        if(end>begin+1){
            for(int k=begin;k<end-1;k++){
                if(colindex[k]>colindex[k+1])
                    return 17; //! errortype 17 marks the colindex elements' order error
            }
            if(0>colindex[begin])
                return 18; //! errortype 18 marks the colindex elements' range error

            if(colindex[end-1]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error
        }
    }

    //! exam diag and colindex
    INT count=0;
    for(INT j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
        for(INT k=begin;k<end;k++){
            if(colindex[k]==j){
                if(diag[count]!=k)
                    return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
                else
                    count++;
            }
        }
    }
    if(count!=diag.size())
        return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error

    return 0;
}

FaspErrorType MAT::CheckCSRx(INT row, INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex){

    /*
 * some simple examinations about parameters
 * to judge whether they are CSRx' parameters
 */
    /*----------------  begin  ----------------*/
    //! basic examinations
    if(row!=rowshift.size()-1)
        return 10; //! errortype 10 marks the mismatch of row and (the dimension of rowshift minus 1)

    if(row<=0 || column <=0)
        return 11; //! errortype 11 marks the row or column index error

    if(nnz!=colindex.size())
        return 13; //! errortype 13 marks the mismatch of nnz and column indices' size

    if(nnz!=values.size())
        return 14; //! errortype 14 marks the mismatch of nnz and values' size

    if(nnz!=rowshift[rowshift.size()-1])
        return 20; //! errortype 15 marks the mismatch of nnz and rowshift.size

    //! simple examinations
    for(int j=0;j<row;j++){
        if(rowshift[j]>rowshift[j+1])
            return 15; //! errortype 15 marks the rowshift elements' order error
    }

    if(rowshift[0]<0 || rowshift[row]>nnz)
        return 16; //! errortype 16 marks the rowshift elements' range error

    INT begin,end;
    INT count=0;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end){
            return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
        }

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error

            if(colindex[begin]!=j){
                return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
            }else{
                count++;
            }
        }

        if(end>begin+1){
            for(int k=begin;k<end-1;k++){
                if(colindex[k]>colindex[k+1])
                    return 17; //! errortype 17 marks the colindex elements' order error

                if(colindex[k]==j)
                    count++;
            }
            if(colindex[end-1]==j)
                count++;

            if(0>colindex[begin])
                return 18; //! errortype 18 marks the colindex elements' range error

            if(colindex[end-1]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error

        }
    }

    if(count!=(row>column?column:row))
        return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error

    return 0;
}

FaspErrorType MAT::CheckCSRx(INT column,INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex,
                             std::vector<INT> diag){
    /*
 * some simple examinations about parameters
 * to judge whether they are CSRx' parameters
 */
    /*----------------  begin  ----------------*/
    //! basic examinations

    if(column <=0)
        return 11; //! errortype 11 marks the row or column index error

    if(((rowshift.size()>column)?column:rowshift.size())==diag.size())
        return 12; //! errortype 12 marks the mismatch of min(row,column) and diagonal elements' number

    if(nnz!=colindex.size())
        return 13; //! errortype 13 marks the mismatch of nnz and column indices' size

    if(nnz!=values.size())
        return 14; //! errortype 14 marks the mismatch of nnz and values' size

    if(nnz!=rowshift[rowshift.size()-1])
        return 20; //! errortype 15 marks the mismatch of nnz and rowshift.size

    //! simple examinations
    for(int j=0;j<rowshift.size();j++){
        if(rowshift[j]>rowshift[j+1])
            return 15; //! errortype 15 marks the rowshift elements' order error
    }

    if(rowshift[0]<0 || rowshift[rowshift.size()-1]>nnz)
        return 16; //! errortype 16 marks the rowshift elements' range error

    INT begin,end;
    for(int j=0;j<rowshift.size();j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            continue;

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error
        }

        if(end>begin+1){
            for(int k=begin;k<end-1;k++){
                if(colindex[k]>colindex[k+1])
                    return 17; //! errortype 17 marks the colindex elements' order error
            }
            if(0>colindex[begin])
                return 18; //! errortype 18 marks the colindex elements' range error

            if(colindex[end-1]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error
        }
    }

    //! exam diag and colindex
    INT count=0;
    for(INT j=0;j<rowshift.size();j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
        for(INT k=begin;k<end;k++){
            if(colindex[k]==j){
                if(diag[count]!=k)
                    return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
                else
                    count++;
            }
        }
    }
    if(count!=diag.size())
        return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error

    return 0;
}

FaspErrorType MAT::CheckCSRx(INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex){
    /*
 * some simple examinations about parameters
 * to judge whether they are CSRx' parameters
 */
    /*----------------  begin  ----------------*/
    //! basic examinations


    if(column <=0)
        return 11; //! errortype 11 marks the row or column index error

    if(nnz!=colindex.size())
        return 13; //! errortype 13 marks the mismatch of nnz and column indices' size

    if(nnz!=values.size())
        return 14; //! errortype 14 marks the mismatch of nnz and values' size

    if(nnz!=rowshift[rowshift.size()-1])
        return 20; //! errortype 15 marks the mismatch of nnz and rowshift.size

    //! simple examinations
    for(int j=0;j<rowshift.size();j++){
        if(rowshift[j]>rowshift[j+1])
            return 15; //! errortype 15 marks the rowshift elements' order error
    }

    if(rowshift[0]<0 || rowshift[rowshift.size()-1]>nnz)
        return 16; //! errortype 16 marks the rowshift elements' range error

    INT begin,end;
    INT count=0;
    for(int j=0;j<rowshift.size();j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end){
            return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
        }

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error

            if(colindex[begin]!=j){
                return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error
            }else{
                count++;
            }
        }

        if(end>begin+1){
            for(int k=begin;k<end-1;k++){
                if(colindex[k]>colindex[k+1])
                    return 17; //! errortype 17 marks the colindex elements' order error

                if(colindex[k]==j)
                    count++;
            }
            if(colindex[end-1]==j)
                count++;

            if(0>colindex[begin])
                return 18; //! errortype 18 marks the colindex elements' range error

            if(colindex[end-1]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error

        }
    }

    if(count!=(rowshift.size()>column?column:rowshift.size()))
        return 19; //! errortype 19 marks the diagonal index (perhaps not contained in colindex) error

    return 0;

}

FaspErrorType MAT::CheckCSR(INT row, INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex){
    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    if(row!=rowshift.size()-1)
        return 10; //! errortype 10 marks the mismatch of row and (the dimension of rowshift minus 1)

    if(row<=0 || column <=0)
        return 11; //! errortype 11 marks the row or column index error

    if(nnz!=colindex.size())
        return 13; //! errortype 13 marks the mismatch of nnz and column indices' size

    if(nnz!=values.size())
        return 14; //! errortype 14 marks the mismatch of nnz and values' size

    if(nnz!=rowshift[rowshift.size()-1])
        return 20; //! errortype 15 marks the mismatch of nnz and rowshift.size

    //! simple examinations
    for(int j=0;j<row;j++){
        if(rowshift[j]>rowshift[j+1])
            return 15; //! errortype 15 marks the rowshift elements' order error
    }

    if(rowshift[0]<0 || rowshift[row]>nnz)
        return 16; //! errortype 16 marks the rowshift elements' range error

    INT begin,end;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            continue;

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error
        }

        if(end>begin+1){
            for(int k=begin;k<end;k++){
                if(0>colindex[k] || colindex[k]>=column){
                    return 18; //! errortype 18 marks the colindex elements' range error
                }
            }
        }
    }

    return 0;
}

FaspErrorType MAT::CheckCSR(INT column, INT nnz, std::vector<DBL> values,
                            std::vector<INT> rowshift, std::vector<INT> colindex){
    /*
* some simple examinations about parameters
* to judge whether they are CSRx' parameters
*/
    /*----------------  begin  ----------------*/
    //! basic examinations

    if(column <=0)
        return 11; //! errortype 11 marks the row or column index error

    if(nnz!=colindex.size())
        return 13; //! errortype 13 marks the mismatch of nnz and column indices' size

    if(nnz!=values.size())
        return 14; //! errortype 14 marks the mismatch of nnz and values' size

    if(nnz!=rowshift[rowshift.size()-1])
        return 20; //! errortype 15 marks the mismatch of nnz and rowshift.size

    //! simple examinations
    for(int j=0;j<rowshift.size();j++){
        if(rowshift[j]>rowshift[j+1])
            return 15; //! errortype 15 marks the rowshift elements' order error
    }

    if(rowshift[0]<0 || rowshift[rowshift.size()-1]>nnz)
        return 16; //! errortype 16 marks the rowshift elements' range error

    INT begin,end;
    for(int j=0;j<rowshift.size();j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end)
            continue;

        if(end==begin+1){
            if(0>colindex[begin] || colindex[begin]>=column)
                return 18; //! errortype 18 marks the colindex elements' range error
        }

        if(end>begin+1){
            for(int k=begin;k<end;k++){
                if(0>colindex[k] || colindex[k]>=column){
                    return 18; //! errortype 18 marks the colindex elements' range error
                }
            }
        }
    }

    return 0;
}

FaspErrorType MAT::ConvertCSR(INT row, INT column, INT nnz, std::vector<DBL> values,
                              std::vector<INT> rowshift, std::vector<INT> colindex,MAT mat){
    INT begin,end;
    INT index;
    DBL data;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(end<=begin+1){
            continue;
        }
        for(int k=begin+1;k<end;k++){
            index=colindex[k];
            data=values[k];
            for(int j=k-1;j>=begin;j--){
                if(index<colindex[j]){
                    colindex[j+1]=colindex[j];
                    values[j+1]=values[j];
                }else{
                    break;
                }
            }
            colindex[j+1]=index;
            values[j+1]=data;
        }
    }


    mat.nnz=0;
    INT mark;
    mat.rowshift.reserve(row+1);
    mat.rowshift[0]=0;

    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end){
            mat.nnz++;
            mat.rowshift[j+1]=mat.rowshift[j]+1;
            continue;
        }
        if(begin==end-1){
            if(colindex[begin]==j){
                mat.nnz++;
                mat.rowshift[j+1]=mat.rowshift[j]+1;
            }else{
                mat.nnz+=2;
                mat.rowshift[j+1]=mat.rowshift[j]+2;
            }
            continue;
        }
        for(int k=begin;k<end-1;k++){
            if(colindex[k]<colindex[k+1]){
                mat.nnz++;
                if(colindex[k]==j){
                    mark=1;
                }
            }
        }
        if(colindex[end-2]<colindex[end-1]){
            mat.nnz++;
            if(colindex[end-1]==j){
                mark=1;
            }
        }

        if(mark!=1){
            mat.nnz++;
            mat.rowshift[j+1]=mat.rowshift[j]+end-begin+1;
        }else{
            mat.rowshift[j+1]=mat.rowshift[j]+end-begin;
            mark=0;
        }
    }

    mat.colindex.reserve(mat.nnz);
    mat.values.reserve(mat.nnz);

    INT count=0;
    INT less=0,equal=0,more=0;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end){
            mat.colindex[count]=j;
            mat.values[count]=0.0;
            count++;
            continue;
        }
        if(begin==end-1){
            if(colindex[begin]==j){
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
            }else{
                if(colindex[begin]>j){
                    mat.colindex[count]=j;
                    mat.values[count]=0.0;
                    count++;
                    mat.colindex[count]=colindex[begin];
                    mat.values[count]=values[begin];
                    count++;
                }
                if(colindex[begin]<j){
                    mat.colindex[count]=colindex[begin];
                    mat.values[count]=values[begin];
                    count++;
                    mat.colindex[count]=j;
                    mat.values[count]=0.0;
                    count++;
                }
            }
            continue;
        }
        if(begin==end-2){
            if(colindex[begin]<j && colindex[begin+1]<j && colindex[begin]<colindex[begin+1]){
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
                mat.colindex[count]=colindex[begin+1];
                mat.values[count]=values[begin+1];
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
            }
            if(colindex[begin]<j && colindex[begin]==colindex[begin+1]){
                mat.colindex[count]=colindex[begin];
                mat.values[count]+=(values[begin]+values[begin+1]);
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
            }
            if(colindex[begin]<j && colindex[begin+1]==j){
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
            }
            if(colindex[begin]==j && colindex[begin+1]==j){
                mat.colindex[count]=j;
                mat.values[count]+=(values[begin]+values[begin+1]);
            }
            if(colindex[begin]==j && colindex[begin+1]>j){
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                mat.colindex[count]=colindex[begin+1];
                mat.values[count]=values[begin+1];
                count++;
            }
            if(colindex[begin]>j && colindex[begin]==colindex[begin+1]){
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                mat.colindex[count]=colindex[begin];
                mat.values[count]+=(values[begin]+values[begin+1]);
                count++;
            }
            if(colindex[begin]>j && colindex[begin+1]>j && colindex[begin]<colindex[begin+1]){
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
                mat.colindex[count]=colindex[begin+1];
                mat.values[count]=values[begin+1];
                count++;
            }
            continue;
        }
        for(int k=begin;k<end-1;k++){
            if(colindex[k+1]<j && colindex[k]<colindex[k+1]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                continue;
            }
            if(colindex[k]<j && colindex[k]==colindex[k+1]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                continue;
            }
            if(colindex[k]<j && colindex[k+1]==j){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                continue;
            }
            if(colindex[k]==j && colindex[k+1]==j){
                mat.colindex[count]=j;
                mat.values[count]+=values[k];
                continue;
            }
            if(colindex[k]<j && colindex[k+1]>j){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                continue;
            }
            if(colindex[k]==j && colindex[k+1]>j){
                mat.colindex[count]=j;
                mat.values[count]+=values[k];
                count++;
                continue;
            }
            if(colindex[k]>j && colindex[k]==colindex[k+1]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                continue;
            }
            if(colindex[k]>j && colindex[k+1]>colindex[k]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                continue;
            }
        }
        if(colindex[end-2]<colindex[end-1] && colindex[end-1]<j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]==colindex[end-1] && colindex[end-1]<j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]+=values[end-1];
            count++;
        }
        if(colindex[end-2]<colindex[end-1] && colindex[end-1]==j){
            mat.colindex[count]=j;
            mat.values[count]=0.0;
            count++;
        }
        if(colindex[end-2]==colindex[end-1] && colindex[end-1]==j){
            mat.colindex[count]=j;
            mat.values[count]+=values[end-1];
            count++;
        }
        if(colindex[end-2]<j && colindex[end-1]>j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]>j && colindex[end-1]>colindex[end-2]){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]==j && colindex[end-1]>j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]>j && colindex[end-2]==colindex[end-1]){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
    }

    mat.diag.reserve(row>column?column:row);
    //! compute this->diag
    count=0;
    for(INT j=0;j<row;j++){
        begin=mat.rowshift[j];
        end=mat.rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(mat.colindex[k]==j){
                mat.diag[count]=k;
                count++;
            }
        }
    }
    return 0;
}

FaspErrorType MAT::ConvertCSR(INT column, INT nnz, std::vector<DBL> values,
                              std::vector<INT> rowshift, std::vector<INT> colindex, MAT mat){
    INT begin,end;
    INT index;
    INT row=rowshift.size()-1;
    DBL data;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(end<=begin+1){
            continue;
        }
        for(int k=begin+1;k<end;k++){
            index=colindex[k];
            data=values[k];
            for(int j=k-1;j>=begin;j--){
                if(index<colindex[j]){
                    colindex[j+1]=colindex[j];
                    values[j+1]=values[j];
                }else{
                    break;
                }
            }
            colindex[j+1]=index;
            values[j+1]=data;
        }
    }


    mat.nnz=0;
    INT mark;
    mat.rowshift.reserve(row+1);
    mat.rowshift[0]=0;

    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end){
            mat.nnz++;
            mat.rowshift[j+1]=mat.rowshift[j]+1;
            continue;
        }
        if(begin==end-1){
            if(colindex[begin]==j){
                mat.nnz++;
                mat.rowshift[j+1]=mat.rowshift[j]+1;
            }else{
                mat.nnz+=2;
                mat.rowshift[j+1]=mat.rowshift[j]+2;
            }
            continue;
        }
        for(int k=begin;k<end-1;k++){
            if(colindex[k]<colindex[k+1]){
                mat.nnz++;
                if(colindex[k]==j){
                    mark=1;
                }
            }
        }
        if(colindex[end-2]<colindex[end-1]){
            mat.nnz++;
            if(colindex[end-1]==j){
                mark=1;
            }
        }

        if(mark!=1){
            mat.nnz++;
            mat.rowshift[j+1]=mat.rowshift[j]+end-begin+1;
        }else{
            mat.rowshift[j+1]=mat.rowshift[j]+end-begin;
            mark=0;
        }
    }

    mat.colindex.reserve(mat.nnz);
    mat.values.reserve(mat.nnz);

    INT count=0;
    INT less=0,equal=0,more=0;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        if(begin==end){
            mat.colindex[count]=j;
            mat.values[count]=0.0;
            count++;
            continue;
        }
        if(begin==end-1){
            if(colindex[begin]==j){
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
            }else{
                if(colindex[begin]>j){
                    mat.colindex[count]=j;
                    mat.values[count]=0.0;
                    count++;
                    mat.colindex[count]=colindex[begin];
                    mat.values[count]=values[begin];
                    count++;
                }
                if(colindex[begin]<j){
                    mat.colindex[count]=colindex[begin];
                    mat.values[count]=values[begin];
                    count++;
                    mat.colindex[count]=j;
                    mat.values[count]=0.0;
                    count++;
                }
            }
            continue;
        }
        if(begin==end-2){
            if(colindex[begin]<j && colindex[begin+1]<j && colindex[begin]<colindex[begin+1]){
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
                mat.colindex[count]=colindex[begin+1];
                mat.values[count]=values[begin+1];
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
            }
            if(colindex[begin]<j && colindex[begin]==colindex[begin+1]){
                mat.colindex[count]=colindex[begin];
                mat.values[count]+=(values[begin]+values[begin+1]);
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
            }
            if(colindex[begin]<j && colindex[begin+1]==j){
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
            }
            if(colindex[begin]==j && colindex[begin+1]==j){
                mat.colindex[count]=j;
                mat.values[count]+=(values[begin]+values[begin+1]);
            }
            if(colindex[begin]==j && colindex[begin+1]>j){
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                mat.colindex[count]=colindex[begin+1];
                mat.values[count]=values[begin+1];
                count++;
            }
            if(colindex[begin]>j && colindex[begin]==colindex[begin+1]){
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                mat.colindex[count]=colindex[begin];
                mat.values[count]+=(values[begin]+values[begin+1]);
                count++;
            }
            if(colindex[begin]>j && colindex[begin+1]>j && colindex[begin]<colindex[begin+1]){
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                mat.colindex[count]=colindex[begin];
                mat.values[count]=values[begin];
                count++;
                mat.colindex[count]=colindex[begin+1];
                mat.values[count]=values[begin+1];
                count++;
            }
            continue;
        }
        for(int k=begin;k<end-1;k++){
            if(colindex[k+1]<j && colindex[k]<colindex[k+1]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                continue;
            }
            if(colindex[k]<j && colindex[k]==colindex[k+1]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                continue;
            }
            if(colindex[k]<j && colindex[k+1]==j){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                continue;
            }
            if(colindex[k]==j && colindex[k+1]==j){
                mat.colindex[count]=j;
                mat.values[count]+=values[k];
                continue;
            }
            if(colindex[k]<j && colindex[k+1]>j){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                mat.colindex[count]=j;
                mat.values[count]=0.0;
                count++;
                continue;
            }
            if(colindex[k]==j && colindex[k+1]>j){
                mat.colindex[count]=j;
                mat.values[count]+=values[k];
                count++;
                continue;
            }
            if(colindex[k]>j && colindex[k]==colindex[k+1]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                continue;
            }
            if(colindex[k]>j && colindex[k+1]>colindex[k]){
                mat.colindex[count]=colindex[k];
                mat.values[count]+=values[k];
                count++;
                continue;
            }
        }
        if(colindex[end-2]<colindex[end-1] && colindex[end-1]<j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]==colindex[end-1] && colindex[end-1]<j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]+=values[end-1];
            count++;
        }
        if(colindex[end-2]<colindex[end-1] && colindex[end-1]==j){
            mat.colindex[count]=j;
            mat.values[count]=0.0;
            count++;
        }
        if(colindex[end-2]==colindex[end-1] && colindex[end-1]==j){
            mat.colindex[count]=j;
            mat.values[count]+=values[end-1];
            count++;
        }
        if(colindex[end-2]<j && colindex[end-1]>j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]>j && colindex[end-1]>colindex[end-2]){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]==j && colindex[end-1]>j){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
        if(colindex[end-2]>j && colindex[end-2]==colindex[end-1]){
            mat.colindex[count]=colindex[end-1];
            mat.values[count]=values[end-1];
            count++;
        }
    }

    mat.diag.reserve(row>column?column:row);
    //! compute this->diag
    count=0;
    for(INT j=0;j<row;j++){
        begin=mat.rowshift[j];
        end=mat.rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(mat.colindex[k]==j){
                mat.diag[count]=k;
                count++;
            }
        }
    }
    return 0;
}

FaspErrorType MAT::MatAssign(INT row,INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex,
                             std::vector<INT> diag){
    this->row=row;
    this->column=column;
    this->nnz=nnz;
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->values.operator=(values);
    this->diag.operator=(diag);

    return 0;
}

FaspErrorType MAT::MatAssign(INT row,INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex){
    this->row=row;
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);

    this->diag.reserve(row>column?column:row);
    //! compute this->diag
    INT count=0;
    INT begin,end;
    for(INT j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(colindex[k]==j){
                this->diag[count]=k;
                count++;
            }
        }
    }

    return 0;
}

//! assign column, nnz, values, rowshift, colindex, diag to *this
FaspErrorType MAT::MatAssign(INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex,
                             std::vector<INT> diag){
    this->row=rowshift.size()-1;
    this->column=column;
    this->nnz=nnz;
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->values.operator=(values);
    this->diag.operator=(diag);

    return 0;
}

FaspErrorType MAT::MatAssign(INT column, INT nnz, std::vector<DBL> values,
                             std::vector<INT> rowshift, std::vector<INT> colindex){

    this->row=rowshift.size()-1;
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);

    this->diag.reserve(this->row>column?column:this->row);
    //! compute this->diag
    INT count=0;
    INT begin,end;
    for(INT j=0;j<this->row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        for(INT k=begin;k<end;k++){
            if(colindex[k]==j){
                this->diag[count]=k;
                count++;
            }
        }
    }

    return 0;
}

//! get the row or column number of this->mat
FaspErrorType MAT::GetSizes(INT &row, INT &column) {
    row = this->row;
    column = this->column;
    return 0;
}

//! get (this->mat).nnz
INT MAT::Getnnz() {
    return this->nnz;
}

//! get (this->mat)[i][j]
FaspErrorType MAT::GetElem(INT row, INT column, DBL &value) {
    if (row < 0 || row >= this->row)
        return 4; //! errortype 4 marks that the row index is not in 1 ~ this->row

    if (column < 0 || column >= this->column)
        return 5; //! errortype 5 marks that the column index is not in 1 ~ this->column

    if (colindex[rowshift[row-1]] <= column) {
        for (int j = colindex[rowshift[row-1]]; j < colindex[rowshift[row]]; j++) {
            if (column == colindex[j]) {
                value = values[j];
                return 0;
            }
        }
    }
    value = 0.0;

    return 0;
}

//! get the whole jth-row elements in this->mat into VEC object
FaspErrorType MAT::GetRow(INT row, VEC &vec_obj) {
    if(row<0 || row>=this->row)
        return 4; //! errortype 4 marks that the row index is not in 1 ~ this->row

    int len=rowshift[row]-rowshift[row-1];
    vec_obj.SetSize(len);
    INT k=0;
    for(int j=rowshift[row-1];j<rowshift[row];j++){
        vec_obj[k]=values[j];
        k++;
    }

    return 0;
}

//! get the whole jth-column elements in this->mat into VEC object
FaspErrorType MAT::GetColumn(INT column, VEC &vec_obj){
    INT count=0;
    if(column<0 || column>=this->column)
        return 5; //! errortype 5 marks the column index error

    vec_obj.SetSize(row);
    for(int j=0;j<row;j++){
        if(column>=colindex[rowshift[j]]){
            for(int k=rowshift[j];k<rowshift[j+1];j++){
                if(colindex[k]==column){
                    vec_obj[count]=values[k];
                    count++;
                    break;
                }
            }
        }
    }
    vec_obj.SetSize(count);
    return 0;
}

//! get the whole diagonal elements in this->mat into VEC object
FaspErrorType MAT::GetDiag(VEC &vec_obj){
    INT len=row>column?column:row;
    for(int j=0;j<len;j++)
        vec_obj[j]=values[diag[j]];

    return 0;
}

//! zero all the elements
FaspErrorType MAT::Zero(){
    nnz=row>column?column:row;
    std::vector<DBL> values_tmp(nnz,0.0);
    std::vector<INT> rowshift_tmp(iota_iterator(1),iota_iterator(row+1));
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
    for(INT j=0;j<nnz;j++){
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

//! vec2_obj = this->mat * vec1_obj
FaspErrorType MAT::MultVec(VEC vec1_obj, VEC &vec2_obj){
    INT begin, end;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        for(int k=begin;k<end;k++){
            vec2_obj[j]+=values[k]*vec1_obj[colindex[k]];
        }
    }

    return 0;
}

//! vec_z = vec_y + this->mat * vec_x
FaspErrorType MAT::MultAdd(VEC vec1_obj, VEC vec2_obj, VEC vec3_obj){
    INT begin, end;
    for(int j=0;j<row;j++){
        begin=rowshift[j];
        end=rowshift[j+1];
        for(int k=begin;k<end;k++){
            vec3_obj[j]+=values[k]*vec1_obj[colindex[k]]+vec2_obj[colindex[k]];
        }
    }
    return 0;
}

//! transpose this->mat
FaspErrorType MAT::Transpose(){

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

    for(int j=0;j<nnz;j++){
        count=rowshift[j];
        if(count<column-1){
            tmp.rowshift[count+2]++;
        }
    }

    for(int j=2;j<=column;j++)
        tmp.rowshift[j]+=tmp.rowshift[j-1];

    int l,k;
    if(this->values.size()){
        for(int j=0;j<row;j++){
            begin=rowshift[j];
            end=rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=colindex[p]+1;
                k=tmp.rowshift[l];
                tmp.colindex[k]=j;
                tmp.values[k]=values[p];
                tmp.rowshift[l]=k+1;
            }
        }
    }else{
        for(int j=0;j<row;j++){
            begin=rowshift[j];
            end=rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=colindex[p]+1;
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

    return 0;
}

//! this->mat = a * this->mat + b * mat
FaspErrorType MAT::Add(MAT mat, DBL a, DBL b){
    if(row!=mat.row || column!=mat.column)
        return 6; //! errortype 6 marks the mismatch of matrices' rows and columns

    int nnz_tmp=mat.Getnnz();
    int nnz_sum=nnz+nnz_tmp;
    MAT mat_tmp;
    //! mat != NULL and this->mat != NULL
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
        } // end for js

        for (int k=mat.rowshift[i]; k<mat.rowshift[i+1]; k++) {
            added = 0;

            for (int l=mat_tmp.rowshift[i]; l<mat_tmp.rowshift[i]+countrow+1; l++) {
                if (mat.colindex[k] == mat_tmp.colindex[l]) {
                    mat_tmp.values[l] = mat_tmp.values[l] + b * mat.values[k];
                    added = 1;
                    break;
                }
            } // end for l

            if (added == 0) {
                mat_tmp.values[count] = b * mat.values[k];
                mat_tmp.colindex[count] = mat.colindex[k];
                mat_tmp.rowshift[i+1]++;
                count++;
            }

        } // end for k

        mat_tmp.rowshift[i+1] += mat_tmp.rowshift[i];

    }

    mat_tmp.nnz = count;

    INT begin,end;
    count=0;
    for(INT j=0;j<row;j++){
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

//! this->mat = this->mat * mat
FaspErrorType MAT::MultLeft(MAT mat){
    if(this->column!=mat.row)
        return 7; //! errortype 7 marks the mismatch between row and column

    INT count,l;
    MAT tmp_mat;
    INT *tmp = new INT[mat.column];

    tmp_mat.row=this->row;

    tmp_mat.column=mat.column;
    tmp_mat.rowshift.reserve(tmp_mat.row+1);

    for (int i=0;i<mat.column;++i) tmp[i]=-1;

    // step 1: Find first the structure IA of C
    for (int i=0;i<tmp_mat.row;++i) {
        count=0;

        for (int k=this->rowshift[i];k<this->rowshift[i+1];++k) {
            for (int j=mat.rowshift[this->colindex[k]];j<mat.rowshift[this->colindex[k]+1];++j) {
                for (l=0;l<count;l++) {
                    if (tmp[l]==mat.colindex[j]) break;
                }

                if (l==count) {
                    tmp[count]=mat.colindex[j];
                    count++;
                }
            }
        }
        tmp_mat.rowshift[i+1]=count;
        for (int j=0;j<count;++j) {
            tmp[j]=-1;
        }
    }

    for (int i=0;i<tmp_mat.row;++i) tmp_mat.rowshift[i+1]+=tmp_mat.rowshift[i];

    // step 2: Find the structure JA of C
    INT count_tmp;

    tmp_mat.colindex.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;++i) {
        count_tmp=0;
        count=tmp_mat.rowshift[i];
        for (int k=this->rowshift[i];k<this->rowshift[i+1];++k) {
            for (int j=mat.rowshift[this->colindex[k]];j<mat.rowshift[this->colindex[k]+1];++j) {
                for (l=0;l<count_tmp;l++) {
                    if (tmp[l]==mat.colindex[j]) break;
                }

                if (l==count_tmp) {
                    tmp_mat.colindex[count]=mat.colindex[j];
                    tmp[count_tmp]=mat.colindex[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for (int j=0;j<count_tmp;++j) tmp[j]=-1;
    }

    delete[] tmp;

    // step 3: Find the structure A of C
    tmp_mat.values.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;++i) {
        for (int j=tmp_mat.rowshift[i];j<tmp_mat.rowshift[i+1];++j) {
            tmp_mat.values[j]=0;
            for (int k=this->rowshift[i];k<this->rowshift[i+1];++k) {
                for (l=mat.rowshift[this->colindex[k]];l<mat.rowshift[this->colindex[k]+1];l++) {
                    if (mat.colindex[l]==tmp_mat.colindex[j]) {
                        tmp_mat.values[j]+=this->values[k]*mat.values[l];
                    } // end if
                } // end for l
            } // end for k
        } // end for j
    }    // end for i

    tmp_mat.nnz = tmp_mat.rowshift[tmp_mat.row]-tmp_mat.rowshift[0];
    this->operator=(tmp_mat);
    return 0;
}

//! this->mat =  mat * this->mat
FaspErrorType MAT::MultRight(MAT mat){
    if(mat.column!=this->row)
        return 7; //! errortype 7 marks the mismatch between row and column

    INT l,count;
    MAT tmp_mat;
    INT *tmp = new INT[this->column];

    tmp_mat.row=mat.row;
    tmp_mat.column=this->column;
    tmp_mat.rowshift.reserve(tmp_mat.row+1);

    for (int i=0;i<this->column;++i) tmp[i]=-1;

    // step 1: Find first the structure IA of C
    for (int i=0;i<tmp_mat.row;++i) {
        count=0;

        for (int k=mat.rowshift[i];k<mat.rowshift[i+1];++k) {
            for (int j=this->rowshift[mat.colindex[k]];j<this->rowshift[mat.colindex[k]+1];++j) {
                for (l=0;l<count;l++) {
                    if (tmp[l]==this->colindex[j]) break;
                }

                if (l==count) {

                    tmp[count]=this->colindex[j];

                    count++;
                }
            }
        }
        tmp_mat.rowshift[i+1]=count;
        for (int j=0;j<count;++j) {
            tmp[j]=-1;
        }
    }

    for (int i=0;i<tmp_mat.row;++i) tmp_mat.rowshift[i+1]+=tmp_mat.rowshift[i];

    INT count_tmp;

    tmp_mat.colindex.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;++i) {
        count_tmp=0;
        count=tmp_mat.rowshift[i];
        for (int k=mat.rowshift[i];k<mat.rowshift[i+1];++k) {
            for (int j=this->rowshift[mat.colindex[k]];j<this->rowshift[mat.colindex[k]+1];++j) {
                for (l=0;l<count_tmp;l++) {
                    if (tmp[l]==this->colindex[j]) break;
                }

                if (l==count_tmp) {
                    tmp_mat.colindex[count]=this->colindex[j];
                    tmp[count_tmp]=this->colindex[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for (int j=0;j<count_tmp;++j) tmp[j]=-1;
    }

    delete[] tmp;

    // step 3: Find the structure A of C
    tmp_mat.values.reserve(tmp_mat.rowshift[tmp_mat.row]);

    for (int i=0;i<tmp_mat.row;++i) {
        for (int j=tmp_mat.rowshift[i];j<tmp_mat.rowshift[i+1];++j) {
            tmp_mat.values[j]=0;
            for (int k=mat.rowshift[i];k<mat.rowshift[i+1];++k) {
                for (l=this->rowshift[mat.colindex[k]];l<this->rowshift[mat.colindex[k]+1];l++) {
                    if (this->colindex[l]==tmp_mat.colindex[j]) {
                        tmp_mat.values[j]+=mat.values[k]*this->values[l];
                    } // end if
                } // end for l
            } // end for k
        } // end for j
    }    // end for i

    tmp_mat.nnz = tmp_mat.rowshift[tmp_mat.row]-tmp_mat.rowshift[0];

    this->operator=(tmp_mat);
    return 0;
}

//! this->mat = mat_left * this->mat * mat_right
//! (m * n) * (n * n) * (n * m)
FaspErrorType MAT::MultLeftRight(MAT matl, MAT matr){
    MAT tmpmat;

    tmpmat.row=matl.row;
    tmpmat.column=matr.column;

    INT tmpi, tmpj, tmpk;
    INT count, begin;
    DBL tmp, tmp_pro, final_pro;

    INT total = matl.row+this->row + matl.row+1 + 1;

    INT *ptotal = new INT[total];
    INT *phalf = ptotal + matl.row;

    /*------------------------------------------------------*
     *  First Pass: Determine size of RAP and set up tmp.rowshift  *
     *------------------------------------------------------*/
    tmpmat.rowshift.reserve(matl.row+1);

    for(int j=0;j<matl.row+this->row;j++){
        ptotal[j]=-1;
    }

    count = 0;
    for (int j = 0; j < matl.row; j ++) {
        ptotal[j] = count;
        begin = count;
        count ++;

        for (int k = matl.rowshift[j]; k < matl.rowshift[j+1]; k ++) {
            tmpi = matl.colindex[k];

            for (int l = this->rowshift[tmpi]; l < this->rowshift[tmpi+1]; l ++) {
                tmpj = this->colindex[l];
                if (phalf[tmpj] != j) {
                    phalf[tmpj] = j;
                    for (int p = matr.rowshift[tmpj]; p < matr.rowshift[tmpj+1]; p ++) {
                        tmpk = matr.colindex[p];
                        if (ptotal[tmpk] < begin) {
                            ptotal[tmpk] = count;
                            count ++;
                        }
                    }
                }
            }
        }

        tmpmat.rowshift[j] = begin;
    }

    tmpmat.rowshift[matl.row] = count;
    tmpmat.nnz = count;

    tmpmat.colindex.reserve(tmpmat.nnz);
    tmpmat.values.reserve(tmpmat.nnz);

    for(int j=0;j<matl.row+this->row;j++){
        ptotal[j]=-1;
    }

    count = 0;
    for (int j = 0; j < matl.row; j ++) {
        ptotal[j] = count;
        begin = count;
        tmpmat.colindex[count] = j;
        tmpmat.values[count] = 0.0;
        count ++;

        for (int k = matl.rowshift[j]; k < matl.rowshift[j+1]; k ++) {
            tmp = matl.values[k];

            tmpi = matl.colindex[k];
            for (int l = this->rowshift[tmpi]; l < this->rowshift[tmpi+1]; l ++) {
                tmp_pro = tmp * this->values[l];

                tmpj = this->colindex[l];
                if (phalf[tmpj] != j) {
                    phalf[tmpj] = j;
                    for (int p = matr.rowshift[tmpj]; p < matr.rowshift[tmpj+1]; p ++) {
                        final_pro = tmp_pro * matr.values[p];

                        tmpk = matr.colindex[p];
                        if (ptotal[tmpk] < begin) {
                            ptotal[tmpk] = count;
                            tmpmat.values[count] = final_pro;
                            tmpmat.colindex[count] = tmpk;
                            count ++;
                        }
                        else {
                            tmpmat.values[ptotal[tmpk]] += final_pro;
                        }
                    }
                }
                else {
                    for (int p = matr.rowshift[tmpj]; p < matr.rowshift[tmpj+1]; p ++) {
                        tmpk = matr.colindex[p];
                        final_pro = tmp_pro * matr.values[p];
                        tmpmat.values[ptotal[tmpk]] += final_pro;
                    }
                }
            }
        }
    }

    this->operator=(tmpmat);

    delete[] ptotal;

    return 0;
}

//! this->mat = transpose(P) * this->mat * P
FaspErrorType MAT::MultP(MAT P) {
    MAT Ptrans(P);
    Ptrans.Transpose();

    MAT tmpmat;

    tmpmat.row=Ptrans.row;
    tmpmat.column=P.column;

    INT tmpi, tmpj, tmpk;
    INT count, begin;
    DBL tmp, tmp_pro, final_pro;

    INT total = Ptrans.row+this->row + Ptrans.row+1 + 1;

    INT *ptotal = new INT[total];
    INT *phalf = ptotal + Ptrans.row;

    /*------------------------------------------------------*
     *  First Pass: Determine size of RAP and set up tmp.rowshift  *
     *------------------------------------------------------*/
    tmpmat.rowshift.reserve(Ptrans.row+1);

    for(int j=0;j<Ptrans.row+this->row;j++){
        ptotal[j]=-1;
    }

    count = 0;
    for (int j = 0; j < Ptrans.row; j ++) {
        ptotal[j] = count;
        begin = count;
        count ++;

        for (int k = Ptrans.rowshift[j]; k < Ptrans.rowshift[j+1]; k ++) {
            tmpi = Ptrans.colindex[k];

            for (int l = this->rowshift[tmpi]; l < this->rowshift[tmpi+1]; l ++) {
                tmpj = this->colindex[l];
                if (phalf[tmpj] != j) {
                    phalf[tmpj] = j;
                    for (int p = P.rowshift[tmpj]; p <P.rowshift[tmpj+1]; p ++) {
                        tmpk = P.colindex[p];
                        if (ptotal[tmpk] < begin) {
                            ptotal[tmpk] = count;
                            count ++;
                        }
                    }
                }
            }
        }

        tmpmat.rowshift[j] = begin;
    }

    tmpmat.rowshift[Ptrans.row] = count;
    tmpmat.nnz = count;

    tmpmat.colindex.reserve(tmpmat.nnz);
    tmpmat.values.reserve(tmpmat.nnz);

    for(int j=0;j<Ptrans.row+this->row;j++){
        ptotal[j]=-1;
    }

    count = 0;
    for (int j = 0; j < Ptrans.row; j ++) {
        ptotal[j] = count;
        begin = count;
        tmpmat.colindex[count] = j;
        tmpmat.values[count] = 0.0;
        count ++;

        for (int k = Ptrans.rowshift[j]; k < Ptrans.rowshift[j+1]; k ++) {

            tmpi = Ptrans.colindex[k];
            for (int l = this->rowshift[tmpi]; l < this->rowshift[tmpi+1]; l ++) {
                tmp_pro = this->values[l];

                tmpj = this->colindex[l];
                if (phalf[tmpj] != j) {
                    phalf[tmpj] = j;
                    for (int p = P.rowshift[tmpj]; p < P.rowshift[tmpj+1]; p ++) {
                        final_pro = tmp_pro;

                        tmpk = P.colindex[p];
                        if (ptotal[tmpk] < begin) {
                            ptotal[tmpk] = count;
                            tmpmat.values[count] = final_pro;
                            tmpmat.colindex[count] = tmpk;
                            count ++;
                        }
                        else {
                            tmpmat.values[ptotal[tmpk]] += final_pro;
                        }
                    }
                }
                else {
                    for (int p = P.rowshift[tmpj]; p < P.rowshift[tmpj+1]; p ++) {
                        tmpk = P.colindex[p];
                        final_pro = tmp_pro;
                        tmpmat.values[ptotal[tmpk]] += final_pro;
                    }
                }
            }
        }
    }

    this->operator=(tmpmat);

    delete[] ptotal;
}

//! vec_z = vec_y + transpose(this->mat) * vec_x
FaspErrorType MAT::MultTransposeAdd(VEC vec_x, VEC vec_y, VEC vec_z){
    if(row!=vec_x.GetSize())
        return 21; //! errortype 21 marks the mismatch of matrix's row and vector' size

    if(column!=vec_y.GetSize())
        return 21; //! errortype 21 marks the mismatch of matrix's column and vector' size

    vec_z.operator=(vec_y);

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

    for(int j=0;j<nnz;j++){
        count=rowshift[j];
        if(count<column-1){
            tmp.rowshift[count+2]++;
        }
    }

    for(int j=2;j<=column;j++)
        tmp.rowshift[j]+=tmp.rowshift[j-1];

    int l,k;
    if(this->values.size()){
        for(int j=0;j<row;j++){
            begin=rowshift[j];
            end=rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=colindex[p]+1;
                k=tmp.rowshift[l];
                tmp.colindex[k]=j;
                tmp.values[k]=values[p];
                tmp.rowshift[l]=k+1;
            }
        }
    }else{
        for(int j=0;j<row;j++){
            begin=rowshift[j];
            end=rowshift[j+1];
            for(int p=begin;p<end;p++){
                l=colindex[p]+1;
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
                vec_z[j]+=vec_x[colindex[k]]*tmp.values[k];
            }
        }
    }else{
        for(int j=0;j<tmp.row;j++){
            begin=tmp.rowshift[j];
            end=tmp.rowshift[j+1];
            for(int k=begin;k<end;k++){
                vec_z[j]+=vec_x[colindex[k]];
            }
        }
    }

    return 0;
}