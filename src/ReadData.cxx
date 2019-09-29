/**\file ReadData.cxx
 *  \brief Reading data from disk files
 *  \author Chensong Zhang, Kailei Zhang
 *  \date Sep/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <fstream>
#include "ReadData.hxx"

/// \brief Read a MTX data file and store them (rowInd, colInd, values)
FaspRetCode ReadMTX(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowInd, std::vector<INT>& colInd,
                    std::vector<DBL>& values)
{
    std::cout << __FUNCTION__ << ": reading file " << filename << "..." << std::endl;
    auto retCode = FaspRetCode::SUCCESS;

    // Open the file to read
    std::ifstream infile(filename);
    try {
        if ( !infile ) retCode = FaspRetCode::ERROR_OPEN_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Get matrix size and number of nonzeros
    infile >> row >> col >> nnz;
    try {
        if ( row <= 0 || col <= 0 || nnz <= 0 ) retCode = FaspRetCode::ERROR_INPUT_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Reserve memory space for MTX data: (i, j, value)
    try{
        rowInd.resize(nnz);
        colInd.resize(nnz);
        values.resize(nnz);
    }catch(std::bad_alloc& ex){
        throw(FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__));
    }
    
    // Read data from file and store them in vectors
    INT count = 0, rowValue, colValue;
    DBL value;
    while ( infile >> rowValue >> colValue >> value ) {
         rowInd[count] = rowValue - 1; // MTX indices start from 1, we use 0
         colInd[count] = colValue - 1; // MTX indices start from 1, we use 0
         values[count] = value;
         count++;
    }

    // Check whether file provides enough data
    try {
        if ( count != nnz ) retCode = FaspRetCode::ERROR_INPUT_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/