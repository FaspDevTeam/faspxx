/*! \file    MATUtil.cxx
 *  \brief   Some auxiliary functions for MAT
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Sep/26/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "MATUtil.hxx"
#include "RetCode.hxx"

/// Check whether two matrices have same sizes for addition
FaspRetCode CheckMATAddSize(const MAT& mat1, const MAT& mat2)
{
    try {
        if ( mat1.GetRowSize() != mat2.GetRowSize() ||
             mat2.GetColSize() != mat2.GetColSize() ||
             mat1.GetRowSize() <= 0 || mat1.GetColSize() <= 0 ) {
            auto errorCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw( FaspRunTime(errorCode, __FILE__, __FUNCTION__, __LINE__) );
        }
    } catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return FaspRetCode::SUCCESS;
}

/// Check MAT-MAT multiplication sizes
FaspRetCode CheckMATMultSize(const MAT& mat1, const MAT& mat2)
{
    try {
        if ( mat1.GetColSize() != mat2.GetRowSize() ||
             mat1.GetColSize() <= 0 || mat2.GetRowSize() <= 0 ) {
            auto errorCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw( FaspRunTime(errorCode, __FILE__, __FUNCTION__, __LINE__) );
        }
    } catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return FaspRetCode::SUCCESS;
}

/// Check whether (row,col) is out of bound
FaspRetCode CheckMATSize(const MAT& mat, const INT& row, const INT& col)
{
    try {
        if ( row < 0 || row >= mat.GetRowSize() ||
             col < 0 || col >= mat.GetColSize() ) {
            auto errorCode = FaspRetCode::ERROR_MAT_SIZE;
            throw( FaspRunTime(errorCode, __FILE__, __FUNCTION__, __LINE__) );
        }
    } catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return FaspRetCode::SUCCESS;
}

/// Check whether (row,:) is out of bound
FaspRetCode CheckMATRowSize(const MAT& mat, const INT& row)
{
    try {
        if ( row < 0 || row >= mat.GetRowSize() ) {
            auto errorCode = FaspRetCode::ERROR_MAT_SIZE;
            throw( FaspRunTime(errorCode, __FILE__, __FUNCTION__, __LINE__) );
        }
    } catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return FaspRetCode::SUCCESS;
}

/// Check whether (:,col) is out of bound
FaspRetCode CheckMATColSize(const MAT& mat, const INT& col)
{
    try {
        if ( col < 0 || col >= mat.GetColSize() ) {
            auto errorCode = FaspRetCode::ERROR_MAT_SIZE;
            throw( FaspRunTime(errorCode, __FILE__, __FUNCTION__, __LINE__) );
        }
    } catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return FaspRetCode::SUCCESS;
}

/// Check MAT-VEC multiplication sizes
FaspRetCode CheckMATVECSize(const MAT& mat, const VEC& vec)
{
    try {
        if ( mat.GetColSize() != vec.GetSize() ) {
            auto errorCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw( FaspRunTime(errorCode, __FILE__, __FUNCTION__, __LINE__) );
        }
    } catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return FaspRetCode::SUCCESS;
}

/// Check whether the data is good for CSR
FaspRetCode CheckCSR(const INT& row, const INT& col, const INT& nnz,
                     const std::vector<DBL>& values, const std::vector<INT>& colInd,
                     const std::vector<INT>& rowPtr)
{
    if ( row == 0 || col == 0 || nnz == 0 ) return FaspRetCode ::SUCCESS;

    if ( row != rowPtr.size() - 1 ) {
        std::cout << "### ERROR: row= "  << row
                  << ", but rowPtr.size= " << rowPtr.size() << std::endl;
        goto WRONG_CSR;
    }

    if ( row <= 0 || col <= 0 ) {
        std::cout << "### ERROR: row= "  << row << ", col= " << col << std::endl;
        goto WRONG_CSR;
    }

    if ( nnz != colInd.size() ) {
        std::cout << "### ERROR: nnz= "  << nnz
                  << ", but colInd.size= " << colInd.size() << std::endl;
        goto WRONG_CSR;
    }

    if ( nnz != values.size()) {
        std::cout << "### ERROR: nnz= "  << nnz
                  << ", but values.size= " << values.size() << std::endl;
        goto WRONG_CSR;
    }

    if ( nnz != rowPtr[row] - rowPtr[0] ) {
        std::cout << "### ERROR: nnz= "  << nnz << ", rowPtr[last]-rowPtr[first]= "
                  << rowPtr[row] - rowPtr[0] << std::endl;
        goto WRONG_CSR;
    }

    for ( INT j = 0; j < row; ++j ) {
        if ( rowPtr[j] > rowPtr[j + 1] ) {
            std::cout << "### ERROR: Problem in " << j << "-th row, "
                      << "starting row pointer = " << rowPtr[j]
                      << ", ending row pointer = " << rowPtr[j+1] << std::endl;
            goto WRONG_CSR;
        }
    }

    INT begin, end;
    for ( INT j = 0; j < row; ++j ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];

        if ( begin == end ) continue;

        for ( INT k = begin; k < end; ++k ) {
            if ( 0 > colInd[k] || colInd[k] >= col ) {
                std::cout << "### ERROR: Wrong column indices" << std::endl;
                goto WRONG_CSR;
            }
        }
    }

    return FaspRetCode::SUCCESS;

    WRONG_CSR:
    return FaspRetCode::ERROR_MAT_DATA;
}

/// Check whether the data is good for CSRx
FaspRetCode CheckCSRx(const INT& row, const INT& col, const INT& nnz,
                      const std::vector<DBL>& values, const std::vector<INT>& colInd,
                      const std::vector<INT>& rowPtr, const std::vector<INT>& diagPtr)
{
    if ( row == 0 || col == 0 || nnz == 0 ) return FaspRetCode::SUCCESS;

    /*
     * some simple examinations about parameters
     * to judge whether they are CSRx' parameters
     */
    /*----------------  begin  ----------------*/
    /// basic examinations
    INT count = 0;
    INT begin, end;

    if ( row != rowPtr.size() - 1 ) goto Return;

    if ( row <= 0 || col <= 0 ) goto Return;

    if (((row > col) ? col : row) != diagPtr.size()) goto Return;

    if ( nnz != colInd.size()) goto Return;

    if ( nnz != values.size()) goto Return;

    if ( nnz != rowPtr[rowPtr.size() - 1] ) goto Return;

    /// simple examinations
    for ( INT j = 0; j < row; ++j ) {
        if ( rowPtr[j] >= rowPtr[j + 1] ) goto Return;
    }

    if ( rowPtr[0] < 0 || rowPtr[row] > nnz ) goto Return;

    for ( INT j = 0; j < row; ++j ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( begin == end ) goto Return;

        if ( end == begin + 1 ) {
            if ( colInd[begin] != j ) goto Return;
        }

        if ( end > begin + 1 ) {
            for ( INT k = begin; k < end - 1; ++k ) {
                if ( colInd[k] >= colInd[k + 1] ) goto Return;
            }
            if ( 0 > colInd[begin] ) goto Return;

            if ( colInd[end - 1] >= col ) goto Return;
        }
    }

    /// exam diagPtr and colInd
    for ( INT j = 0; j < row; ++j ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        for ( INT k = begin; k < end; ++k ) {
            if ( colInd[k] == j ) {
                if ( diagPtr[count] != k )
                    goto Return;
                else
                    ++count;
            }
        }
    }
    if ( count != diagPtr.size()) goto Return;

    return FaspRetCode::SUCCESS;

    Return: return FaspRetCode::ERROR_INPUT_PAR;
}

/// Convert MTX data (indices from 0) to CSR data structure (private)
static
FaspRetCode MTXtoCSR(const INT& row, const INT& col, const INT& nnz,
                     const std::vector<INT>& rowInd, const std::vector<INT>& colInd,
                     const std::vector<DBL>& values,
                     std::vector<DBL>& valuesCSR, std::vector<INT>& colIndCSR,
                     std::vector<INT>& rowPtrCSR)
{
    FaspRetCode retCode;

    try{
        valuesCSR.assign(nnz,0);
        colIndCSR.assign(nnz,0);
        rowPtrCSR.assign(row + 1,0);
    }catch(std::bad_alloc& ex){
        throw(FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__));
    }

    // 1. Count number of non zeros in each row -> rowPtrCSR[row+1]
    rowPtrCSR[0] = 0;
    for ( INT j = 0; j < nnz; ++j ) ++rowPtrCSR[rowInd[j]+1];

    // 2. Form an initial pointer for each row -> rowPtrCSR[row+1]
    for ( INT i = 1; i < row; ++i ) rowPtrCSR[i] += rowPtrCSR[i-1];

    // 3. Set values and colInd for CSR
    // Note: rowPtr used as a temporary pointer for current insertion position
    INT irow, jptr;
    for ( INT j = 0; j < nnz; ++j ) {
        irow = rowInd[j];
        jptr = rowPtrCSR[irow];
        colIndCSR[jptr] = colInd[j];
        valuesCSR[jptr] = values[j];
        rowPtrCSR[irow] = ++jptr;
    }

    // 4. Restore rowPtr back to the  original value
    for ( INT i = row; i > 0; i-- ) rowPtrCSR[i] = rowPtrCSR[i-1];
    rowPtrCSR[0] = 0;

    // 5. Check whether the format is CSR or not
    try {
        retCode = CheckCSR(row, col, nnz, valuesCSR, colIndCSR, rowPtrCSR);
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime &ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
}

/// Convert a CSR matrix to MAT (private)
FaspRetCode CSRtoMAT(const INT& row, const INT& col, const INT& nnz,
                     const std::vector<DBL>& values, const std::vector<INT>& colInd,
                     const std::vector<INT>& rowPtr, MAT& mat)
{
    FaspRetCode retCode;
    INT begin, end, numZeroDiag = 0;
    std::vector<bool> isZeroDiag(row);

    for ( INT i = 0; i < row; ++i) {
        isZeroDiag[i] = false; // Set a marker for diagonal pointer
        begin = rowPtr[i];
        end   = rowPtr[i+1];
        for ( INT k = begin; k < end; ++k ) {
            if ( colInd[k] == i ) {
                isZeroDiag[i] = true; // Diagonal entry found
                break;
            }
        }
        if ( !isZeroDiag[i] ) ++numZeroDiag; // If no diagonal entry, insert a zero
    }

    if ( numZeroDiag == 0 ) {
        // Set values for MAT matrix
        try {
            retCode = CheckCSR(row, col, nnz, values, colInd, rowPtr);
            if ( retCode < 0 )
                throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
            else
                mat.SetValues(row, col, nnz, values, colInd, rowPtr);
        } catch ( FaspRunTime& ex ) {
            ex.LogExcep();
            return ex.errorCode;
        }
    }

    // Add a spot for each zero diagonal entry
    INT nnzNew = nnz + numZeroDiag, count = 0;
    INT nextEntry, diagZeroAdded = 0;

    std::vector<INT> colIndNew;
    std::vector<DBL> valuesNew;
    std::vector<INT> rowPtrNew;

    try {
        colIndNew.resize(nnzNew);
        valuesNew.resize(nnzNew);
        rowPtrNew.resize(row+1);
    } catch(std::bad_alloc& ex) {
        throw( FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__) );
    }

    rowPtrNew[0] = 0; // Starting index always 0
    for ( INT m = 0; m < row+1; ++m ) rowPtrNew[m] = 0;

    INT k, j;
    for ( INT i = 0; i < row; ++i) {
        begin = rowPtr[i];
        end   = rowPtr[i+1];
        if ( !isZeroDiag[i] ) { // No diagonal entry
            // Lower triangular part
            nextEntry = begin;
            for ( k = begin; k < end; ++k ) {
                if ( colInd[k] < i ) {
                    valuesNew[count] = values[k];
                    colIndNew[count] = colInd[k];
                    ++count;
                } else {
                    nextEntry = k; // Record next nonzero location
                    break;
                }
            }

            // Zero diagonal part
            valuesNew[count] = 0.0;
            colIndNew[count] = i;
            ++count;

            if ( colInd[end-1] < i ) nextEntry = end;

            // Upper triangular part
            for ( j = nextEntry; j < end; ++j ) {
                valuesNew[count] = values[j];
                colIndNew[count] = colInd[j];
                ++count;
            }
            // Update row pointers
            ++diagZeroAdded;
        } else { // With diagonal entry, just copy
            for ( k = begin; k < end; ++k ) {
                valuesNew[count] = values[k];
                colIndNew[count] = colInd[k];
                ++count;
            }
        }
        rowPtrNew[i+1] = rowPtr[i+1] + diagZeroAdded;
    }

    // Set values for MAT matrix
    try {
        retCode = CheckCSR(row, col, nnzNew, valuesNew, colIndNew, rowPtrNew);
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
        else
            mat.SetValues(row, col, nnzNew, valuesNew, colIndNew, rowPtrNew);
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
}

/// Convert MTX data to MAT
FaspRetCode MTXtoMAT(const INT& row, const INT& col, const INT& nnz,
                     const std::vector<INT>& rowInd, const std::vector<INT>& colInd,
                     const std::vector<DBL>& values, MAT& mat)
{
    auto retCode = FaspRetCode::SUCCESS;

    std::vector<INT> rowPtrCSR;
    std::vector<INT> colIndCSR;
    std::vector<DBL> valuesCSR;

    // Convert data format from MTX to CSR
    MTXtoCSR(row, col, nnz, rowInd, colInd, values,valuesCSR, colIndCSR,rowPtrCSR);

    // Sort CSR matrix row by row
    SortCSRRow(row, col, nnz, rowPtrCSR, colIndCSR, valuesCSR);

    // Check whether diagonal is a nonzero position
    CSRtoMAT(row, col, nnz, valuesCSR, colIndCSR, rowPtrCSR, mat);

    return retCode;
}

/// Sort "colInd" of each row in ascending order and rearrange "values" accordingly
FaspRetCode SortCSRRow(const INT& row, const INT& col, const INT& nnz,
                       const std::vector<INT>& rowPtr,
                       std::vector<INT>& colInd, std::vector<DBL>& values)
{
    FaspRetCode retCode;
    INT l, begin, end, index;
    DBL data;

    for ( INT j = 0; j < row; ++j ) {
        begin = rowPtr[j];
        end = rowPtr[j + 1];
        if ( end <= begin + 1 ) continue;
        for ( INT k = begin + 1; k < end; ++k ) {
            index = colInd[k];
            data = values[k];
            for ( l = k - 1; l >= begin; --l ) {
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

    try {
        retCode = CheckCSR(row, col, nnz, values, colInd, rowPtr);
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/