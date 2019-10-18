/** \file ReadData.cxx
 *  \brief Reading data from disk files
 *  \author Chensong Zhang, Kailei Zhang
 *  \date Oct/11/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <fstream>
#include "ReadData.hxx"

/// \brief Read an MTX data file and store them in (rowInd, colInd, values)
FaspRetCode ReadMTX(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowInd, std::vector<INT>& colInd,
                    std::vector<DBL>& values)
{
#if 0
    std::cout << __FUNCTION__ << ": reading file " << filename << "..." << std::endl;
    auto retCode = FaspRetCode::SUCCESS;

    // Open the disk file to read
    std::ifstream infile(filename);
    try {
        if ( !infile ) retCode = FaspRetCode::ERROR_OPEN_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Get matrix size and number of nonzeros
    try {
        infile >> row >> col >> nnz;
        if ( row <= 0 || col <= 0 || nnz <= 0 ) retCode = FaspRetCode::ERROR_INPUT_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Reserve memory space for MTX data: (i, j, value)
    try {
        rowInd.resize(nnz);
        colInd.resize(nnz);
        values.resize(nnz);
    } catch ( std::bad_alloc& ex ) {
        throw(FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__));
    }
    
    // Read data from file and store them in (rowInd, colInd, values)
    INT count = 0, rowValue, colValue;
    DBL value;
    while ( infile >> rowValue >> colValue >> value ) {
         rowInd[count] = rowValue - 1; // MTX indices start from 1, we use 0
         colInd[count] = colValue - 1; // MTX indices start from 1, we use 0
         values[count] = value;
         ++count;
    }

    // Check whether file provides enough data
    try {
        if ( count != nnz ) retCode = FaspRetCode::ERROR_INPUT_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
#endif
    std::ifstream in(filename);
    FaspRetCode retCode=FaspRetCode ::SUCCESS;
    if (!in.is_open()) {
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }
    in.seekg(0, std::ios::end);
    long long int length = in.tellg();
    in.seekg(0, std::ios::beg);
    char *buffer, *decimal;
    try {
        buffer = new char[length];
        decimal = new char[16];
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode=FaspRetCode ::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length);
    in.close();

    INT count = 0;
    long long int position = 0;
    INT mark = 0;
    while (1) {
        if (buffer[position] != ' ' && buffer[position] != '\n') {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            decimal[count] = '\0';
            count = 0;
            mark++;
            position++;
            switch (mark) {
                case 1:
                    row = atoi(decimal);
                    break;
                case 2:
                    col = atoi(decimal);
                    break;
                case 3:
                    nnz = atoi(decimal);
                    break;
                default:
                    break;
            }
        }
        if (mark == 3)
            break;
    }

    try {
        rowInd.resize(nnz);
        colInd.resize(nnz);
        values.resize(nnz);
    } catch (std::bad_alloc &ex) {
        delete[] buffer;
        delete[] decimal;
        retCode=FaspRetCode ::ERROR_ALLOC_MEM;
        return retCode;
    }

    INT locate = 0;
    INT tmp=0;
    while (1) {
        if (buffer[position] != ' ' && buffer[position] != '\n' &&
            buffer[position] != EOF) {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            position++;
            if(buffer[position]==' ')
                continue;
            decimal[count] = '\0';
            count = 0;
            tmp++;
            locate=tmp/3;
            switch (tmp%3) {
                case 1:
                    rowInd[locate] = atoi(decimal)-1;
                    break;
                case 2:
                    colInd[locate] = atoi(decimal)-1;
                    break;
                case 0:
                    values[locate-1] = atof(decimal);
                    break;
            }
        }
        if (locate == nnz)
            break;
    }

    return retCode;
}

/// \brief Read a CSR data file and store them in (rowPtr, colInd, values)
FaspRetCode ReadCSR(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowPtr, std::vector<INT>& colInd,
                    std::vector<DBL>& values)
{
#if 0
    std::cout << __FUNCTION__ << ": reading file " << filename << "..." << std::endl;
    auto retCode = FaspRetCode::SUCCESS;
    INT count;

    // Open the file to read
    std::ifstream infile(filename);
    try {
        if ( !infile ) retCode = FaspRetCode::ERROR_OPEN_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Get number of rows of matrix
    try {
        infile >> row;
        if ( row <= 0 ) retCode = FaspRetCode::ERROR_INPUT_FILE;
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    } catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Reserve memory space for rowPtr only
    try {
        rowPtr.resize(row+1);
    } catch ( std::bad_alloc& ex ) {
        throw( FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__) );
    }

    // Read data from file and store them in rowPtr
    for ( count = 0; count <= row; ++count ) infile >> rowPtr[count];

    // Calculate problem size and number of nonzeros
    try {
        col = row;
        nnz = rowPtr[row] - rowPtr[0]; // rowPtr[0] could be 0 or 1
        colInd.resize(nnz);
        values.resize(nnz);
    } catch ( std::bad_alloc& ex ) {
        throw( FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__) );
    }

    // Read data from file and store them in colInd
    for ( count = 0; count < nnz; ++count ) infile >> colInd[count];

    // Read data from file and store them in values
    for ( count = 0; count < nnz; ++count ) infile >> values[count];

    // Shift indices: Some Fortran-style data has indices from 1
    if ( rowPtr[0] != 0 ) {
        for ( count = 0; count <  nnz; ++count ) colInd[count]--;
        for ( count = 0; count <= row; ++count ) rowPtr[count]--;
    }

    return retCode;
#endif
    std::ifstream in(filename);
    FaspRetCode retCode=FaspRetCode ::SUCCESS;
    if ( !in.is_open() ){
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }

    in.seekg(0, std::ios::end);
    long long int length = in.tellg();
    in.seekg(0, std::ios::beg);
    char *buffer,*decimal;
    try {
        buffer = new char[length];
        decimal = new char[16];
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode=FaspRetCode ::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length);
    in.close();

    INT count = 0;
    long long int position = 0;
    while (1) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            decimal[count] = '\0';
            position++;
            break;
        }
    }

    row = atoi(decimal);

    if(row<=0){
        retCode = FaspRetCode::ERROR_INPUT_PAR;
        delete[] buffer;
        delete[] decimal;
        return retCode;
    }

    col = row;
    long long int locate = 0;
    count = 0;
    try{
        rowPtr.resize(row + 1);
    }catch(std::bad_alloc &ex){
        retCode=FaspRetCode ::ERROR_ALLOC_MEM;
        return retCode;
    }

    while (1) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            position++;
            decimal[count] = '\0';
            count = 0;
            rowPtr[locate] = atoi(decimal);
            locate++;
            if (locate == row + 1)
                break;
        }
    }

    nnz = rowPtr[row] - rowPtr[0];

    try{
        colInd.resize(nnz);
        values.resize(nnz);
    }catch(std::bad_alloc &ex){
        retCode=FaspRetCode ::ERROR_ALLOC_MEM;
        return retCode;
    }

    locate = 0;
    while (1) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            position++;
            decimal[count] = '\0';
            count = 0;
            colInd[locate] = atof(decimal);
            locate++;
            if (locate == nnz)
                break;
        }
    }

    locate = 0;
    while (1) {
        if (buffer[position] != '\n' && buffer[position] != EOF) {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            position++;
            decimal[count] = '\0';
            count = 0;
            values[locate] = atof(decimal);
            locate++;
            if (locate == nnz)
                break;
        }
    }

    if (rowPtr[0] != 0) {
        for (count = 0; count < nnz; ++count) colInd[count]--;
        for (count = 0; count <= row; ++count) rowPtr[count]--;
    }

    delete[] buffer;
    delete[] decimal;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/