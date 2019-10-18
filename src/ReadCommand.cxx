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
    char *buffer, decimal[128];
    long long int length, position;
    INT count, locate, len;

    std::ifstream in(filename); 
    if (in.is_open()) {
        retCode = FaspRetCode::ERROR_OPEN_FILE;
        return retCode;
    }
    in.seekg(0, std::ios::end);
    length = in.tellg();
    in.seekg(0, std::ios::beg);
    try {
        buffer = new char[length];
    } catch (std::bad_alloc &ex) {
        in.close();
        retCode = FaspRetCode::ERROR_ALLOC_MEM;
        return retCode;
    }
    in.read(buffer, length);
    in.close();

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

    rhs.SetValues(len, 0.0);

    locate = 0;
    while (1) {
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

    delete[] buffer;
}

// Todo: This is already define somewhere else
/// convert CSR format to CSRx matrix
FaspRetCode CSRToMAT(char *filename, MAT &mat) {
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
                        MAT& mat, VEC& rhs, VEC& initGuess) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    switch (argc) {
        case 2:
            retCode = CSRToMAT(args[1], mat);
            break;
        case 3:
            retCode = CSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadRHS(args[2], rhs);
            break;
        case 4:
            retCode = CSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadRHS(args[2], rhs);
            if (retCode < 0) break;
            retCode = ReadRHS(args[3], initGuess);
            break;
        default:
            std::cout << "### ERROR: No data file specified!" << std::endl;
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            break;
    }

    return retCode;
}