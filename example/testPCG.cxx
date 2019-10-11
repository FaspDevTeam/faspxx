/**
 * a test file
 */

#include <fstream>
#include <iostream>
#include "Timing.hxx"
#include "ReadData.hxx"
#include "LOP.hxx"
#include "MATUtil.hxx"
#include "PCG.hxx"
#include "Param.hxx"

int main()
{
    GetWallTime timer;

    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw

    // TODO: use this param later
    IterParam param;
    param.Print();

    // Read a CSR matrix from file
    INT row, col, nnz;
    std::vector<INT> rowPtr; // row pointer
    std::vector<INT> colInd; // col index
    std::vector<DBL> values; // matrix nnz

    try {
        //retCode = ReadCSR("../data/fdm_10X10.csr", row, col, nnz,
        //                  rowPtr, colInd, values);
        retCode = ReadCSR("../data/fdm_1023X1023.csr", row, col, nnz,
                          rowPtr, colInd, values);
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Sort each row in ascending order
    try {
        retCode = SortCSRRow(row, col, nnz, rowPtr, colInd, values);
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Convert a MTX matrix to MAT
    MAT mat;
    try {
        retCode = CSRtoMAT(row, col, nnz, values, colInd, rowPtr, mat);
        if ( retCode < 0 )
            throw( FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__) );
    }
    catch ( FaspRunTime& ex ) {
        ex.LogExcep();
        return ex.errorCode;
    }
    std::cout << "  nrow = " << mat.GetRowSize()
              << ", ncol = " << mat.GetColSize()
              << ", nnz = "  << mat.GetNNZ() << std::endl;

    VEC b(row, 0.0), x(col, 1.0);

    PCGInputParam iparam;
    PCGOutputParam oparam;
    iparam.maxIter=200;
    iparam.printLevel=PRINT_MORE;
    iparam.absTol=1e-8;
    iparam.relTol=1e-5;
    iparam.restart=20;

    LOP lop(row,col);

    PCG pcg;
    pcg.SetUp(iparam);
    pcg.SetUpPCD(lop);

    timer.Start();
    pcg.Start(mat, b,x,STOP_REL_RES,oparam);
    std::cout << "FASPXX time : " << timer.Stop() << std::endl;

    return 0;
}