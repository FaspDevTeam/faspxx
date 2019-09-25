/**
 * a source file about some auxiliary functions
 */

#include "auxmat.hxx"

//! check GetElem function
FaspErrorCode MatCheck_CrossOver(const MAT mat, const INT row, const INT col) {
    try {
        if ( row < 0 || row >= mat.GetRowSize() || col < 0 ||
             col >= mat.GetColSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_MAT_SIZE;
    }
    return FaspErrorCode::SUCCESS;
}

//! Check GetRow function
FaspErrorCode MatCheck_CrossOverRow(const MAT mat, const INT row) {
    try {
        if ( row < 0 || row >= mat.GetRowSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_MAT_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check GetCol function
FaspErrorCode MatCheck_CrossOverCol(const MAT mat, const INT col) {
    try {
        if ( col < 0 || col >= mat.GetColSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_MAT_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check MultVec function
FaspErrorCode MatCheck_MultVec(const MAT mat,const VEC vec) {
    try {
        if ( mat.GetColSize() != vec.GetSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_NONMATCH_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check MultTransposeAdd function
FaspErrorCode MatCheck_MultTransposeAdd(const MAT mat, const VEC vec1, const VEC
vec2) {
    try {
        if ( mat.GetRowSize() != vec1.GetSize() || mat.GetColSize() != vec2.GetSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_NONMATCH_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check Add function
FaspErrorCode MatCheck_Add(const MAT mat1, const MAT mat2) {
    try {
        if ( mat1.GetRowSize() != mat2.GetRowSize() ||
             mat2.GetColSize() != mat2.GetColSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_NONMATCH_SIZE;
    }

    try {
        if ( mat1.GetRowSize() == 0 || mat1.GetColSize() == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_MAT_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_MAT_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check Mult function
FaspErrorCode MatCheck_Mult(const MAT mat1,const MAT mat2) {
    try {
        if ( mat1.GetColSize() != mat2.GetRowSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_NONMATCH_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Sort "colInd" on the order from small to large by its rows and change the
//! positions of "values" according to "colInd"
FaspErrorCode Sort(const INT row,const std::vector<INT> rowPtr,std::vector<INT>
        &colInd,std::vector<DBL> &values){

    INT l;
    INT begin,end;
    INT index;
    DBL data;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = colInd[k];
            data = values[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < colInd[l] ) {
                    colInd[l + 1] = colInd[l];
                    values[l + 1] = values[l];
                } else {
                    break;
                }
            }
            colInd[l + 1] = index;
            values[l + 1] = data;
        }
    }
}

//! check whether the input data are CSRx
FaspErrorCode CheckCSRx(const INT row, const INT col, const INT nnz,
              const std::vector<DBL> values,
              const std::vector<INT> rowPtr,
              const std::vector<INT> colInd,
              const std::vector<INT> diagPtr) {
    if ( row == 0 || col == 0 || nnz == 0 )
        return FaspErrorCode::SUCCESS;

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT flag=0;
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


    flag=1;
    if(flag==0){
        Return:
        return FaspErrorCode::ERROR_INPUT_PAR;
    }

    return FaspErrorCode::SUCCESS;
}

//! check whether the input data are CSR
FaspErrorCode CheckCSR(const INT row, const INT col, const INT nnz,
             const std::vector<DBL> values,
             const std::vector<INT> rowPtr,
             const std::vector<INT> colInd) {
    if ( row == 0 || col == 0 || nnz == 0 )
        return FaspErrorCode ::SUCCESS;

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT flag=0;
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

    flag=1;
    if(flag==0){
        Return:
        return FaspErrorCode ::ERROR_INPUT_PAR;
    }

    return FaspErrorCode::SUCCESS;
}