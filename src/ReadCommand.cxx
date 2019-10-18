/**
 * a source file about reading command parameters
 */

#include "ReadCommand.hxx"
#include "ReadData.hxx"
#include "MATUtil.hxx"
#include <fstream>

using namespace std;

/// read rhs or inital solution
FaspRetCode ReadRhs(char *filename, VEC &rhs) {
    FaspRetCode retCode;
    char *buffer, decimal[16];
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

FaspRetCode ReadCommad(unsigned int argc, char *args[], MAT &mat, VEC &rhs, VEC
&initGuess) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;
    switch (argc) {
        case 1:
            std::cout<<" ### ERROR: No data file entered!"<<std::endl;
            std::cout<<" ### Usage: ./*.exe MatrixDataFile"<<std::endl;
            retCode=FaspRetCode ::ERROR_INPUT_PAR;
            return retCode;
        case 2:
            retCode = CSRToMAT(args[1], mat);
            return retCode;
        case 3:
            retCode = CSRToMAT(args[1], mat);
            if (retCode < 0)
                return retCode;
            retCode = ReadRhs(args[2], rhs);
            return retCode;
        case 4:
            retCode = CSRToMAT(args[1], mat);
            if (retCode < 0)
                return retCode;
            retCode = ReadRhs(args[2], rhs);
            if (retCode < 0)
                return retCode;
            retCode = ReadRhs(args[3], initGuess);
            return retCode;
        default:
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            return retCode;
    }
}