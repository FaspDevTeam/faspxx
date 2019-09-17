/*! \file vec.cxx
 *  \brief Source file for the FASP++ Matrix class
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */
#include "mat.hxx"
#include "error.hxx"
#include <iostream>

/*---------------------------------------------------------------------------------*/
/**
 * if "row == 0" or "column ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, column, nnz, values, rowshift, colindex, diag to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
values, const std::vector<INT> rowshift, const std::vector<INT>
         colindex, const std::vector<INT> diag) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->diag.resize(0);
        this->colindex.resize(0);
        this->values.resize(0);
        return;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if (((row > column) ? column : row) != diag.size())
        goto Return;

    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] >= rowshift[j + 1] ) {
            goto Return;
        }
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            goto Return;

        if ( end == begin + 1 ) {
            if ( colindex[begin] != j )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; k++ ) {
                if ( colindex[k] >= colindex[k + 1] )
                    goto Return;
            }
            if ( 0 > colindex[begin] )
                goto Return;

            if ( colindex[end - 1] >= column )
                goto Return;
        }
    }

    //! exam diag and colindex
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( colindex[k] == j ) {
                if ( diag[count] != k )
                    goto Return;
                else
                    count++;
            }
        }
    }
    if ( count != diag.size())
        goto Return;

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);

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

/**
 * if "row == 0" or "column ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 * attention : all the CSRx data 's values are one
 */
//! assign row, column, nnz, rowshift, colindex, diag to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
rowshift, const std::vector<INT> colindex, const std::vector<INT>
         diag) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->colindex.resize(0);
        this->values.resize(0);
        this->rowshift.resize(0);
        this->diag.resize(0);
        return;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if (((row > column) ? column : row) != diag.size())
        goto Return;


    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] >= rowshift[j + 1] ) {
            goto Return;
        }
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            goto Return;

        if ( end == begin + 1 ) {
            if ( colindex[begin] != j )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; k++ ) {
                if ( colindex[k] >= colindex[k + 1] )
                    goto Return;
            }
            if ( 0 > colindex[begin] )
                goto Return;

            if ( colindex[end - 1] >= column )
                goto Return;
        }
    }

    //! exam diag and colindex
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( colindex[k] == j ) {
                if ( diag[count] != k )
                    goto Return;
                else
                    count++;
            }
        }
    }
    if ( count != diag.size())
        goto Return;

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);

        return;
    }

    this->row = row;
    this->column = column;
    this->nnz = nnz;
    this->values.resize(0);
    this->rowshift.operator=(rowshift);
    this->colindex.operator=(colindex);
    this->diag.operator=(diag);
}

/**
 * if "row == 0" or "column ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, column, nnz, values, rowshift, colindex to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
values, const std::vector<INT> rowshift, const std::vector<INT> colindex) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->diag.resize(0);
        this->rowshift.resize(0);
        this->values.resize(0);
        this->colindex.resize(0);
        return;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] > rowshift[j + 1] )
            goto Return;
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colindex[begin] || colindex[begin] >= column )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colindex[k] || colindex[k] >= column ) {
                    goto Return;
                }
            }
        }
    }

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);

        return;
    }

    INT index;
    DBL data;
    std::vector<INT> tmprowshift, tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.operator=(values);

    //sort
    INT l;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolindex[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolindex[l] ) {
                    tmpcolindex[l + 1] = tmpcolindex[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolindex[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->column = column;
    this->nnz = 0;
    this->rowshift.resize(row + 1);
    this->rowshift[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            this->nnz++;
            this->rowshift[j + 1] = this->rowshift[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                this->nnz++;
                this->rowshift[j + 1] = this->rowshift[j] + 1;
            } else {
                this->nnz += 2;
                this->rowshift[j + 1] = this->rowshift[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k] < tmpcolindex[k + 1] ) {
                this->nnz++;
                if ( tmpcolindex[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] ) {
            this->nnz++;
            if ( tmpcolindex[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
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
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolindex[begin] > j ) {
                    this->colindex[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colindex[count] = tmpcolindex[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolindex[begin] < j ) {
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
        if ( begin == end - 2 ) {
            if ( tmpcolindex[begin + 1] < j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
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
            if ( tmpcolindex[begin] < j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j && tmpcolindex[begin + 1] == j ) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] == j ) {
                this->colindex[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] > j ) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = tmpcolindex[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
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
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k + 1] < j && tmpcolindex[k] < tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] == j ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] == j ) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] > j ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] > j ) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k + 1] > tmpcolindex[k] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            this->colindex[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] < j && tmpcolindex[end - 1] > j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 1] > tmpcolindex[end - 2] ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] == j && tmpcolindex[end - 1] > j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 2] == tmpcolindex[end - 1] ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diag.resize(row > column ? column : row);
    //! compute this->diag
    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowshift[j];
        end = this->rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colindex[k] == j ) {
                this->diag[count] = k;
                count++;
            }
        }
    }

}

/**
 * if "row == 0" or "column ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 * attention : all the elements in the CSRx 's values are one
 */
//! assign row, column, nnz, rowshift, colindex to *this
MAT::MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
rowshift, const std::vector<INT> colindex) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->colindex.resize(0);
        this->diag.resize(0);
        this->rowshift.resize(0);
        this->values.resize(0);
        return;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] > rowshift[j + 1] )
            goto Return;
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colindex[begin] || colindex[begin] >= column )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colindex[k] || colindex[k] >= column ) {
                    goto Return;
                }
            }
        }
    }

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);

        return;
    }

    INT index, l;
    DBL data;
    std::vector<INT> tmprowshift, tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.assign(rowshift[rowshift.size() - 1], 1);
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolindex[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolindex[l] ) {
                    tmpcolindex[l + 1] = tmpcolindex[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolindex[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->column = column;
    this->nnz = 0;
    this->rowshift.resize(row + 1);
    this->rowshift[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            this->nnz++;
            this->rowshift[j + 1] = this->rowshift[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                this->nnz++;
                this->rowshift[j + 1] = this->rowshift[j] + 1;
            } else {
                this->nnz += 2;
                this->rowshift[j + 1] = this->rowshift[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k] < tmpcolindex[k + 1] ) {
                this->nnz++;
                if ( tmpcolindex[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] ) {
            this->nnz++;
            if ( tmpcolindex[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
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
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolindex[begin] > j ) {
                    this->colindex[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colindex[count] = tmpcolindex[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolindex[begin] < j ) {
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
        if ( begin == end - 2 ) {
            if ( tmpcolindex[begin + 1] < j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
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
            if ( tmpcolindex[begin] < j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j && tmpcolindex[begin + 1] == j ) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] == j ) {
                this->colindex[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] > j ) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = tmpcolindex[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
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
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k + 1] < j && tmpcolindex[k] < tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] == j ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] == j ) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] > j ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] > j ) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k + 1] > tmpcolindex[k] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            this->colindex[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] < j && tmpcolindex[end - 1] > j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 1] > tmpcolindex[end - 2] ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] == j && tmpcolindex[end - 1] > j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 2] == tmpcolindex[end - 1] ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diag.resize(row > column ? column : row);
    //! compute this->diag
    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowshift[j];
        end = this->rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colindex[k] == j ) {
                this->diag[count] = k;
                count++;
            }
        }
    }

}

//! assign VEC object to form the diagonal matrix
MAT::MAT(VEC &vec_obj) {
    INT size = vec_obj.GetSize();
    if ( size == 0 ) {
        this->column = 0;
        this->row = 0;
        this->nnz = 0;
        this->values.resize(0);
        this->colindex.resize(0);
        this->rowshift.resize(0);
        this->diag.resize(0);
    } else {
        INT *array = new INT[size];
        for ( INT j = 0; j < size; j++ )
            array[j] = j;

        this->row = size;
        this->column = size;
        this->nnz = size;
        this->rowshift.resize(size + 1);
        this->rowshift.assign(array, array + size);
        this->rowshift[size] = size;
        this->colindex.resize(size);
        this->colindex.assign(array, array + size);
        this->values.resize(size);

        for ( INT j = 0; j < size; j++ )
            this->values[j] = vec_obj[j];

        this->diag.resize(size);
        this->diag.assign(array, array + size);

        delete[] array;
        array = nullptr;
    }
}

//! assign vector object to form the diagonal matrix
MAT::MAT(const std::vector<DBL> &vector_obj) {
    INT size = vector_obj.size();
    if ( size == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);
    } else {
        this->row = size;
        this->column = size;
        this->nnz = size;

        INT *p = new INT[size];
        for ( INT j = 0; j < size; j++ )
            p[j] = j;

        this->rowshift.resize(size + 1);
        this->rowshift.assign(p, p + size);
        this->rowshift[size] = size;
        this->colindex.resize(size);
        this->colindex.assign(p, p + size);
        this->diag.resize(size);
        this->diag.assign(p, p + size);
        this->values.resize(size);
        this->values.assign(vector_obj.begin(), vector_obj.begin() + size);

        delete[] p;
        p = nullptr;
    }
}

//! assign MAT object to *this
MAT::MAT(const MAT &mat) {
    this->row = mat.row;
    this->column = mat.column;
    this->nnz = mat.nnz;
    this->values.operator=(mat.values);
    this->diag.operator=(mat.diag);
    this->colindex.operator=(mat.colindex);
    this->rowshift.operator=(mat.rowshift);
}

//! overload = operator
MAT &MAT::operator=(const MAT &mat) {
    this->row = mat.row;
    this->column = mat.column;
    this->nnz = mat.nnz;
    this->diag.operator=(mat.diag);
    this->colindex.operator=(mat.colindex);
    this->rowshift.operator=(mat.rowshift);
    this->values.operator=(mat.values);
}
/**
 * if "row == 0" or "column ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, column, nnz, values, rowshift, colindex, diag to *this
void MAT::SetValues(const INT row, const INT column, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowshift,
                    const std::vector<INT> colindex, const std::vector<INT> diag) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->diag.resize(0);
        this->colindex.resize(0);
        this->values.resize(0);
        return;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if (((row > column) ? column : row) != diag.size())
        goto Return;

    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] >= rowshift[j + 1] ) {
            goto Return;
        }
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            goto Return;

        if ( end == begin + 1 ) {
            if ( colindex[begin] != j )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; k++ ) {
                if ( colindex[k] >= colindex[k + 1] )
                    goto Return;
            }
            if ( 0 > colindex[begin] )
                goto Return;

            if ( colindex[end - 1] >= column )
                goto Return;
        }
    }

    //! exam diag and colindex
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( colindex[k] == j ) {
                if ( diag[count] != k )
                    goto Return;
                else
                    count++;
            }
        }
    }
    if ( count != diag.size())
        goto Return;

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);

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

/**
 * if "row == 0" or "column ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, column, nnz, values, rowshift, colindex to *this
void MAT::SetValues(const INT row, const INT column, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowshift,
                    const std::vector<INT> colindex) {

    if ( row == 0 || column == 0 || nnz == 0 ) {
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->diag.resize(0);
        this->colindex.resize(0);
        this->values.resize(0);
        return;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] > rowshift[j + 1] )
            goto Return;
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colindex[begin] || colindex[begin] >= column )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colindex[k] || colindex[k] >= column ) {
                    goto Return;
                }
            }
        }
    }

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->column = 0;
        this->nnz = 0;
        this->rowshift.resize(0);
        this->colindex.resize(0);
        this->diag.resize(0);
        this->values.resize(0);

        return;
    }

    INT index, l;
    DBL data;
    std::vector<INT> tmprowshift, tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.operator=(values);
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolindex[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolindex[l] ) {
                    tmpcolindex[l + 1] = tmpcolindex[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolindex[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->column = column;
    this->nnz = 0;
    this->rowshift.resize(row + 1);
    this->rowshift[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            this->nnz++;
            this->rowshift[j + 1] = this->rowshift[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                this->nnz++;
                this->rowshift[j + 1] = this->rowshift[j] + 1;
            } else {
                this->nnz += 2;
                this->rowshift[j + 1] = this->rowshift[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k] < tmpcolindex[k + 1] ) {
                this->nnz++;
                if ( tmpcolindex[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] ) {
            this->nnz++;
            if ( tmpcolindex[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
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
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolindex[begin] > j ) {
                    this->colindex[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colindex[count] = tmpcolindex[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolindex[begin] < j ) {
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
        if ( begin == end - 2 ) {
            if ( tmpcolindex[begin + 1] < j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
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
            if ( tmpcolindex[begin] < j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j && tmpcolindex[begin + 1] == j ) {
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] == j ) {
                this->colindex[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] > j ) {
                this->colindex[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colindex[count] = tmpcolindex[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colindex[count] = tmpcolindex[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
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
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k + 1] < j && tmpcolindex[k] < tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] == j ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] == j ) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] > j ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colindex[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] > j ) {
                this->colindex[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k + 1] > tmpcolindex[k] ) {
                this->colindex[count] = tmpcolindex[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            this->colindex[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            this->colindex[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] < j && tmpcolindex[end - 1] > j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 1] > tmpcolindex[end - 2] ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] == j && tmpcolindex[end - 1] > j ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 2] == tmpcolindex[end - 1] ) {
            this->colindex[count] = tmpcolindex[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diag.resize(this->row > this->column ? this->column : this->row);
    //! compute this->diag
    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = this->rowshift[j];
        end = this->rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( this->colindex[k] == j ) {
                this->diag[count] = k;
                count++;
            }
        }
    }
}

//! get the row or column number of *this
void MAT::GetSizes(INT &row, INT &column) const {
    row = this->row;
    column = this->column;
}

//! get this->nnz
INT MAT::Getnnz() const {
    return this->nnz;
}

/**
 * if "row < 0" or "row > this->row" or "column < 0" or "column >=this->coliumn"
 * happens, throw an exception. In other cases,it is normally dealt.
 */
//! get (*this)[i][j]
DBL MAT::GetElem(const INT row, const INT column) const {
    try {
        if ( row < 0 || row >= this->row || column < 0 || column >= this->column ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;

        return 0;
    }

    if ( this->colindex[this->rowshift[row]] <= column ) {
        for ( INT j = this->rowshift[row]; j < this->rowshift[row + 1]; j++ ) {
            if ( column == this->colindex[j] ) {
                if ( this->values.size() == 0 ) {
                    return 1.0;
                } else {
                    return (this->values[j]);
                }
            }
        }
    }
}

/**
 * if "row < 0" or "row > this->row" happens,
 * throw an exception. In other cases,it is normally dealt.
 */
//! get the whole jth-row elements in *this into vector object
std::vector<DBL> MAT::GetRow(const INT row) const {
    std::vector<DBL> vec;
    try {
        if ( row < 0 || row >= this->row ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        vec.resize(0);
        return vec;
    }

    INT len = this->rowshift[row + 1] - this->rowshift[row];
    vec.resize(len);
    if ( this->values.size() == 0 )
        vec.assign(len, 1);

    INT k = 0;
    for ( INT j = this->rowshift[row]; j < this->rowshift[row + 1]; j++ ) {
        vec[k] = this->values[j];
        k++;
    }

    return vec;
}

/**
 * if "column < 0" or "column > this->row" happens,
 * throw an exception. In other cases,it is normally dealt.
 */
//! get the whole jth-column elements in *this into vector object
std::vector<DBL> MAT::GetColumn(const INT column)
const {
    std::vector<DBL> vec;
    try {
        if ( column < 0 || column >= this->column ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        vec.resize(0);
        return vec;
    }

    INT count = 0;
    vec.resize(this->row);
    for ( INT j = 0; j < this->row; j++ ) {
        if ( column >= this->colindex[this->rowshift[j]] ) {
            for ( INT k = this->rowshift[j]; k < this->rowshift[j + 1]; k++ ) {
                if ( this->colindex[k] == column ) {
                    if ( this->values.size() == 0 ) {
                        vec[count] = 1;
                    } else {
                        vec[count] = this->values[k];
                    }
                    count++;
                    break;
                }
            }
        }
    }
    vec.resize(count);

    return vec;
}

//! get the whole diagonal elements in *this into VEC object
std::vector<DBL> MAT::GetDiag() const {
    std::vector<DBL> vec;
    INT len = this->row > this->column ? this->column : this->row;
    vec.resize(len);
    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < len; j++ )
            vec[j] = this->values[this->diag[j]];
    } else {
        vec.assign(len, 1);
    }

    return vec;
}

//! zero all the elements
void MAT::Zero() {
    this->nnz = this->row > this->column ? this->column : this->row;
    INT *pcol = new INT[this->nnz];
    INT *prow = new INT[this->row + 1];

    for ( INT j = 0; j < this->diag.size(); j++ )
        pcol[j] = j;

    for ( INT j = 0; j < this->row + 1; j++ )
        prow[j] = j;

    std::vector<INT> rowshift_tmp(this->row + 1);
    std::vector<INT> colindex_tmp(this->nnz);
    std::vector<DBL> values_tmp(this->nnz);

    rowshift_tmp.assign(prow, prow + this->row);
    colindex_tmp.assign(pcol, pcol + this->nnz);
    values_tmp.assign(this->nnz, 0);

    this->rowshift.operator=(rowshift_tmp);
    this->colindex.operator=(colindex_tmp);
    this->values.operator=(values_tmp);

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
        for ( int j = 0; j < this->diag.size(); j++ )
            this->values[j] = 1.0;
        for ( int j = 0; j < diag.size(); j++ )
            this->values[this->diag[j]] = 1 + a;
    } else {
        for ( INT j = 0; j < this->diag.size(); j++ )
            this->values[this->diag[j]] = a + this->values[this->diag[j]];
    }
}

/**
 * if *this 's column dimension does not match "vec" 's dimension, throw an
 * exception.
 */
//! vec_b = *this * vec_x
VEC MAT::MultVec(const VEC vec) const {
    VEC vec_;
    try {
        if ( this->column != vec.GetSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        vec_.SetSize(0);
        return vec_;
    }

    INT begin, end;
    vec_.SetSize(this->row);
    if ( this->values.size() == 0 ) {
        for ( INT j = 0; j < this->row; j++ ) {
            begin = this->rowshift[j];
            end = this->rowshift[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec_[j] += vec[this->colindex[k]];
            }
        }
    } else {
        for ( INT j = 0; j < this->row; j++ ) {
            begin = this->rowshift[j];
            end = this->rowshift[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec_[j] += this->values[k] * vec[this->colindex[k]];
            }
        }
    }

    return vec_;
}

//! transpose *this
void MAT::Transpose() {
    MAT tmp;
    tmp.row = this->column;
    tmp.column = this->row;
    tmp.nnz = this->nnz;

    tmp.rowshift.resize(tmp.row + 1);
    tmp.colindex.resize(tmp.nnz);
    tmp.values.resize(tmp.nnz);
    tmp.diag.resize(tmp.row > tmp.column ? tmp.column : tmp.row);

    INT begin, end;
    INT count = 0;
    tmp.rowshift[0] = 0;

    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < tmp.row; j++ ) {
            for ( INT k = 0; k < this->row; k++ ) {
                begin = this->rowshift[k];
                end = this->rowshift[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex[l] == j ) {
                        tmp.rowshift[j + 1]++;
                        tmp.colindex[count] = k;
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
                begin = this->rowshift[k];
                end = this->rowshift[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex[l] == j ) {
                        tmp.rowshift[j + 1]++;
                        tmp.colindex[count] = k;
                        count++;
                        break;
                    }
                }
            }
        }
    }

    for ( INT j = 0; j < tmp.row; j++ )
        tmp.rowshift[j + 1] += tmp.rowshift[j];

    count = 0;
    for ( INT j = 0; j < tmp.row; j++ ) {
        begin = tmp.rowshift[j];
        end = tmp.rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( tmp.colindex[k] == j ) {
                tmp.diag[count] = k;
                count++;
            }
        }
    }

    this->operator=(tmp);
}

/**
 * if "this->row" 's dimension is not equal to "vec1" 's or "this->column" 's
 * dimension is not equal to "vec2" 's . throw an exception.
 */
//! vec3 = vec2 + transpose(*this) * vec1
VEC MAT::MultTransposeAdd(const VEC vec1, const VEC vec2)
const {
    VEC vec;
    try {
        if ( this->row != vec1.GetSize() || this->column != vec2.GetSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        vec.SetSize(0);
        return vec;
    }

    vec.operator=(vec2);

    MAT tmp;
    tmp.row = this->column;
    tmp.column = this->row;
    tmp.nnz = this->nnz;

    tmp.rowshift.resize(tmp.row + 1);
    tmp.colindex.resize(tmp.nnz);
    tmp.values.resize(tmp.nnz);

    INT begin, end;
    INT count = 0;
    tmp.rowshift[0] = 0;

    if ( this->values.size() != 0 ) {
        for ( INT j = 0; j < tmp.row; j++ ) {
            for ( INT k = 0; k < this->row; k++ ) {
                begin = this->rowshift[k];
                end = this->rowshift[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex[l] == j ) {
                        tmp.rowshift[j + 1]++;
                        tmp.colindex[count] = k;
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
                begin = this->rowshift[k];
                end = this->rowshift[k + 1];
                for ( INT l = begin; l < end; l++ ) {
                    if ( this->colindex[l] == j ) {
                        tmp.rowshift[j + 1]++;
                        tmp.colindex[count] = k;
                        count++;
                        break;
                    }
                }
            }
        }
    }

    for ( INT j = 0; j < tmp.row; j++ )
        tmp.rowshift[j + 1] += tmp.rowshift[j];

    if ( this->values.size()) {
        for ( INT j = 0; j < tmp.row; j++ ) {
            begin = tmp.rowshift[j];
            end = tmp.rowshift[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec[j] += vec1[this->colindex[k]] * tmp.values[k];
            }
        }
    } else {
        for ( INT j = 0; j < tmp.row; j++ ) {
            begin = tmp.rowshift[j];
            end = tmp.rowshift[j + 1];
            for ( INT k = begin; k < end; k++ ) {
                vec[j] += vec1[this->colindex[k]];
            }
        }
    }

    return vec;
}

/**
 * if these matrices both 's dimensions do not match, throw an exception.
 */
//! *this = a * *this + b * mat
void MAT::Add(const DBL a, const DBL b, const MAT mat) {
    try {
        if ( mat.row != this->row || mat.column != this->column ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        return;
    }

    try {
        if ( this->row == 0 || this->column == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        return;
    }

    if ( this->nnz == 0 )
        this->operator=(mat);

    if ( mat.nnz == 0 )
        return;

    MAT tmp1, tmp2;
    tmp1.row = this->row;
    tmp1.column = this->column;
    tmp1.rowshift.resize(this->row + 1);
    tmp1.colindex.resize(this->nnz + mat.nnz);
    tmp1.values.resize(this->nnz + mat.nnz);
    tmp1.diag.resize(this->row > this->column ? this->column : this->row);

    INT begin1, end1, begin2, end2;
    INT count;

    count = 0;
    for ( int j = 0; j < this->row; j++ ) {
        begin1 = this->rowshift[j];
        end1 = this->rowshift[j + 1];
        begin2 = mat.rowshift[j];
        end2 = mat.rowshift[j + 1];
        if ( this->values.size() != 0 && mat.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colindex[count] = this->colindex[k];
                tmp1.values[count] = a * this->values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colindex[count] = mat.colindex[k];
                tmp1.values[count] = b * mat.values[k];
                count++;
            }
        }
        if ( this->values.size() == 0 && mat.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colindex[count] = this->colindex[k];
                tmp1.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colindex[count] = mat.colindex[k];
                tmp1.values[count] = b * mat.values[k];
                count++;
            }
        }
        if ( this->values.size() != 0 && mat.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colindex[count] = this->colindex[k];
                tmp1.values[count] = a * this->values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colindex[count] = mat.colindex[k];
                tmp1.values[count] = b;
                count++;
            }
        }
        if ( this->values.size() == 0 && mat.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colindex[count] = this->colindex[k];
                tmp1.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colindex[count] = mat.colindex[k];
                tmp1.values[count] = b;
                count++;
            }
        }
    }

    tmp1.rowshift[0] = 0;
    for ( int j = 0; j < this->row; j++ )
        tmp1.rowshift[j + 1] = this->rowshift[j + 1] + mat.rowshift[j + 1];

    INT l;
    INT begin, end, index;
    DBL data;
    for ( INT j = 0; j < this->row; j++ ) {
        begin = tmp1.rowshift[j];
        end = tmp1.rowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmp1.colindex[k];
            data = tmp1.values[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmp1.colindex[l] ) {
                    tmp1.colindex[l + 1] = tmp1.colindex[l];
                    tmp1.values[l + 1] = tmp1.values[l];
                } else {
                    break;
                }
            }
            tmp1.colindex[l + 1] = index;
            tmp1.values[l + 1] = data;
        }
    }

    tmp2.row = tmp1.row;
    tmp2.column = tmp1.column;
    tmp2.nnz = 0;
    tmp2.rowshift.resize(tmp2.row + 1);
    tmp2.rowshift[0] = 0;

    INT mem;
    for ( INT j = 0; j < this->row; j++ ) {
        begin = tmp1.rowshift[j];
        end = tmp1.rowshift[j + 1];
        if ( begin == end - 1 ) {
            tmp2.nnz++;
            tmp2.rowshift[j + 1] += 1;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp1.colindex[k] < tmp1.colindex[k + 1] ) {
                tmp2.nnz++;
                tmp2.rowshift[j + 1] += 1;
                mem = tmp1.colindex[k];
            }
        }
        if ( mem < tmp1.colindex[end - 1] ) {
            tmp2.nnz++;
            tmp2.rowshift[j + 1] += 1;
        }
    }

    for ( int j = 1; j < this->row + 1; j++ )
        tmp2.rowshift[j] += tmp2.rowshift[j - 1];

    tmp2.colindex.resize(tmp2.nnz);
    tmp2.values.resize(tmp2.nnz);
    tmp2.diag.resize(this->row > this->column ? this->column : this->row);

    count = 0;
    for ( INT j = 0; j < tmp1.row; j++ ) {
        begin = tmp1.rowshift[j];
        end = tmp1.rowshift[j + 1];
        if ( begin == end - 1 ) {
            tmp2.colindex[count] = j;
            tmp2.values[count] = tmp1.values[begin];
            count++;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp1.colindex[k] < tmp1.colindex[k + 1] ) {
                tmp2.colindex[count] = tmp1.colindex[k];
                tmp2.values[count] += tmp1.values[k];
                count++;
            }
            if ( tmp1.colindex[k] == tmp1.colindex[k + 1] ) {
                tmp2.colindex[count] = tmp1.colindex[k];
                tmp2.values[count] += tmp1.values[k];
            }
        }

        tmp2.colindex[count] = tmp1.colindex[end - 1];
        tmp2.values[count] += tmp1.values[end - 1];
        count++;
    }

    //! compute this->diag
    count = 0;

    for ( INT j = 0; j < tmp2.row; j++ ) {
        begin = tmp2.rowshift[j];
        end = tmp2.rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( tmp2.colindex[k] == j ) {
                tmp2.diag[count] = k;
                count++;
            }
        }
    }

    this->operator=(tmp2);
}

/**
 * if these matrices both 's dimensions do not match, throw an exception.
 */
//! mat3 = a * mat1 + b * mat2
MAT Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2) {
    MAT mat;
    try {
        if ( mat2.row != mat1.row || mat2.column != mat1.column ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.column = 0;
        mat.nnz = 0;
        mat.rowshift.resize(0);
        mat.colindex.resize(0);
        mat.values.resize(0);
        mat.diag.resize(0);
        return mat;
    }

    try {
        if ( mat1.row == 0 || mat1.column == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.column = 0;
        mat.nnz = 0;
        mat.rowshift.resize(0);
        mat.colindex.resize(0);
        mat.values.resize(0);
        mat.diag.resize(0);
        return mat;
    }

    if ( mat1.nnz == 0 ) {
        mat.row = mat2.row;
        mat.column = mat2.column;
        mat.nnz = mat2.nnz;
        mat.rowshift.operator=(mat2.rowshift);
        mat.colindex.operator=(mat2.colindex);
        mat.diag.operator=(mat2.diag);
        for ( int j = 0; j < mat2.nnz; j++ )
            mat.values[j] = b * mat2.values[j];

        return mat;
    }

    if ( mat2.nnz == 0 ) {
        mat.row = mat1.row;
        mat.column = mat1.column;
        mat.nnz = mat1.nnz;
        mat.rowshift.operator=(mat1.rowshift);
        mat.colindex.operator=(mat1.colindex);
        mat.diag.operator=(mat1.diag);
        for ( int j = 0; j < mat1.nnz; j++ )
            mat.values[j] = a * mat1.values[j];

        return mat;
    }


    MAT tmp;
    tmp.row = mat1.row;
    tmp.column = mat1.column;
    tmp.rowshift.resize(mat1.row + 1);
    tmp.colindex.resize(mat1.nnz + mat2.nnz);
    tmp.values.resize(mat1.nnz + mat2.nnz);
    tmp.diag.resize(mat1.row > mat1.column ? mat1.column : mat1.row);

    INT begin1, end1, begin2, end2;
    INT count;

    count = 0;
    for ( int j = 0; j < mat1.row; j++ ) {
        begin1 = mat1.rowshift[j];
        end1 = mat1.rowshift[j + 1];
        begin2 = mat2.rowshift[j];
        end2 = mat2.rowshift[j + 1];
        if ( mat1.values.size() != 0 && mat2.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colindex[count] = mat1.colindex[k];
                tmp.values[count] = a * mat1.values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colindex[count] = mat2.colindex[k];
                tmp.values[count] = b * mat2.values[k];
                count++;
            }
        }
        if ( mat1.values.size() == 0 && mat2.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colindex[count] = mat1.colindex[k];
                tmp.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colindex[count] = mat2.colindex[k];
                tmp.values[count] = b * mat2.values[k];
                count++;
            }
        }
        if ( mat1.values.size() != 0 && mat2.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colindex[count] = mat1.colindex[k];
                tmp.values[count] = a * mat1.values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colindex[count] = mat2.colindex[k];
                tmp.values[count] = b;
                count++;
            }
        }
        if ( mat1.values.size() == 0 && mat2.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colindex[count] = mat1.colindex[k];
                tmp.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colindex[count] = mat2.colindex[k];
                tmp.values[count] = b;
                count++;
            }
        }
    }

    tmp.rowshift[0] = 0;
    for ( int j = 0; j < mat1.row; j++ )
        tmp.rowshift[j + 1] = mat1.rowshift[j + 1] + mat2.rowshift[j + 1];

    INT l;
    INT begin, end, index;
    DBL data;
    for ( INT j = 0; j < mat1.row; j++ ) {
        begin = tmp.rowshift[j];
        end = tmp.rowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmp.colindex[k];
            data = tmp.values[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmp.colindex[l] ) {
                    tmp.colindex[l + 1] = tmp.colindex[l];
                    tmp.values[l + 1] = tmp.values[l];
                } else {
                    break;
                }
            }
            tmp.colindex[l + 1] = index;
            tmp.values[l + 1] = data;
        }
    }

    mat.row = tmp.row;
    mat.column = tmp.column;
    mat.nnz = 0;
    mat.rowshift.resize(mat.row + 1);
    mat.rowshift[0] = 0;

    INT mem;
    for ( INT j = 0; j < mat1.row; j++ ) {
        begin = tmp.rowshift[j];
        end = tmp.rowshift[j + 1];
        if ( begin == end - 1 ) {
            mat.nnz++;
            mat.rowshift[j + 1] += 1;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp.colindex[k] < tmp.colindex[k + 1] ) {
                mat.nnz++;
                mat.rowshift[j + 1] += 1;
                mem = tmp.colindex[k];
            }
        }
        if ( mem < tmp.colindex[end - 1] ) {
            mat.nnz++;
            mat.rowshift[j + 1] += 1;
        }
    }

    for ( int j = 1; j < mat1.row + 1; j++ )
        mat.rowshift[j] += mat.rowshift[j - 1];

    mat.colindex.resize(mat.nnz);
    mat.values.resize(mat.nnz);
    mat.diag.resize(mat1.row > mat1.column ? mat1.column : mat1.row);

    count = 0;
    for ( INT j = 0; j < tmp.row; j++ ) {
        begin = tmp.rowshift[j];
        end = tmp.rowshift[j + 1];
        if ( begin == end - 1 ) {
            mat.colindex[count] = j;
            mat.values[count] = tmp.values[begin];
            count++;
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmp.colindex[k] < tmp.colindex[k + 1] ) {
                mat.colindex[count] = tmp.colindex[k];
                mat.values[count] += tmp.values[k];
                count++;
            }
            if ( tmp.colindex[k] == tmp.colindex[k + 1] ) {
                mat.colindex[count] = tmp.colindex[k];
                mat.values[count] += tmp.values[k];
            }
        }

        mat.colindex[count] = tmp.colindex[end - 1];
        mat.values[count] += tmp.values[end - 1];
        count++;
    }

    //! compute this->diag
    count = 0;

    for ( INT j = 0; j < mat.row; j++ ) {
        begin = mat.rowshift[j];
        end = mat.rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( mat.colindex[k] == j ) {
                mat.diag[count] = k;
                count++;
            }
        }
    }

    return mat;
}

/**
 * if matl.column is not equal to matr.row, throw an exception.
 */
//! mat3 = mat1 * mat2
MAT Mult2(const MAT matl, const MAT matr) {
    MAT mat;
    try {
        if ( matl.column != matr.row ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.column = 0;
        mat.nnz = 0;
        mat.rowshift.resize(0);
        mat.diag.resize(0);
        mat.rowshift.resize(0);
        mat.colindex.resize(0);
        return mat;
    }

    INT l, count;
    INT *tmp = new INT[matr.column];

    mat.row = matl.row;
    mat.column = matr.column;
    mat.rowshift.resize(mat.row + 1);

    for ( INT i = 0; i < matr.column; i++ )
        tmp[i] = -1;

    for ( INT i = 0; i < mat.row; i++ ) {
        count = 0;
        for ( INT k = matl.rowshift[i]; k < matl.rowshift[i + 1]; k++ ) {
            for ( INT j = matr.rowshift[matl.colindex[k]];
                  j < matr.rowshift[matl.colindex[k] + 1]; j++ ) {
                for ( l = 0; l < count; l++ ) {
                    if ( tmp[l] == matr.colindex[j] )
                        break;
                }
                if ( l == count ) {
                    tmp[count] = matr.colindex[j];
                    count++;
                }
            }
        }
        mat.rowshift[i + 1] = count;
        for ( INT j = 0; j < count; j++ )
            tmp[j] = -1;
    }

    for ( INT i = 0; i < mat.row; i++ )
        mat.rowshift[i + 1] += mat.rowshift[i];

    INT count_tmp;

    mat.colindex.resize(mat.rowshift[mat.row]);

    for ( INT i = 0; i < mat.row; i++ ) {
        count_tmp = 0;
        count = mat.rowshift[i];
        for ( INT k = matl.rowshift[i]; k < matl.rowshift[i + 1]; k++ ) {
            for ( INT j = matr.rowshift[matl.colindex[k]];
                  j < matr.rowshift[matl.colindex[k] + 1]; j++ ) {
                for ( l = 0; l < count_tmp; l++ ) {
                    if ( tmp[l] == matr.colindex[j] )
                        break;
                }

                if ( l == count_tmp ) {
                    mat.colindex[count] = matr.colindex[j];
                    tmp[count_tmp] = matr.colindex[j];
                    count++;
                    count_tmp++;
                }
            }
        }

        for ( INT j = 0; j < count_tmp; j++ )
            tmp[j] = -1;
    }

    delete[] tmp;

    mat.values.resize(mat.rowshift[mat.row]);

    if ( matl.nnz != 0 && matr.nnz != 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowshift[i]; j < mat.rowshift[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowshift[i]; k < matl.rowshift[i + 1]; k++ ) {
                    for ( l = matr.rowshift[matl.colindex[k]];
                          l < matr.rowshift[matl.colindex[k] + 1]; l++ ) {
                        if ( matr.colindex[l] == mat.colindex[j] )
                            mat.values[j] += matl.values[k] * matr.values[l];
                    }
                }
            }
        }
    }
    if ( matl.nnz != 0 && matr.nnz == 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowshift[i]; j < mat.rowshift[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowshift[i]; k < matl.rowshift[i + 1]; k++ ) {
                    for ( l = matr.rowshift[matl.colindex[k]];
                          l < matr.rowshift[matl.colindex[k] + 1]; l++ ) {
                        if ( matr.colindex[l] == mat.colindex[j] )
                            mat.values[j] += matl.values[k];
                    }
                }
            }
        }
    }
    if ( matl.nnz == 0 && matr.nnz != 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowshift[i]; j < mat.rowshift[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowshift[i]; k < matl.rowshift[i + 1]; k++ ) {
                    for ( l = matr.rowshift[matl.colindex[k]];
                          l < matr.rowshift[matl.colindex[k] + 1]; l++ ) {
                        if ( matr.colindex[l] == mat.colindex[j] )
                            mat.values[j] += matr.values[l];
                    }
                }
            }
        }
    }
    if ( matl.nnz == 0 && matr.nnz == 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowshift[i]; j < mat.rowshift[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowshift[i]; k < matl.rowshift[i + 1]; k++ ) {
                    for ( l = matr.rowshift[matl.colindex[k]];
                          l < matr.rowshift[matl.colindex[k] + 1]; l++ ) {
                        if ( matr.colindex[l] == mat.colindex[j] )
                            mat.values[j] += 1;
                    }
                }
            }
        }
    }

    mat.nnz = mat.rowshift[mat.row] - mat.rowshift[0];

    return mat;
}

//! *this = *this * mat
void MAT::MultLeft(const MAT mat) {
    this->operator=(Mult2(*this, mat));
}

//! *this = mat * *this
void MAT::MultRight(const MAT mat) {
    MAT tmp = Mult2(mat, *this);
    this->operator=(tmp);

}

/**
 * if "row == 0" or "column == 0" or "nnz == 0" happens, set the returned values
 * by default constructor. if these data are not CSRx, set the returned values by
 * default constructor and throw an exception.
 */
//! convert the data CSR format to CSRx format
MAT ConvertCSR(const INT row, const INT column, const INT nnz,
               const std::vector<DBL> values,
               const std::vector<INT> rowshift,
               const std::vector<INT> colindex) {

    MAT mat;
    if ( row == 0 || column == 0 || nnz == 0 ) {
        mat.row = 0;
        mat.column = 0;
        mat.nnz = 0;
        mat.diag.resize(0);
        mat.rowshift.resize(0);
        mat.values.resize(0);
        mat.colindex.resize(0);
        return mat;
    }

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowshift.size() - 1 )
        goto Return;

    if ( row <= 0 || column <= 0 )
        goto Return;

    if ( nnz != colindex.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowshift[rowshift.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowshift[j] > rowshift[j + 1] )
            goto Return;
    }

    if ( rowshift[0] < 0 || rowshift[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colindex[begin] || colindex[begin] >= column )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colindex[k] || colindex[k] >= column ) {
                    goto Return;
                }
            }
        }
    }

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.column = 0;
        mat.nnz = 0;
        mat.rowshift.resize(0);
        mat.colindex.resize(0);
        mat.diag.resize(0);
        mat.values.resize(0);

        return mat;
    }

    INT index;
    DBL data;
    std::vector<INT> tmprowshift, tmpcolindex;
    std::vector<DBL> tmpvalues;
    tmprowshift.operator=(rowshift);
    tmpcolindex.operator=(colindex);
    tmpvalues.operator=(values);

    //sort
    INT l;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolindex[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolindex[l] ) {
                    tmpcolindex[l + 1] = tmpcolindex[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolindex[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    mat.row = row;
    mat.column = column;
    mat.nnz = 0;
    mat.rowshift.resize(row + 1);
    mat.rowshift[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            mat.nnz++;
            mat.rowshift[j + 1] = mat.rowshift[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                mat.nnz++;
                mat.rowshift[j + 1] = mat.rowshift[j] + 1;
            } else {
                mat.nnz += 2;
                mat.rowshift[j + 1] = mat.rowshift[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k] < tmpcolindex[k + 1] ) {
                mat.nnz++;
                if ( tmpcolindex[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] ) {
            mat.nnz++;
            if ( tmpcolindex[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
            mat.nnz++;
            mat.rowshift[j + 1] = mat.rowshift[j] + end - begin + 1;
        } else {
            mat.rowshift[j + 1] = mat.rowshift[j] + end - begin;
            mark = 0;
        }
    }

    mat.colindex.resize(mat.nnz);
    mat.values.resize(mat.nnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowshift[j];
        end = tmprowshift[j + 1];
        if ( begin == end ) {
            mat.colindex[count] = j;
            mat.values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolindex[begin] == j ) {
                mat.colindex[count] = j;
                mat.values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolindex[begin] > j ) {
                    mat.colindex[count] = j;
                    mat.values[count] = 0.0;
                    count++;
                    mat.colindex[count] = tmpcolindex[begin];
                    mat.values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolindex[begin] < j ) {
                    mat.colindex[count] = tmpcolindex[begin];
                    mat.values[count] = tmpvalues[begin];
                    count++;
                    mat.colindex[count] = j;
                    mat.values[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( tmpcolindex[begin + 1] < j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
                mat.colindex[count] = tmpcolindex[begin];
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colindex[count] = tmpcolindex[begin + 1];
                mat.values[count] = tmpvalues[begin + 1];
                count++;
                mat.colindex[count] = j;
                mat.values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                mat.colindex[count] = tmpcolindex[begin];
                mat.values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                mat.colindex[count] = j;
                mat.values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[begin] < j && tmpcolindex[begin + 1] == j ) {
                mat.colindex[count] = tmpcolindex[begin];
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colindex[count] = j;
                mat.values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] == j ) {
                mat.colindex[count] = j;
                mat.values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] == j && tmpcolindex[begin + 1] > j ) {
                mat.colindex[count] = j;
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colindex[count] = tmpcolindex[begin + 1];
                mat.values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] == tmpcolindex[begin + 1] ) {
                mat.colindex[count] = j;
                mat.values[count] = 0.0;
                count++;
                mat.colindex[count] = tmpcolindex[begin];
                mat.values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolindex[begin] > j &&
                 tmpcolindex[begin] < tmpcolindex[begin + 1] ) {
                mat.colindex[count] = j;
                mat.values[count] = 0.0;
                count++;
                mat.colindex[count] = tmpcolindex[begin];
                mat.values[count] = tmpvalues[begin];
                count++;
                mat.colindex[count] = tmpcolindex[begin + 1];
                mat.values[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolindex[k + 1] < j && tmpcolindex[k] < tmpcolindex[k + 1] ) {
                mat.colindex[count] = tmpcolindex[k];
                mat.values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                mat.colindex[count] = tmpcolindex[k];
                mat.values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] == j ) {
                mat.colindex[count] = tmpcolindex[k];
                mat.values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] == j ) {
                mat.colindex[count] = j;
                mat.values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] < j && tmpcolindex[k + 1] > j ) {
                mat.colindex[count] = tmpcolindex[k];
                mat.values[count] += tmpvalues[k];
                count++;
                mat.colindex[count] = j;
                mat.values[count] = 0.0;
                count++;
            }
            if ( tmpcolindex[k] == j && tmpcolindex[k + 1] > j ) {
                mat.colindex[count] = j;
                mat.values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k] == tmpcolindex[k + 1] ) {
                mat.colindex[count] = tmpcolindex[k];
                mat.values[count] += tmpvalues[k];
            }
            if ( tmpcolindex[k] > j && tmpcolindex[k + 1] > tmpcolindex[k] ) {
                mat.colindex[count] = tmpcolindex[k];
                mat.values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            mat.colindex[count] = tmpcolindex[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
            mat.colindex[count] = j;
            mat.values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] < j ) {
            mat.colindex[count] = tmpcolindex[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
            mat.colindex[count] = j;
            mat.values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] < tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            mat.colindex[count] = j;
            mat.values[count] = 0.0;
            count++;
        }
        if ( tmpcolindex[end - 2] == tmpcolindex[end - 1] &&
             tmpcolindex[end - 1] == j ) {
            mat.colindex[count] = j;
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] < j && tmpcolindex[end - 1] > j ) {
            mat.colindex[count] = tmpcolindex[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 1] > tmpcolindex[end - 2] ) {
            mat.colindex[count] = tmpcolindex[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] == j && tmpcolindex[end - 1] > j ) {
            mat.colindex[count] = tmpcolindex[end - 1];
            mat.values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolindex[end - 2] > j &&
             tmpcolindex[end - 2] == tmpcolindex[end - 1] ) {
            mat.colindex[count] = tmpcolindex[end - 1];
            mat.values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    mat.diag.resize(row > column ? column : row);
    //! compute mat.diag
    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = mat.rowshift[j];
        end = mat.rowshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( mat.colindex[k] == j ) {
                mat.diag[count] = k;
                count++;
            }
        }
    }
}