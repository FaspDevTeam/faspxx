/*! \file    testCG.hxx
 *  \brief   Test performance of CG method
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include <string>
#include "Param.hxx"
#include "Timing.hxx"
#include "ReadData.hxx"
#include "LOP.hxx"
#include "Iter.hxx"
#include "CG.hxx"

int main(int argc, const char *args[])
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw

    // User default parameters
    std::string parFile = "../data/multiple_sol.param";
    std::string matFile = "../data/fdm_10X10.csr";
    std::string rhsFile, x0File;
    Identity    pc; // no preconditioning
    SOLParams   solParam;

    // Read in parameters
    Parameters params(argc, args);
    params.AddParam("-mat", "Left hand side of linear system", &matFile);
    params.AddParam("-rhs", "Right hand side of linear system", &rhsFile);
    params.AddParam("-x0", "Initial guess of solution", &x0File);
    params.AddParam("-par", "Solver parameter file", &parFile);
    params.AddParam("-maxIter", "Maximum iteration steps", &solParam.maxIter);
    params.AddParam("-minIter", "Minimum iteration steps", &solParam.minIter);
    params.AddParam("-safeIter", "Safe-guard iteration", &solParam.safeIter);
    params.AddParam("-restart", "Iteration restart number", &solParam.restart);
    params.AddParam("-resRel", "Relative residual tolerance", &solParam.relTol);
    params.AddParam("-resAbs", "Absolute residual tolerance", &solParam.absTol);
    params.AddParam("-verbose", "Verbose level", &solParam.verbose);
    params.Parse();
    params.Print();

    GetWallTime timer;
    timer.Start();

    // Read matrix data file
    MAT mat;
    if ( (retCode = ReadMat(matFile.c_str(), mat)) < 0 ) return retCode;

    // Print problem size information
    const INT nrow = mat.GetRowSize();
    const INT mcol = mat.GetColSize();
    std::cout << "nrow: " << nrow << ", mcol: " << mcol << std::endl;

    // Read the right-hand side b; if not specified, use b = 0.0
    VEC b;
    b.SetValues(nrow, 0.0);
    if ( strcmp(rhsFile.c_str(), "") != 0 ) ReadVEC(rhsFile.c_str(), b);

    // Read the initial guess x0; if not specified, use x0 = 1.0
    VEC x;
    x.SetValues(mcol, 1.0);
    if ( strcmp(x0File.c_str(), "") != 0 ) ReadVEC(x0File.c_str(), x);

    std::cout << "Reading Ax = b costs " << timer.Stop() << "ms" << std::endl;

    // Setup PCG parameters
    class CG solver;
    solver.SetMaxIter(solParam.maxIter);
    solver.SetMinIter(solParam.minIter);
    solver.SetSafeIter(solParam.safeIter);
    solver.SetRestart(solParam.restart);
    solver.SetRelTol(solParam.relTol);
    solver.SetAbsTol(solParam.absTol);
    solver.SetOutput(solParam.verbose);
    solver.SetPC(&pc);
    solver.Setup(mat);

    // Solve the linear system using CG
    timer.Start();
    retCode = solver.Solve(b, x);
    std::cout << "Solving linear system costs " << std::fixed
              << std::setprecision(2) << timer.Stop() << "ms" << std::endl;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/