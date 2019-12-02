/*! \file    testPCG.hxx
 *  \brief   Test performance of PCG method
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include "Timing.hxx"
#include "ReadData.hxx"
#include "Param.hxx"
#include "LOP.hxx"
#include "CG.hxx"
#include "Iter.hxx"

int main(int argc, char *args[]) {
    const char *mat_file = "../data/fdm_10X10.csr";
    const char *vec_file = "";
    const char *initial_guess = "";
    Output print_level = PRINT_NONE;
    const char *opts = "../data/multiple_sol.opts";
    const char *prefix = "-solver1";
    DBL resrel=1e-4;
    DBL resabs=1e-8;
    INT restart=20, maxIter=100;

    Parameters params(argc, args);
    params.AddParam("-mat", "Left hand side of linear system", &mat_file);
    params.AddParam("-vec", "Right hand side of linear system", &vec_file);
    params.AddParam("-initial", "Initial guess of solution", &initial_guess);
    params.AddParam("-level", "print level", &print_level);
    params.AddParam("-opts_file", "Solver parameter file", &opts);
    params.AddParam("-pref", "solver parameter prefix in file", &prefix);
    params.AddParam("-resrel", "residual relative tolerance", &resrel);
    params.AddParam("-resabs", "residual relative tolerance", &resabs);
    params.AddParam("-restart", "restart", &restart);
    params.AddParam("-maxIter", "maximum iteration", &maxIter);
    params.Parse();

    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;
    timer.Start();

    // Read matrix data file
    MAT mat;
    if ((retCode = ReadMat(mat_file, mat)) < 0) return retCode;
    INT row = mat.GetRowSize();
    INT col = mat.GetColSize();
    INT nnz = mat.GetNNZ();

    // Read or generate right-hand side
    VEC b, x;
    if (strcmp(vec_file, "") != 0)
        ReadVEC(vec_file, b);
    else
        b.SetValues(row, 0.0);

    // Read or generate initial guess
    if (strcmp(initial_guess, "") != 0)
        ReadVEC(initial_guess, x);
    else
        x.SetValues(col, 1.0);

    // Print problem size information
    std::cout << "  nrow = " << row
              << ", ncol = " << col
              << ", nnz = " << nnz << std::endl;
    std::cout << "Reading Ax = b costs " << timer.Stop() << "ms" << std::endl;

    // Setup PCG class
    CG pcg;
    pcg.SetMaxIter(maxIter);
    pcg.SetRelTol(resrel);
    pcg.SetAbsTol(resabs);
    pcg.SetRestart(restart);
    pcg.SetPrtLvl(print_level);
    // pcg.SetOptionsFromFile(opts, prefix);
    pcg.Setup(mat);

    params.Print();

    // Setup preconditioner
    Identity pc;
    pcg.SetPC(&pc);

    // PCG solve
    timer.Start();
    retCode = pcg.Solve(b, x);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;
    std::cout << "Number of iterations : " << pcg.GetIterations() << std::endl;
    std::cout << "Norm2 of residual    : " << pcg.GetNorm2()      << std::endl;
    std::cout << "NormInf of residual  : " << pcg.GetInfNorm()    << std::endl;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/