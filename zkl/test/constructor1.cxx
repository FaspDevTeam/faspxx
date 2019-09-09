/**
 * MAT source file
 */

#include "mat.hxx"
#include "fasp++.hxx"

/*----------------------------------------------------------------------------*/

MAT::MAT(INT row, INT column, INT nnz, std::vector<DBL>  values,
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

    if(((row>column)?column:row)!=diag.size())
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
            return 21; //! errortype 21 marks the empty row

        if(end==begin+1){
            if(colindex[begin]!=j)
                return 22; //! errortype 22 marks the No diagonal element in the row
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

    this->row=row;
    this->column=column;
    this->nnz=nnz;
    this->values.operator=(values);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->diag.operator=(diag);
}