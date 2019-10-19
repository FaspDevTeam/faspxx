/**
 * a source file about reading command parameters
 */

#include "ReadCommand.hxx"
#include "ReadData.hxx"
#include "MATUtil.hxx"
#include <fstream>

//using namespace std;

/// read rhs or initial solution
static FaspRetCode ReadRHS(char *filename, VEC &rhs) {
    FaspRetCode retCode;
    char *buffer; // store the whole data file
    char decimal[128]; // store a data
    long long int length; // total bytes of file
    long long int position; // mark the position of file pointer
    INT count, locate, len;

    std::ifstream in(filename);
    if (in.is_open()) { // judge whether file is opened successfully
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }
    // compute total bytes of file
    in.seekg(0, std::ios::end);
    length = in.tellg();
    in.seekg(0, std::ios::beg);

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
            position++;
            count++;
        } else {
            decimal[count] = '\0';
            count = 0;
            position++;
            len = atoi(decimal);
            break;
        }
    }

    /* apply for the memory space */
    rhs.SetValues(len, 0.0);

    locate = 0; // mark the element position
    while (1) { // compute the VEC object 's elements
        if (buffer[position] != '\n') {
            decimal[count] = buffer[position];
            position++;
            count++;
        } else {
            decimal[count] = '\0';
            count = 0;
            position++;
            rhs[locate] = atof(decimal);
            locate++;
            if (locate == len)
                break;
        }
    }

    delete[] buffer; // free up memory space
}

// Todo: This is already define somewhere else
// explain : functions both are different, so I change the name of this function
/// convert CSR format to CSRx matrix
FaspRetCode ReadCSRToMAT(char *filename, MAT &mat) {
    // Read a CSR matrix from file
    INT row, col, nnz;
    std::vector<INT> rowPtr; // row pointer
    std::vector<INT> colInd; // col index
    std::vector<DBL> values; // matrix nnz

    FaspRetCode retCode;
    try {
        retCode = ReadCSR(filename, row, col, nnz, rowPtr, colInd, values);
        if (retCode < 0)
            throw (FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
    }
    catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Sort each row in ascending order
    try {
        retCode = SortCSRRow(row, col, nnz, rowPtr, colInd, values);
        if (retCode < 0)
            throw (FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
    }
    catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Convert a MTX matrix to MAT
    try {
        retCode = CSRtoMAT(row, col, nnz, values, colInd, rowPtr, mat);
        if (retCode < 0)
            throw (FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
    }
    catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
}

FaspRetCode ReadCommand(unsigned int argc, char *args[],
                        MAT &mat, VEC &rhs, VEC &initGuess) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    switch (argc) {
        case 2: // only read matrix
            retCode = ReadCSRToMAT(args[1], mat);
            break;
        case 3: // read matrix and right hand item
            retCode = ReadCSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadRHS(args[2], rhs);
            break;
        case 4: // read matrix, right hand item and initial solution
            retCode = ReadCSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadRHS(args[2], rhs);
            if (retCode < 0) break;
            retCode = ReadRHS(args[3], initGuess);
            break;
        default: // if no data file is specified, trigger error mechanism
            std::cout << "### ERROR: No data file specified!" << std::endl;
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            break;
    }

    return retCode;
}