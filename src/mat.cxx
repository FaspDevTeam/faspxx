/*! \file mat.cxx
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
 * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, col, nnz, values, rowPtr, colInd, diag to *this
MAT::MAT(const INT row, const INT col, const INT nnz,
         const std::vector<DBL> values, const std::vector<INT> rowPtr,
         const std::vector<INT> colInd, const std::vector<INT> diagPtr) {

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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    // basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if (((row > col) ? col : row) != diagPtr.size())
        goto Return;

    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    // simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] >= rowPtr[j + 1] ) {
            goto Return;
        }
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            goto Return;

        if ( end == begin + 1 ) {
            if ( colInd[begin] != j )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; k++ ) {
                if ( colInd[k] >= colInd[k + 1] )
                    goto Return;
            }
            if ( 0 > colInd[begin] )
                goto Return;

            if ( colInd[end - 1] >= col )
                goto Return;
        }
    }

    // exam diagPtr and colInd
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( colInd[k] == j ) {
                if ( diagPtr[count] != k )
                    goto Return;
                else
                    count++;
            }
        }
    }
    if ( count != diagPtr.size())
        goto Return;

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);

        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values.operator=(values);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diagPtr);
}

/**
 * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 * attention : all the CSRx data 's values are one
 */
//! assign row, col, nnz, rowPtr, colInd, diagPtr to *this
MAT::MAT(const INT row, const INT col, const INT nnz,
         const std::vector<INT> rowPtr, const std::vector<INT> colInd,
         const std::vector<INT> diagPtr) {

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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    // basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if (((row > col) ? col : row) != diagPtr.size())
        goto Return;


    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    // simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] >= rowPtr[j + 1] ) {
            goto Return;
        }
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            goto Return;

        if ( end == begin + 1 ) {
            if ( colInd[begin] != j )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; k++ ) {
                if ( colInd[k] >= colInd[k + 1] )
                    goto Return;
            }
            if ( 0 > colInd[begin] )
                goto Return;

            if ( colInd[end - 1] >= col )
                goto Return;
        }
    }

    // exam diagPtr and colInd
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( colInd[k] == j ) {
                if ( diagPtr[count] != k )
                    goto Return;
                else
                    count++;
            }
        }
    }
    if ( count != diagPtr.size())
        goto Return;

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);

        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values.resize(0);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diagPtr);
}

/**
 * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, col, nnz, values, rowPtr, colInd to *this
MAT::MAT(const INT row, const INT col, const INT nnz,
         const std::vector<DBL> values, const std::vector<INT> rowPtr,
         const std::vector<INT> colInd) {

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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    // basic examinations
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    // simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] > rowPtr[j + 1] )
            goto Return;
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colInd[begin] || colInd[begin] >= col )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colInd[k] || colInd[k] >= col ) {
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
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);

        return;
    }

    INT index;
    DBL data;
    std::vector<INT> tmprowPtr, tmpcolInd;
    std::vector<DBL> tmpvalues;
    tmprowPtr.operator=(rowPtr);
    tmpcolInd.operator=(colInd);
    tmpvalues.operator=(values);

    // sort
    INT l;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolInd[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolInd[l] ) {
                    tmpcolInd[l + 1] = tmpcolInd[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolInd[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->col = col;
    this->nnz = 0;
    this->rowPtr.resize(row + 1);
    this->rowPtr[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            this->nnz++;
            this->rowPtr[j + 1] = this->rowPtr[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                this->nnz++;
                this->rowPtr[j + 1] = this->rowPtr[j] + 1;
            } else {
                this->nnz += 2;
                this->rowPtr[j + 1] = this->rowPtr[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k] < tmpcolInd[k + 1] ) {
                this->nnz++;
                if ( tmpcolInd[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] ) {
            this->nnz++;
            if ( tmpcolInd[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
            this->nnz++;
            this->rowPtr[j + 1] = this->rowPtr[j] + end - begin + 1;
        } else {
            this->rowPtr[j + 1] = this->rowPtr[j] + end - begin;
            mark = 0;
        }
    }

    this->colInd.resize(this->nnz);
    this->values.resize(this->nnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolInd[begin] > j ) {
                    this->colInd[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colInd[count] = tmpcolInd[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolInd[begin] < j ) {
                    this->colInd[count] = tmpcolInd[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                    this->colInd[count] = j;
                    this->values[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( tmpcolInd[begin + 1] < j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j && tmpcolInd[begin + 1] == j ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] == j ) {
                this->colInd[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] > j ) {
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k + 1] < j && tmpcolInd[k] < tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] == j ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] == j ) {
                this->colInd[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] > j ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] > j ) {
                this->colInd[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k + 1] > tmpcolInd[k] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            this->colInd[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] < j && tmpcolInd[end - 1] > j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 1] > tmpcolInd[end - 2] ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] == j && tmpcolInd[end - 1] > j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 2] == tmpcolInd[end - 1] ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diagPtr.resize(row > col ? col : row);
    //! compute this->diagPtr
    count = 0;
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

/**
 * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 * attention : all the elements in the CSRx 's values are one
 */
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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] > rowPtr[j + 1] )
            goto Return;
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colInd[begin] || colInd[begin] >= col )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colInd[k] || colInd[k] >= col ) {
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
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);

        return;
    }

    INT index, l;
    DBL data;
    std::vector<INT> tmprowPtr, tmpcolInd;
    std::vector<DBL> tmpvalues;
    tmprowPtr.operator=(rowPtr);
    tmpcolInd.operator=(colInd);
    tmpvalues.assign(rowPtr[rowPtr.size() - 1], 1);
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolInd[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolInd[l] ) {
                    tmpcolInd[l + 1] = tmpcolInd[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolInd[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->col = col;
    this->nnz = 0;
    this->rowPtr.resize(row + 1);
    this->rowPtr[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            this->nnz++;
            this->rowPtr[j + 1] = this->rowPtr[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                this->nnz++;
                this->rowPtr[j + 1] = this->rowPtr[j] + 1;
            } else {
                this->nnz += 2;
                this->rowPtr[j + 1] = this->rowPtr[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k] < tmpcolInd[k + 1] ) {
                this->nnz++;
                if ( tmpcolInd[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] ) {
            this->nnz++;
            if ( tmpcolInd[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
            this->nnz++;
            this->rowPtr[j + 1] = this->rowPtr[j] + end - begin + 1;
        } else {
            this->rowPtr[j + 1] = this->rowPtr[j] + end - begin;
            mark = 0;
        }
    }

    this->colInd.resize(this->nnz);
    this->values.resize(this->nnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolInd[begin] > j ) {
                    this->colInd[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colInd[count] = tmpcolInd[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolInd[begin] < j ) {
                    this->colInd[count] = tmpcolInd[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                    this->colInd[count] = j;
                    this->values[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( tmpcolInd[begin + 1] < j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j && tmpcolInd[begin + 1] == j ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] == j ) {
                this->colInd[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] > j ) {
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k + 1] < j && tmpcolInd[k] < tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] == j ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] == j ) {
                this->colInd[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] > j ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] > j ) {
                this->colInd[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k + 1] > tmpcolInd[k] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            this->colInd[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] < j && tmpcolInd[end - 1] > j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 1] > tmpcolInd[end - 2] ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] == j && tmpcolInd[end - 1] > j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 2] == tmpcolInd[end - 1] ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diagPtr.resize(row > col ? col : row);
    //! compute this->diagPtr
    count = 0;
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

//! assign VEC object to form the diagPtronal matrix
MAT::MAT(VEC &vec_obj) {
    INT size = vec_obj.GetSize();
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
            this->values[j] = vec_obj[j];

        this->diagPtr.resize(size);
        this->diagPtr.assign(array, array + size);

        delete[] array;
        array = nullptr;
    }
}

//! assign vector object to form the diagPtronal matrix
MAT::MAT(const std::vector<DBL> &vector_obj) {
    INT size = vector_obj.size();
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
        this->values.assign(vector_obj.begin(), vector_obj.begin() + size);

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
}
/**
 * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
//! assign row, col, nnz, values, rowPtr, colInd, diagPtr to *this
void MAT::SetValues(const INT row, const INT col, const INT nnz,
                    const std::vector<DBL> values, const std::vector<INT> rowPtr,
                    const std::vector<INT> colInd, const std::vector<INT> diagPtr) {

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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT mark = 0;
    INT count = 0;
    INT begin, end;
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if (((row > col) ? col : row) != diagPtr.size())
        goto Return;

    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] >= rowPtr[j + 1] ) {
            goto Return;
        }
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            goto Return;

        if ( end == begin + 1 ) {
            if ( colInd[begin] != j )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; k++ ) {
                if ( colInd[k] >= colInd[k + 1] )
                    goto Return;
            }
            if ( 0 > colInd[begin] )
                goto Return;

            if ( colInd[end - 1] >= col )
                goto Return;
        }
    }

    //! exam diagPtr and colInd
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( colInd[k] == j ) {
                if ( diagPtr[count] != k )
                    goto Return;
                else
                    count++;
            }
        }
    }
    if ( count != diagPtr.size())
        goto Return;

    mark = 1;

    Return:
    try {
        if ( mark == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_INPUT_FILE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);

        return;
    }

    this->row = row;
    this->col = col;
    this->nnz = nnz;
    this->values.operator=(values);
    this->rowPtr.operator=(rowPtr);
    this->colInd.operator=(colInd);
    this->diagPtr.operator=(diagPtr);
}

/**
 * if "row == 0" or "col ==0 " or "nnz == 0" happens, set *this by default.
 * if these parameters can't form a CSRx data type, throw an exception. or set these
 * parameters to *this.
 */
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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] > rowPtr[j + 1] )
            goto Return;
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colInd[begin] || colInd[begin] >= col )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colInd[k] || colInd[k] >= col ) {
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
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        this->row = 0;
        this->col = 0;
        this->nnz = 0;
        this->rowPtr.resize(0);
        this->colInd.resize(0);
        this->diagPtr.resize(0);
        this->values.resize(0);

        return;
    }

    INT index, l;
    DBL data;
    std::vector<INT> tmprowPtr, tmpcolInd;
    std::vector<DBL> tmpvalues;
    tmprowPtr.operator=(rowPtr);
    tmpcolInd.operator=(colInd);
    tmpvalues.operator=(values);
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolInd[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolInd[l] ) {
                    tmpcolInd[l + 1] = tmpcolInd[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolInd[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
    this->row = row;
    this->col = col;
    this->nnz = 0;
    this->rowPtr.resize(row + 1);
    this->rowPtr[0] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            this->nnz++;
            this->rowPtr[j + 1] = this->rowPtr[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                this->nnz++;
                this->rowPtr[j + 1] = this->rowPtr[j] + 1;
            } else {
                this->nnz += 2;
                this->rowPtr[j + 1] = this->rowPtr[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k] < tmpcolInd[k + 1] ) {
                this->nnz++;
                if ( tmpcolInd[k] == j ) {
                    mark = 1;
                }
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] ) {
            this->nnz++;
            if ( tmpcolInd[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
            this->nnz++;
            this->rowPtr[j + 1] = this->rowPtr[j] + end - begin + 1;
        } else {
            this->rowPtr[j + 1] = this->rowPtr[j] + end - begin;
            mark = 0;
        }
    }

    this->colInd.resize(this->nnz);
    this->values.resize(this->nnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( begin == end ) {
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( tmpcolInd[begin] == j ) {
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
            } else {
                if ( tmpcolInd[begin] > j ) {
                    this->colInd[count] = j;
                    this->values[count] = 0.0;
                    count++;
                    this->colInd[count] = tmpcolInd[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                }
                if ( tmpcolInd[begin] < j ) {
                    this->colInd[count] = tmpcolInd[begin];
                    this->values[count] = tmpvalues[begin];
                    count++;
                    this->colInd[count] = j;
                    this->values[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( tmpcolInd[begin + 1] < j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[begin] < j && tmpcolInd[begin + 1] == j ) {
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] == j ) {
                this->colInd[count] = j;
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] == j && tmpcolInd[begin + 1] > j ) {
                this->colInd[count] = j;
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] == tmpcolInd[begin + 1] ) {
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] += (tmpvalues[begin] + tmpvalues[begin + 1]);
                count++;
            }
            if ( tmpcolInd[begin] > j &&
                 tmpcolInd[begin] < tmpcolInd[begin + 1] ) {
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
                this->colInd[count] = tmpcolInd[begin];
                this->values[count] = tmpvalues[begin];
                count++;
                this->colInd[count] = tmpcolInd[begin + 1];
                this->values[count] = tmpvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( tmpcolInd[k + 1] < j && tmpcolInd[k] < tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] == j ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] == j ) {
                this->colInd[count] = j;
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] < j && tmpcolInd[k + 1] > j ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
                this->colInd[count] = j;
                this->values[count] = 0.0;
                count++;
            }
            if ( tmpcolInd[k] == j && tmpcolInd[k + 1] > j ) {
                this->colInd[count] = j;
                this->values[count] += tmpvalues[k];
                count++;
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k] == tmpcolInd[k + 1] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
            }
            if ( tmpcolInd[k] > j && tmpcolInd[k + 1] > tmpcolInd[k] ) {
                this->colInd[count] = tmpcolInd[k];
                this->values[count] += tmpvalues[k];
                count++;
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] < j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            this->colInd[count] = j;
            this->values[count] = 0.0;
            count++;
        }
        if ( tmpcolInd[end - 2] == tmpcolInd[end - 1] &&
             tmpcolInd[end - 1] == j ) {
            this->colInd[count] = j;
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] < j && tmpcolInd[end - 1] > j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 1] > tmpcolInd[end - 2] ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] == j && tmpcolInd[end - 1] > j ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] = tmpvalues[end - 1];
            count++;
        }
        if ( tmpcolInd[end - 2] > j &&
             tmpcolInd[end - 2] == tmpcolInd[end - 1] ) {
            this->colInd[count] = tmpcolInd[end - 1];
            this->values[count] += tmpvalues[end - 1];
            count++;
        }
    }

    this->diagPtr.resize(this->row > this->col ? this->col : this->row);
    //! compute this->diagPtr
    count = 0;
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

//! get the row or col number of *this
void MAT::GetSizes(INT &row, INT &col) const {
    row = this->row;
    col = this->col;
}

//! get row size
INT MAT::GetRowSize() const {
    return this->row;
}

//! get col size
INT MAT::GetColSize() const {
    return this->col;
}

//! get this->nnz
INT MAT::GetNNZ() const {
    return this->nnz;
}

/**
 * if "row < 0" or "row > this->row" or "col < 0" or "col >=this->col"
 * happens, throw an exception. In other cases,it is normally dealt.
 */
//! get (*this)[i][j]
DBL MAT::GetElem(const INT row, const INT col) const {
    try {
        if ( row < 0 || row >= this->row || col < 0 || col >= this->col ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;

        return 0;
    }

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
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        vec.resize(0);
        return vec;
    }

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

/**
 * if "col < 0" or "col > this->row" happens,
 * throw an exception. In other cases,it is normally dealt.
 */
//! get the whole jth-col elements in *this into vector object
std::vector<DBL> MAT::GetCol(const INT col)
const {
    std::vector<DBL> vec;
    try {
        if ( col < 0 || col >= this->col ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
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
        if ( col >= this->colInd[this->rowPtr[j]] ) {
            for ( INT k = this->rowPtr[j]; k < this->rowPtr[j + 1]; k++ ) {
                if ( this->colInd[k] == col ) {
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

    std::vector<INT> rowPtr_tmp(this->row + 1);
    std::vector<INT> colInd_tmp(this->nnz);
    std::vector<DBL> values_tmp(this->nnz);

    rowPtr_tmp.assign(prow, prow + this->row);
    colInd_tmp.assign(pcol, pcol + this->nnz);
    values_tmp.assign(this->nnz, 0);

    this->rowPtr.operator=(rowPtr_tmp);
    this->colInd.operator=(colInd_tmp);
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
        for ( int j = 0; j < this->diagPtr.size(); j++ )
            this->values[j] = 1.0;
        for ( int j = 0; j < diagPtr.size(); j++ )
            this->values[this->diagPtr[j]] = 1 + a;
    } else {
        for ( INT j = 0; j < this->diagPtr.size(); j++ )
            this->values[this->diagPtr[j]] = a + this->values[this->diagPtr[j]];
    }
}

/**
 * if *this 's col dimension does not match "vec" 's dimension, throw an
 * exception.
 */
//! vec_b = *this * vec_x
VEC MAT::MultVec(const VEC vec) const {
    VEC vec_;
    vec_.Reserve(this->row);

    INT begin, end;
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

/**
 * if "this->row" 's dimension is not equal to "vec1" 's or "this->col" 's
 * dimension is not equal to "vec2" 's . throw an exception.
 */
//! vec3 = vec2 + transpose(*this) * vec1
VEC MAT::MultTransposeAdd(const VEC vec1, const VEC vec2) const {
    VEC vec;
    vec.operator = (vec2);

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

/**
 * if these matrices both 's dimensions do not match, throw an exception.
 */
//! *this = a * *this + b * mat
void MAT::Add(const DBL a, const DBL b, const MAT mat) {
    try {
        if ( mat.row != this->row || mat.col != this->col ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        return;
    }

    try {
        if ( this->row == 0 || this->col == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
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
        if ( this->values.size() == 0 && mat.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colInd[count] = this->colInd[k];
                tmp1.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colInd[count] = mat.colInd[k];
                tmp1.values[count] = b * mat.values[k];
                count++;
            }
        }
        if ( this->values.size() != 0 && mat.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colInd[count] = this->colInd[k];
                tmp1.values[count] = a * this->values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colInd[count] = mat.colInd[k];
                tmp1.values[count] = b;
                count++;
            }
        }
        if ( this->values.size() == 0 && mat.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp1.colInd[count] = this->colInd[k];
                tmp1.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp1.colInd[count] = mat.colInd[k];
                tmp1.values[count] = b;
                count++;
            }
        }
    }

    tmp1.rowPtr[0] = 0;
    for ( int j = 0; j < this->row; j++ )
        tmp1.rowPtr[j + 1] = this->rowPtr[j + 1] + mat.rowPtr[j + 1];

    INT l;
    INT begin, end, index;
    DBL data;
    for ( INT j = 0; j < this->row; j++ ) {
        begin = tmp1.rowPtr[j];
        end = tmp1.rowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmp1.colInd[k];
            data = tmp1.values[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmp1.colInd[l] ) {
                    tmp1.colInd[l + 1] = tmp1.colInd[l];
                    tmp1.values[l + 1] = tmp1.values[l];
                } else {
                    break;
                }
            }
            tmp1.colInd[l + 1] = index;
            tmp1.values[l + 1] = data;
        }
    }

    tmp2.row = tmp1.row;
    tmp2.col = tmp1.col;
    tmp2.nnz = 0;
    tmp2.rowPtr.resize(tmp2.row + 1);
    tmp2.rowPtr[0] = 0;

    INT mem;
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

/**
 * if these matrices both 's dimensions do not match, throw an exception.
 */
//! mat3 = a * mat1 + b * mat2
MAT Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2) {
    MAT mat;
    try {
        if ( mat2.row != mat1.row || mat2.col != mat1.col ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.col = 0;
        mat.nnz = 0;
        mat.rowPtr.resize(0);
        mat.colInd.resize(0);
        mat.values.resize(0);
        mat.diagPtr.resize(0);
        return mat;
    }

    try {
        if ( mat1.row == 0 || mat1.col == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.col = 0;
        mat.nnz = 0;
        mat.rowPtr.resize(0);
        mat.colInd.resize(0);
        mat.values.resize(0);
        mat.diagPtr.resize(0);
        return mat;
    }

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
        if ( mat1.values.size() == 0 && mat2.values.size() != 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colInd[count] = mat1.colInd[k];
                tmp.values[count] = a;
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colInd[count] = mat2.colInd[k];
                tmp.values[count] = b * mat2.values[k];
                count++;
            }
        }
        if ( mat1.values.size() != 0 && mat2.values.size() == 0 ) {
            for ( int k = begin1; k < end1; k++ ) {
                tmp.colInd[count] = mat1.colInd[k];
                tmp.values[count] = a * mat1.values[k];
                count++;
            }
            for ( int k = begin2; k < end2; k++ ) {
                tmp.colInd[count] = mat2.colInd[k];
                tmp.values[count] = b;
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

    INT l;
    INT begin, end, index;
    DBL data;
    for ( INT j = 0; j < mat1.row; j++ ) {
        begin = tmp.rowPtr[j];
        end = tmp.rowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmp.colInd[k];
            data = tmp.values[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmp.colInd[l] ) {
                    tmp.colInd[l + 1] = tmp.colInd[l];
                    tmp.values[l + 1] = tmp.values[l];
                } else {
                    break;
                }
            }
            tmp.colInd[l + 1] = index;
            tmp.values[l + 1] = data;
        }
    }

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

/**
 * if matl.col is not equal to matr.row, throw an exception.
 */
//! mat3 = mat1 * mat2
MAT Mult2(const MAT matl, const MAT matr) {
    MAT mat;
    try {
        if ( matl.col != matr.row ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.col = 0;
        mat.nnz = 0;
        mat.rowPtr.resize(0);
        mat.diagPtr.resize(0);
        mat.rowPtr.resize(0);
        mat.colInd.resize(0);
        return mat;
    }

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

    mat.values.resize(mat.rowPtr[mat.row]);

    if ( matl.nnz != 0 && matr.nnz != 0 ) {
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
    }
    if ( matl.nnz != 0 && matr.nnz == 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowPtr[i]; j < mat.rowPtr[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; k++ ) {
                    for ( l = matr.rowPtr[matl.colInd[k]];
                          l < matr.rowPtr[matl.colInd[k] + 1]; l++ ) {
                        if ( matr.colInd[l] == mat.colInd[j] )
                            mat.values[j] += matl.values[k];
                    }
                }
            }
        }
    }
    if ( matl.nnz == 0 && matr.nnz != 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowPtr[i]; j < mat.rowPtr[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; k++ ) {
                    for ( l = matr.rowPtr[matl.colInd[k]];
                          l < matr.rowPtr[matl.colInd[k] + 1]; l++ ) {
                        if ( matr.colInd[l] == mat.colInd[j] )
                            mat.values[j] += matr.values[l];
                    }
                }
            }
        }
    }
    if ( matl.nnz == 0 && matr.nnz == 0 ) {
        for ( INT i = 0; i < mat.row; i++ ) {
            for ( INT j = mat.rowPtr[i]; j < mat.rowPtr[i + 1]; j++ ) {
                mat.values[j] = 0;
                for ( INT k = matl.rowPtr[i]; k < matl.rowPtr[i + 1]; k++ ) {
                    for ( l = matr.rowPtr[matl.colInd[k]];
                          l < matr.rowPtr[matl.colInd[k] + 1]; l++ ) {
                        if ( matr.colInd[l] == mat.colInd[j] )
                            mat.values[j] += 1;
                    }
                }
            }
        }
    }

    mat.nnz = mat.rowPtr[mat.row] - mat.rowPtr[0];

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
 * if "row == 0" or "col == 0" or "nnz == 0" happens, set the returned values
 * by default constructor. if these data are not CSRx, set the returned values by
 * default constructor and throw an exception.
 */
//! convert the data CSR format to CSRx format
MAT ConvertCSR(const INT row, const INT col, const INT nnz,
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

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    INT mark = 0;
    //! basic examinations
    if ( row != rowPtr.size() - 1 )
        goto Return;

    if ( row <= 0 || col <= 0 )
        goto Return;

    if ( nnz != colInd.size())
        goto Return;

    if ( nnz != values.size())
        goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] )
        goto Return;

    //! simple examinations
    for ( INT j = 0; j < row; j++ ) {
        if ( rowPtr[j] > rowPtr[j + 1] )
            goto Return;
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz )
        goto Return;

    INT begin, end;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end )
            continue;

        if ( end == begin + 1 ) {
            if ( 0 > colInd[begin] || colInd[begin] >= col )
                goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end; k++ ) {
                if ( 0 > colInd[k] || colInd[k] >= col ) {
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
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : " << ex.getFile() << " at Line " << ex.getLine()
                  << std::endl;
        mat.row = 0;
        mat.col = 0;
        mat.nnz = 0;
        mat.rowPtr.resize(0);
        mat.colInd.resize(0);
        mat.diagPtr.resize(0);
        mat.values.resize(0);

        return mat;
    }

    INT index;
    DBL data;
    std::vector<INT> tmprowPtr, tmpcolInd;
    std::vector<DBL> tmpvalues;
    tmprowPtr.operator=(rowPtr);
    tmpcolInd.operator=(colInd);
    tmpvalues.operator=(values);

    //sort
    INT l;
    for ( INT j = 0; j < row; j++ ) {
        begin = tmprowPtr[j];
        end = tmprowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = tmpcolInd[k];
            data = tmpvalues[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < tmpcolInd[l] ) {
                    tmpcolInd[l + 1] = tmpcolInd[l];
                    tmpvalues[l + 1] = tmpvalues[l];
                } else {
                    break;
                }
            }
            tmpcolInd[l + 1] = index;
            tmpvalues[l + 1] = data;
        }
    }

    mark = 0;
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
                    mark = 1;
                }
            }
        }
        if ( tmpcolInd[end - 2] < tmpcolInd[end - 1] ) {
            mat.nnz++;
            if ( tmpcolInd[end - 1] == j ) {
                mark = 1;
            }
        }

        if ( mark != 1 ) {
            mat.nnz++;
            mat.rowPtr[j + 1] = mat.rowPtr[j] + end - begin + 1;
        } else {
            mat.rowPtr[j + 1] = mat.rowPtr[j] + end - begin;
            mark = 0;
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
            mat.values[count] = 0.0;
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
            mat.values[count] = tmpvalues[end - 1];
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
}