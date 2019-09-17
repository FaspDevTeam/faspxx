/*! \file vec.cxx
 *  \brief Source file for some FASP++ functions
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights resized.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "common.hxx"
#include "error.hxx"

//! check whether the input data are CSRx
INT CheckCSRx(const INT row, const INT column, const INT nnz,
              const std::vector<DBL> values,
              const std::vector<INT> rowshift,
              const std::vector<INT> colindex,
              const std::vector<INT> diag) {
    if ( row == 0 || column == 0 || nnz == 0 )
        return 1;

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

        return FaspErrorCode::ERROR_INPUT_FILE;
    }

    return FaspErrorCode::SUCCESS;
}

//! check whether the input data are CSR
INT CheckCSR(const INT row, const INT column, const INT nnz,
             const std::vector<DBL> values,
             const std::vector<INT> rowshift,
             const std::vector<INT> colindex) {
    if ( row == 0 || column == 0 || nnz == 0 )
        return 1;

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

        return FaspErrorCode::ERROR_INPUT_FILE;
    }

    return FaspErrorCode::SUCCESS;
}