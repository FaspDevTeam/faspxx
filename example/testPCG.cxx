/*! \file testPCG.hxx
 *  \brief Test performance of PCG method
 *  \author Kailei Zhang, Chensong Zhang
 *  \date Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include "Timing.hxx"
#include "ReadData.hxx"
#include "MATUtil.hxx"
#include "Param.hxx"
#include "PCG.hxx"
#include "LOP.hxx"

int main()
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;

    // Read a CSR matrix from file
    INT row, col, nnz;
    std::vector<INT> rowPtr; // row pointer
    std::vector<INT> colInd; // col index
    std::vector<DBL> values; // matrix nnz

    timer.Start();

    try {
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

    VEC b(row, 0.0), x(col, 1.0);

    // Print problem size information
    std::cout << "  nrow = " << mat.GetRowSize()
              << ", ncol = " << mat.GetColSize()
              << ", nnz = "  << mat.GetNNZ() << std::endl;

    std::cout << "Reading Ax=b costs " << timer.Stop() << "ms" << std::endl;

    // Setup parameters
    IterParam param;
    param.SetOutLvl(PRINT_NONE);
    param.SetRelTol(1e-5);
    param.SetAbsTol(1e-8);
    param.SetMaxIter(200);
    param.Print();

    // Setup PCG class
    PCG pcg;
    pcg.Setup(mat, b, x, param);

    // Setup preconditioner
    LOP lop(row, col);
    pcg.SetupPCD(lop);

    // PCG solve
    timer.Start();
    retCode = pcg.Solve(mat, b,x,param);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

    // Clean up
    pcg.CleanPCD();
    pcg.Clean();

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/