/**
 * a test file
 */

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

    IterParam param;

    param.SetAbsTol(1e-8);
    param.SetMaxIter(200);
    param.SetOutLvl(PRINT_NONE);
    param.SetRelTol(1e-5);
    param.SetRestart(20);
    //param.Print();

    LOP lop(row,col);

    PCG pcg;
    pcg.Setup(mat,b,x,param);
    pcg.SetupPCD(lop);

    timer.Start();
    pcg.Solve(mat, b,x,param);
    std::cout << "FASPXX time : " << timer.Stop() << "s" << std::endl;
    pcg.CleanPCD();
    pcg.Clean();

    return 0;
}