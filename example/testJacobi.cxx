/*! \file    testBiCGStab.hxx
 *  \brief   Test performance of BiCGStab method
 *  \author  Kailei Zhang
 *  \date    Dec/12/2019
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
#include "Iter.hxx"

int main(int argc, char *args[]) {
    const char *mat_file = "../data/fdm_10X10.csr";
    const char *vec_file = "";
    const char *initial_guess = "";
    Output verbose = PRINT_NONE;
    const char *opts = "../data/multiple_sol.opts";
    const char *prefix = "-solver1";
    double resrel = 1e-6, resabs = 1e-8;
    int restart = 20, maxIter = 100, minIter = 0;

    Parameters params(argc, args);
    params.AddParam("-mat", "Left hand side of linear system", &mat_file);
    params.AddParam("-vec", "Right hand side of linear system", &vec_file);
    params.AddParam("-initial", "Initial guess of solution", &initial_guess);
    params.AddParam("-verbose", "Verbose level", &verbose);
    params.AddParam("-opts_file", "Solver parameter file", &opts);

    params.AddParam("-pref", "solver parameter prefix in file", &prefix);
    params.AddParam("-resrel", "Relative residual tolerance", &resrel);
    params.AddParam("-resabs", "Absolute residual tolerance", &resabs);
    params.AddParam("-restart", "Iteration restart number", &restart);
    params.AddParam("-maxIter", "Maximum iteration steps", &maxIter);
    params.AddParam("-minIter", "Minimum iteration steps", &minIter);
    params.Parse();
    params.Print();

    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;
    timer.Start();

    // Read matrix data file
    MAT mat;
    if ( (retCode = ReadMat(mat_file, mat)) < 0 ) return retCode;
    const INT nrow = mat.GetRowSize();
    const INT mcol = mat.GetColSize();

    // Read or generate right-hand side
    VEC b;
    if ( strcmp(vec_file, "") != 0 )
        ReadVEC(vec_file, b);
    else
        b.SetValues(nrow, 0.0);

    // Read or generate initial guess
    VEC x;
    if ( strcmp(initial_guess, "") != 0 )
        ReadVEC(initial_guess, x);
    else
        x.SetValues(mcol, 1.0);

    VEC y(b.GetSize());

    // Print problem size information
    std::cout << "Reading Ax = b costs " << timer.Stop() << "ms" << std::endl;

    // Setup PCG class
    Jacobi jac(mat,b);
    jac.SetMaxIter(maxIter);
    jac.SetRelTol(resrel);
    jac.SetAbsTol(resabs);
    jac.Setup(mat);

    // PCG solve
    timer.Start();
    retCode = jac.Solve(x, y);
    std::cout << "Solving linear system costs " << std::fixed
              << std::setprecision(2) << timer.Stop() << "ms" << std::endl;
    std::cout<<"Norm2 : "<<jac.GetNorm2()<<std::endl;
    std::cout<<"NormInf : "<<jac.GetInfNorm()<<std::endl;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/