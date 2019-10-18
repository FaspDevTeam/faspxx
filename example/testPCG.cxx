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
#include "Param.hxx"
#include "PCG.hxx"
#include "LOP.hxx"
#include "ReadCommand.hxx"

int main(int argc,char *args[])
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;
    MAT mat;
    VEC b,x;
    INT row,col,nnz;

    std::cout<<"argc : "<<argc<<std::endl;
    for(int j=0;j<argc;j++)
        std::cout<<"args["<<j<<"] : "<<args[j]<<std::endl;

    timer.Start();
    /// read matrix, rhs and inital solution
    if((retCode=ReadCommad(argc,args,mat,b,x))<0)
        return retCode;

    std::cout<<"Reading Ax = b costs "<<timer.Stop()<<"ms"<<std::endl;
    /*
     * because of short of rhs data and initial solution data,
     * so set 'b' and 'x' as follows:
     */
    row=mat.GetRowSize();
    col=mat.GetColSize();
    nnz=mat.GetNNZ();
    b.SetValues(row,0.0);
    x.SetValues(col,1.0);

    // Print problem size information
    std::cout << "  nrow = " << row
              << ", ncol = " << col
              << ", nnz = "  << nnz << std::endl;

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
