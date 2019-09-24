/**
 * a source file about some auxiliary functions
 */

#include "auxmat.hxx"

//! check GetElem function
FaspErrorCode MatCheck_CrossOver(const MAT mat, const INT row, const INT column) {
    try {
        if ( row < 0 || row >= mat.GetRow() || column < 0 ||
             column >= mat.GetColumn()) {
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
        if ( row < 0 || row >= mat.GetRow()) {
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

//! Check GetColumn function
FaspErrorCode MatCheck_CrossOverColumn(const MAT mat, const INT column) {
    try {
        if ( column < 0 || column >= mat.GetColumn()) {
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
        if ( mat.GetColumn() != vec.GetSize()) {
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
        if ( mat.GetRow() != vec1.GetSize() || mat.GetColumn() != vec2.GetSize()) {
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
        if ( mat1.GetRow() != mat2.GetRow() ||
             mat2.GetColumn() != mat2.GetColumn()) {
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
        if ( mat1.GetRow() == 0 || mat1.GetColumn() == 0 ) {
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
        if ( mat1.GetColumn() != mat2.GetRow()) {
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

//! Sort "colindex" on the order from small to large by its rows and change the
//! positions of "values" according to "colindex"
FaspErrorCode Sort(const INT row,const std::vector<INT> rowshift,std::vector<INT>
        &colindex,std::vector<DBL> &values){

    INT l;
    INT begin,end;
    INT index;
    DBL data;
    for ( INT j = 0; j < row; j++ ) {
        begin = rowshift[j];
        end = rowshift[j + 1];
        if ( end <= begin + 1 ) {
            continue;
        }
        for ( INT k = begin + 1; k < end; k++ ) {
            index = colindex[k];
            data = values[k];
            for ( l = k - 1; l >= begin; l-- ) {
                if ( index < colindex[l] ) {
                    colindex[l + 1] = colindex[l];
                    values[l + 1] = values[l];
                } else {
                    break;
                }
            }
            colindex[l + 1] = index;
            values[l + 1] = data;
        }
    }
}

//! check whether the input data are CSRx
FaspErrorCode CheckCSRx(const INT row, const INT column, const INT nnz,
              const std::vector<DBL> values,
              const std::vector<INT> rowshift,
              const std::vector<INT> colindex,
              const std::vector<INT> diag) {
    if ( row == 0 || column == 0 || nnz == 0 )
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


    flag=1;
    if(flag==0){
        Return:
        return FaspErrorCode::ERROR_INPUT_PAR;
    }

    return FaspErrorCode::SUCCESS;
}

//! check whether the input data are CSR
FaspErrorCode CheckCSR(const INT row, const INT column, const INT nnz,
             const std::vector<DBL> values,
             const std::vector<INT> rowshift,
             const std::vector<INT> colindex) {
    if ( row == 0 || column == 0 || nnz == 0 )
        return FaspErrorCode ::SUCCESS;

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    //! basic examinations
    INT flag=0;
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

    flag=1;
    if(flag==0){
        Return:
        return FaspErrorCode ::ERROR_INPUT_PAR;
    }

    return FaspErrorCode::SUCCESS;
}