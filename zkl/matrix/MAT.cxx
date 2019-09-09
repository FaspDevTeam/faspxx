/**
 * a part of source code about mat.hxx
 */

#include "mat.hxx"
#include "fasp++.hxx"
#include <iostream>

/*---------------------------------------------------------------------------------*/

//! assign row, column, nnz, values, rowshift, colindex, diag to this->mat
MAT::MAT(INT row, INT column, INT nnz, std::vector<DBL> values,
         std::vector<INT> rowshift, std::vector<INT> colindex,
         std::vector<INT> diag) {

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