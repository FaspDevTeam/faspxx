/*! \file    testAMG.cxx
 *  \brief   Test performance of AMG method
 *  \author  Chensong Zhang
 *  \date    Sep/12/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Sample usages:
//   ./testAMG -maxIter 200 -minIter 100
//   ./testAMG -maxIter 200 -minIter 0 -mat ../data/fdm_1023X1023.csr -verbose 3

// FASPXX header files
#include "Iter.hxx"
#include "LOP.hxx"
#include "MG.hxx"
#include "Param.hxx"
#include "ReadData.hxx"
#include "Timing.hxx"

int main(int argc, const char* args[])
{
    // User default parameters
    std::string parFile = "../data/input.param";
    std::string matFile = "../data/fdm_10X10.csr";
    std::string rhsFile, xinFile;

    // Read general parameters
    Parameters params(argc, args);
    params.AddParam("-par", "Solver parameter file", &parFile);
    params.AddParam("-mat", "Coefficient matrix A", &matFile);
    params.AddParam("-rhs", "Right-hand-side b", &rhsFile);
    params.AddParam("-xin", "Initial guess for iteration", &xinFile);

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
    if (retCode < 0) FASPXX_ABORT("Failed to read matrix file!");

    // Print problem size information
    const USI nrow = mat.GetRowSize(), mcol = mat.GetColSize();
    std::cout << "nrow: " << nrow << ", mcol: " << mcol << std::endl;

    // Read the right-hand-side b; if not specified, use b = 0.0
    VEC b;
    b.SetValues(nrow, 0.0);
    if (strcmp(rhsFile.c_str(), "") != 0) ReadVEC(rhsFile.c_str(), b);

    // Read the initial guess x0; if not specified, use x0 = 1.0
    VEC x;
    x.SetValues(mcol, 1.0);
    if (strcmp(xinFile.c_str(), "") != 0) ReadVEC(xinFile.c_str(), x);

    timer.StopInfo("Reading Ax = b");

    // Setup solver parameters
    class MG<class MAT> solver;
    solver.SetOutput(solParam.verbose);
    solver.SetMaxIter(solParam.maxIter);
    solver.SetMinIter(solParam.minIter);
    solver.SetSavIter(solParam.savIter);
    solver.SetRestart(solParam.restart);
    solver.SetRelTol(solParam.relTol);
    solver.SetAbsTol(solParam.absTol);

    // Setup HL levels
    const unsigned numLevels = 3;            // number of coarse levels
    class Jacobi   smoothers[numLevels];     // smoothers at coarse levels
    class Jacobi   coarseSolvers[numLevels]; // solver at coarse levels

    // Transfer operators
    IdentityMatrix tranOpers[numLevels];
    for (USI i = 0; i < numLevels; ++i) tranOpers[i] = IdentityMatrix(mat.GetRowSize());

    // Smoothers and coarse solvers
    solver.SetupALL(mat, numLevels);
    for (USI i = 0; i < numLevels; ++i) {
        smoothers[i].Setup(mat);
        coarseSolvers[i].Setup(mat);
        solver.SetupLevel(mat, i, &tranOpers[i], &smoothers[i], &coarseSolvers[i]);
    }

    // Solve the linear system using AMG
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
/*  Chensong Zhang      Sep/12/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/