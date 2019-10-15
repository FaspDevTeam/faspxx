/**
 * a header file about TODO partition
 */
#ifndef P_TODO_HXX
#define P_TODO_HXX

#include <fstream>
#include "RetCode.hxx"
#include <VEC.hxx>

FaspRetCode ReadCSRAll(const char *filename, INT &row, INT &col, INT &nnz,
                       std::vector<INT> &rowPtr, std::vector<INT> &colInd,
                       std::vector<DBL> &values) {
#if 0
    std::ifstream in(filename);
    in.seekg(0, std::ios::end);
    long long int length = in.tellg();
    in.seekg(0, std::ios::beg);
    char *buffer;
    try{
        buffer = new char[length];
    }catch(std::bad_alloc &ex){
        in.close();
        return FaspRetCode ::ERROR_ALLOC_MEM;
    }
    in.read(buffer, length);
    in.close();
#endif


    char *decimal = new char[16];
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

    col = row;
    long long int locate = 0;
    count=0;
    rowPtr.resize(row + 1);
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

    colInd.resize(nnz);
    values.resize(nnz);

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
        if (buffer[position] != '\n' && buffer[position]!=EOF) {
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

    return FaspRetCode ::SUCCESS;
}


#endif //P_TODO_HXX
