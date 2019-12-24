/*! \file    testJacobi.cxx
 *  \brief   Test performance of Jacobi method
 *  \author  Kailei Zhang
 *  \date    Dec/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */


#include <iostream>
#include <string>
#include "Timing.hxx"
#include "ReadData.hxx"
#include "Param.hxx"
#include "LOP.hxx"
#include "Iter.hxx"

int main(int argc, char *args[]) {
    std::string mat_file = "../data/fdm_10X10.csr";
    std::string vec_file = "";
    std::string initial_guess = "";
    Output verbose = PRINT_NONE;
    std::string opts = "../data/multiple_sol.opts";
    std::string prefix = "-solver1";
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
    //params.Print();

    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;
    timer.Start();

    // Read matrix data file
    MAT mat;
    if ( (retCode = ReadMat(mat_file.c_str(), mat)) < 0 ) return retCode;
    const INT nrow = mat.GetRowSize();
    const INT mcol = mat.GetColSize();
    std::cout<<"nrow : "<<nrow<<std::endl;
    std::cout<<"mcol : "<<mcol<<std::endl;
    // Read or generate right-hand side
    VEC b;
    if ( strcmp(vec_file.c_str(), "") != 0 )
        ReadVEC(vec_file.c_str(), b);
    else
        b.SetValues(nrow, 0.0);

    // Read or generate initial guess
    VEC x;
    if ( strcmp(initial_guess.c_str(), "") != 0 )
        ReadVEC(initial_guess.c_str(), x);
    else
        x.SetValues(mcol, 1.0);

    VEC y(mcol);
    // Print problem size information
    std::cout << "Reading Ax = b costs " << timer.Stop() << "ms" << std::endl;

    // Setup preconditioner
    Identity pc;

    // Setup PCG class
    Jacobi jac(mat,b,1.0);
    jac.SetOutput(verbose);
    jac.SetMaxIter(maxIter);
    jac.SetMinIter(minIter);
    jac.SetRelTol(resrel);
    jac.SetAbsTol(resabs);
    jac.SetRestart(restart);

    // PCG solve
    timer.Start();

    retCode = jac.Solve(x, y);
    std::cout << "Solving linear system costs " << std::fixed
              << std::setprecision(2) << timer.Stop() << "ms" << std::endl;

    std::cout<<"NormInf : "<<jac.GetInfNorm()<<std::endl;
    std::cout<<"Norm2   : "<<jac.GetNorm2()<<std::endl;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/