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
#include "MATUtil.hxx"
#include <cstring>

/// \bridf Read a VEC data file
FaspRetCode ReadVEC(const char *filename, VEC& rhs) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    std::cout << "Reading from disk file " << filename << std::endl;
    std::ifstream in(filename);
    if (!in.is_open()) { // check whether file is opened successfully
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }
    // compute total bytes of file
    in.seekg(0, std::ios::end);
    long long int length = in.tellg();
    in.seekg(0, std::ios::beg);

    char *buffer, *next;
    char decimal[128]; // store a data
    long long int position; // mark the position of file pointer
    INT count, locate, len;

    try { // catch bad allocation error if it happens
        buffer = new char[length]; // apply for the dynamic memory
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length); // read the total bytes of file
    in.close(); // close the file pointer

    /* compute the size of VEC object needed */
    position = 0;
    count = 0;
    while (1) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            ++position;
            ++count;
        } else {
            decimal[count] = '\0';
            count = 0;
            ++position;
            len = std::strtol(decimal, &next, 10);
            break;
        }
    }

    /* apply for the memory space */
    rhs.SetValues(len, 0.0);

    locate = 0; // mark the element position
    while (1) { // compute the VEC object 's elements
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            ++position;
            ++count;
        } else {
            decimal[count] = '\0';
            count = 0;
            ++position;
            rhs[locate] = std::strtod(decimal, &next);
            ++locate;
            if (locate == len)
                break;
        }
    }

    delete[] buffer; // free up memory space

    return retCode;
}

/// \brief Read an MTX data file and store them in (rowInd, colInd, values)
FaspRetCode ReadMTX(const char *filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowInd, std::vector<INT>& colInd,
                    std::vector<DBL>& values) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    std::cout << "Reading from disk file " << filename << std::endl;
    std::ifstream in(filename);
    if (!in.is_open()) { // check whether file is opened successfully
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }
    // compute total bytes of file
    in.seekg(0, std::ios::end);
    long long int length = in.tellg();
    in.seekg(0, std::ios::beg);

    char decimal[128];
    char *buffer, *next;

    // reserve dynamic memory space for storing the whole file
    try { // catch bad allocation if it happens
        buffer = new char[length];
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length); // read the whole file's bytes into buffer
    in.close(); // close the file stream

    long long int position = 0; // mark the position of file pointer
    INT count = 0; // mark the number of valid bytes in the decimal
    INT mark = 0;
    while (1) { // read matrix 's row, column, nnz
        if (buffer[position] != ' ' && buffer[position] != '\n') {
            decimal[count] = buffer[position];
            ++count;
            ++position;
        } else {
            decimal[count] = '\0';
            count = 0;
            ++mark;
            ++position;
            switch (mark) {
                case 1:
                    row = std::strtol(decimal, &next, 10);
                    break;
                case 2:
                    col = std::strtol(decimal, &next, 10);
                    break;
                case 3:
                    nnz = std::strtol(decimal, &next, 10);
                    break;
                default:
                    break;
            }
        }
        if (mark == 3) break;
    }

    // apply for new memory space to store row indices, column indices and values
    try { // catch the bad allocation if it happens
        rowInd.resize(nnz);
        colInd.resize(nnz);
        values.resize(nnz);
    } catch (std::bad_alloc &ex) {
        delete[] buffer; // if bad allocation happens, free up the memory space
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }

    // read the mtx format 's data into rowInd, colInd and values
    INT locate = 0; // mark the element 's position in rowInd, colInd and values
    INT tmp = 0;
    while (1) {
        if (buffer[position] != ' ' && buffer[position] != '\n' &&
            buffer[position] != '\0') {
            decimal[count] = buffer[position];
            ++count;
            ++position;
        } else {
            ++position;
            if (buffer[position] == ' ') continue; // multiple consecutive spaces
            decimal[count] = '\0'; // mark the end of 'decimal' string
            count = 0;
            ++tmp;
            locate = tmp / 3;
            switch (tmp % 3) {
                case 1:
                    rowInd[locate] = std::strtol(decimal, &next, 10) - 1;
                    break;
                case 2:
                    colInd[locate] = std::strtol(decimal, &next, 10) - 1;
                    break;
                case 0:
                    values[locate-1] = std::strtod(decimal, &next);
                    break;
            }
            if (buffer[position] == '\0') break;
        }
    }

    if ( locate != nnz ) retCode = FaspRetCode::ERROR_INPUT_FILE;

    delete[] buffer;

    return retCode;
}

/// \brief Read a CSR data file and store them in (rowPtr, colInd, values)
FaspRetCode ReadCSR(const char *filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowPtr, std::vector<INT>& colInd,
                    std::vector<DBL>& values) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    // Open the file to read
    std::cout << "Reading from disk file " << filename << std::endl;
    std::ifstream in(filename);
    if (!in.is_open()) { // judge whether file is opened successfully
        std::cout << "Reading from disk file " << filename << std::endl;
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }

    // Read the file in to a buffer
    in.seekg(0, std::ios::end);
    long long int length = in.tellg(); // compute total bytes 's number
    in.seekg(0, std::ios::beg);

    char decimal[128];
    // reserve dynamic memory space for storing the whole file
    char *buffer, *next;
    try { // catch the bad allocation if it happens
        buffer = new char[length];
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length); // read the whole file's bytes
    in.close(); // close the file stream

    // Read number of rows
    INT count = 0;
    long long int position = 0; // mark the position of file pointer
    while (true) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            ++count;
            ++position;
        } else {
            decimal[count] = '\0'; // mark the end of 'decimal' string
            ++position;
            break;
        }
    }

    row = std::strtol(decimal, &next, 10);
    if (row <= 0) { // prevent memory leaks if error happens
        retCode = FaspRetCode::ERROR_INPUT_PAR;
        delete[] buffer;
        return retCode;
    }
    col = row;

    // Read row pointers
    try { // catch bad allocation if it happens
        rowPtr.resize(row + 1);
    } catch (std::bad_alloc &ex) {
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }

    // read the rowPtr of CSRx matrix
    INT locate = 0;
    count = 0;
    while (true) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            ++count;
            ++position;
        } else {
            ++position;
            decimal[count] = '\0';
            count = 0;
            rowPtr[locate] = std::strtol(decimal, &next, 10);
            ++locate;
            if (locate == row + 1) break;
        }
    }

    // Reserve memory for colInd and values
    try { // catch bad allocation if it happens
        nnz = rowPtr[row] - rowPtr[0];
        colInd.resize(nnz);
        values.resize(nnz);
    } catch (std::bad_alloc &ex) {
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }

    // Read column indices
    locate = 0;
    while (true) {
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            ++count;
            ++position;
        } else {
            ++position;
            decimal[count] = '\0';
            count = 0;
            colInd[locate] = std::strtol(decimal, &next, 10);;
            ++locate;
            if (locate == nnz) break;
        }
    }

    // Read values
    locate = 0;
    while (true) {
        if (buffer[position] != '\n' && buffer[position] != '\0') {
            decimal[count] = buffer[position];
            ++count;
            ++position;
        } else {
            if (buffer[position] == '\0') break;
            ++position;
            decimal[count] = '\0';
            count = 0;
            values[locate] = std::strtod(decimal, &next);;
            ++locate;
        }
    }
    if (locate != nnz) retCode = FaspRetCode::ERROR_INPUT_FILE;

    // If the indices start from 1, we shift them to start from 0
    if (rowPtr[0] == 1) {
        for (count = 0; count <= row; ++count) rowPtr[count]--;
        for (count = 0; count < nnz; ++count) colInd[count]--;
    }

    delete[] buffer; // free up memory space

    return retCode;
}

FaspRetCode ReadMat(const char *filename, MAT& mat){
    char type[4];
    INT flag;
    INT len=strlen(filename);
    FaspRetCode retCode;

    if(len<=4){
        retCode=FaspRetCode ::ERROR_INPUT_FILE;
        return retCode;
    }

    type[0]=filename[len-3];
    type[1]=filename[len-2];
    type[2]=filename[len-1];
    type[3]='\0';

    if(strcmp(type,"csr")==0)
        flag=1;
    else if(strcmp(type,"mtx")==0)
        flag=2;
    else
        flag=0;

    INT row,col,nnz;
    std::vector<INT> rowPtr,colInd,rowInd;
    std::vector<DBL> values;

    switch(flag){
        case 0:
            retCode=FaspRetCode ::ERROR_INPUT_FILE;
            break;
        case 1:
            try {
                retCode = ReadCSR(filename,row, col, nnz,
                        rowPtr, colInd, values);
                if ( retCode < 0 )
                    throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
            }
            catch ( FaspRunTime& ex ) {
                ex.LogExcep();
                break;
            }

            // Sort each row in ascending order
            try {
                retCode = SortCSRRow(row, col, nnz, rowPtr, colInd, values);
                if ( retCode < 0 )
                    throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
            }
            catch ( FaspRunTime& ex ) {
                ex.LogExcep();
                break;
            }
            // Convert a MTX matrix to MAT
            try {
                retCode = CSRtoMAT(row, col, nnz, values, colInd, rowPtr, mat);
                if ( retCode < 0 )
                    throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
            }
            catch ( FaspRunTime& ex ) {
                ex.LogExcep();
                break;
            }
            break;
        case 2:
            try {
                retCode = ReadMTX(filename,row, col, nnz,rowInd,
                        colInd, values);
                if ( retCode < 0 )
                    throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
            }
            catch ( FaspRunTime& ex ) {
                ex.LogExcep();
                break;
            }

            // Sort each row in ascending order
            try {
                retCode = MTXtoMAT(row, col, nnz, rowInd, colInd, values,mat);
                if ( retCode < 0 )
                    throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
            }
            catch ( FaspRunTime& ex ) {
                ex.LogExcep();
                break;
            }
            break;
    }

    return retCode;
}
/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/