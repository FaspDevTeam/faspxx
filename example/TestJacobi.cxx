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

// Sample usages:
//   ./TestJacobi -maxIter 1000 -verbose 2

// FASPXX header files
#include "Iter.hxx"
#include "LOP.hxx"
#include "Param.hxx"
#include "ReadData.hxx"
#include "Timing.hxx"

int main(int argc, const char *args[])
{
    // User default parameters
    std::string parFile = "../../data/input.param";
    std::string matFile = "../../data/fdm_10X10.csr";
    std::string rhsFile, xinFile;

    // Read general parameters
    Parameters params(argc, args);
    params.AddParam("-mat", "Coefficient matrix A", &matFile);
    params.AddParam("-rhs", "Right-hand-side b", &rhsFile);
    params.AddParam("-xin", "Initial guess for iteration", &xinFile);
    params.AddParam("-par", "Solver parameter file", &parFile);

    // Set solver parameters
    SOLParams solParam;
    params.SetSOLParams(solParam);

    // Parse and print used parameters
    params.Parse();
    params.Print();

    GetWallTime timer;
    timer.Start();

    // Read matrix data file and exit if failed
    MAT         mat;
    FaspRetCode retCode = ReadMat(matFile.c_str(), mat);
    if (retCode < 0) return retCode;

    // Print problem size information
    const USI nrow = mat.GetRowSize(), mcol = mat.GetColSize();
    std::cout << "nrow: " << nrow << ", mcol: " << mcol << std::endl;

    // Read the right-hand side b; if not specified, use b = 0.0
    VEC b;
    b.SetValues(nrow, 0.0);
    if (strcmp(rhsFile.c_str(), "") != 0) ReadVEC(rhsFile.c_str(), b);

    // Read the initial guess x0; if not specified, use x0 = 1.0
    VEC x;
    x.SetValues(mcol, 1.0);
    if (strcmp(xinFile.c_str(), "") != 0) ReadVEC(xinFile.c_str(), x);

    timer.StopInfo("Reading Ax = b");

    // Setup solver parameters
    class Jacobi solver;
    solver.SetOutput(solParam.verbose);
    solver.SetMaxIter(solParam.maxIter);
    solver.SetMinIter(solParam.minIter);
    solver.SetRelTol(solParam.relTol);
    solver.SetAbsTol(solParam.absTol);
    solver.SetWeight(0.8);  // TODO: Read from cmd
    solver.Setup(mat);

    // Solve the linear system using Jacobi iteration
    timer.Start();
    retCode = solver.Solve(b, x);
    solver.PrintTime(timer.Stop());

    return retCode;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Dec/12/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/