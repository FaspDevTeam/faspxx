/*! \file    testKrylov.cxx
 *  \brief   Test performance of Krylov method
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Dec/23/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Sample usages:
//   ./testKrylov -mat ../data/fdm_1023X1023.csr -maxIter 200 -minIter 0 -algName cg
//   ./testKrylov -mat ../data/fdm_1023X1023.csr -maxIter 200 -algName bicgstab

#include <iostream>
#include <string>
#include "Timing.hxx"
#include "ReadData.hxx"
#include "Param.hxx"
#include "LOP.hxx"
#include "Iter.hxx"
#include "Krylov.hxx"

int main(int argc, const char *args[])
{
    // User default parameters
    std::string parFile = "../data/input.param";
    std::string matFile = "../data/fdm_10X10.csr";
    std::string rhsFile, x0File;

    // Read in general parameters
    Parameters params(argc, args);
    params.AddParam("-mat", "Coefficient matrix A", &matFile);
    params.AddParam("-rhs", "Right-hand-side b", &rhsFile);
    params.AddParam("-x0", "Initial guess for iteration", &x0File);
    params.AddParam("-par", "Solver parameter file", &parFile);

    // Read in solver parameters
    SOLParams solParam;
    params.AddParam("-maxIter", "Max iteration steps", &solParam.maxIter);
    params.AddParam("-minIter", "Min iteration steps", &solParam.minIter);
    params.AddParam("-safeIter", "Safe-guard steps", &solParam.safeIter);
    params.AddParam("-restart", "Restart number", &solParam.restart);
    params.AddParam("-resRel", "Relative residual tolerance", &solParam.relTol);
    params.AddParam("-resAbs", "Absolute residual tolerance", &solParam.absTol);
    params.AddParam("-algName", "Iterative solver type", &solParam.algName);
    params.AddParam("-verbose", "Verbose level", &solParam.verbose);

    // Parse and print used parameters
    params.Parse();
    params.Print();

    // Initiate timer
    GetWallTime timer;
    timer.Start();

    // Read matrix data file and exit if failed
    MAT mat;
    FaspRetCode retCode = ReadMat(matFile.c_str(), mat);
    if ( retCode < 0 ) return retCode;

    // Print problem size information
    const INT nrow = mat.GetRowSize(), mcol = mat.GetColSize();
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

    // Setup preconditioner parameters
    Identity pc;  // pc = identity, no preconditioning used

    // Solve the linear system using a general interface for Krylov methods
    timer.Start();
    retCode = Krylov(mat, b, x, pc, solParam);
    std::cout << "Solving linear system costs " << std::fixed
              << std::setprecision(2) << timer.Stop() << "ms" << std::endl;

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/