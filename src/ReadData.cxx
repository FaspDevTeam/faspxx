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
FaspRetCode ReadMTX(const char *filename, INT &row, INT &col, INT &nnz,
                    std::vector<INT> &rowInd, std::vector<INT> &colInd,
                    std::vector<DBL> &values) {
    std::ifstream in(filename);
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    if (!in.is_open()) { // judge whether file is opened successfully
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }
    // compute total bytes of file
    in.seekg(0, std::ios::end);
    long long int length = in.tellg();
    in.seekg(0, std::ios::beg);

    // apply for dynamic memory space for storing total bytes of file and
    // single data
    char *buffer, *decimal;
    try { // catch bad allocation if it happens
        buffer = new char[length];
        decimal = new char[128];
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length); // read the whole file's bytes into buffer
    in.close(); // close the file stream

    INT count = 0; // mark the number of valid bytes in the decimal
    long long int position = 0; // mark the position of file pointer
    INT mark = 0;
    while (1) { // read matrix 's row, column, nnz
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

    // apply for new memory space to store row indices, column indices and values
    try { // catch the bad allocation if it happens
        rowInd.resize(nnz);
        colInd.resize(nnz);
        values.resize(nnz);
    } catch (std::bad_alloc &ex) {
        delete[] buffer; // if bad allocation happens, free up the memory space
        delete[] decimal;
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }

    // read the mtx format 's data into rowInd, colInd and values
    INT locate = 0; // mark the element 's position in rowInd, colInd and values
    INT tmp = 0;
    while (1) {
        if (buffer[position] != ' ' && buffer[position] != '\n' &&
            buffer[position] != EOF) {
            decimal[count] = buffer[position];
            count++;
            position++;
        } else {
            position++;
            if (buffer[position] == ' ') // prevent multiple consecutive spaces
                continue;
            decimal[count] = '\0'; // mark the end of 'dicimal' string
            count = 0;
            tmp++;
            locate = tmp / 3;
            switch (tmp % 3) {
                case 1:
                    rowInd[locate] = atoi(decimal) - 1;
                    break;
                case 2:
                    colInd[locate] = atoi(decimal) - 1;
                    break;
                case 0:
                    values[locate - 1] = atof(decimal);
                    break;
            }
        }
        if (buffer[position - 1] == EOF)
            break;
    }

    if (locate != nnz) retCode = FaspRetCode::ERROR_INPUT_FILE;

    return retCode;
}

/// \brief Read a CSR data file and store them in (rowPtr, colInd, values)
FaspRetCode ReadCSR(const char *filename, INT &row, INT &col, INT &nnz,
                    std::vector<INT> &rowPtr, std::vector<INT> &colInd,
                    std::vector<DBL> &values) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    // Open the file to read
    std::ifstream in(filename);
    if (!in.is_open()) { // judge whether file is opened successfully
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }

    // Read the file in to a buffer
    in.seekg(0, std::ios::end);
    long long int length = in.tellg(); // compute total bytes 's number
    in.seekg(0, std::ios::beg);
    char *buffer, *decimal;
    try { // catch the bad allocation if it happens
        buffer = new char[length];
        decimal = new char[128]; // Note: Careful with very long data
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
            count++;
            position++;
        } else {
            decimal[count] = '\0'; // mark the end of 'decimal' string
            position++;
            break;
        }
    }

    row = atoi(decimal);
    if (row <= 0) { // prevent memory leaks if error happens
        retCode = FaspRetCode::ERROR_INPUT_PAR;
        delete[] buffer;
        delete[] decimal;
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
            count++;
            position++;
        } else {
            position++;
            decimal[count] = '\0';
            count = 0;
            rowPtr[locate] = atoi(decimal);
            locate++;
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
            count++;
            position++;
        } else {
            position++;
            decimal[count] = '\0';
            count = 0;
            colInd[locate] = atoi(decimal);
            locate++;
            if (locate == nnz) break;
        }
    }

    // Read values
    locate = 0;
    while (true) {
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
        }
        if (buffer[position - 1] == EOF)
            break;
    }
    if (locate != nnz) retCode = FaspRetCode::ERROR_INPUT_FILE;

    // If the indices start from 1, we shift them to start from 0
    if (rowPtr[0] == 1) {
        for (count = 0; count <= row; ++count) rowPtr[count]--;
        for (count = 0; count < nnz; ++count) colInd[count]--;
    }

    delete[] buffer; // free up memory space
    delete[] decimal;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/