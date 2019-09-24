/**
 * a function for reading files
 */

#ifndef READ_FILE_HXX
#define READ_FILE_HXX

#include "faspxx.hxx"
#include "mat.hxx"

FaspErrorCode ReadMtx(const char *filename,INT &row,INT &column,INT &nnz,
        std::vector<INT> &rowind,std::vector<INT> &colind,std::vector<DBL> &data);

void MtxtoCSRx(INT row,INT column,INT nnz,std::vector<INT> rowind,
        std::vector<INT> colind,std::vector<DBL> data,MAT &mat);

#endif //READ_FILE_HXX
