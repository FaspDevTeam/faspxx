/*! \file    testPCG.hxx
 *  \brief   Test performance of PCG method
 *  \author  Chensong Zhang
 *  \date    Feb/15/2020
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Sample usages:
//   ./testPCG -maxIter 200 -minIter 0 -pcIter 1
//   ./testPCG -maxIter 200 -minIter 200 -mat ../data/fdm_1023X1023.csr -pcIter 1

// FASPXX header files
#include "Param.hxx"
#include "Timing.hxx"
#include "ReadData.hxx"
#include "LOP.hxx"
#include "Iter.hxx"
#include "CG.hxx"

int main(int argc, const char *args[])
{
    // User default parameters
    std::string parFile = "../data/input.param";
    std::string matFile = "../data/fdm_10X10.csr";
    std::string rhsFile, xinFile;

    // Read general parameters
    Parameters params(argc, args);
    params.AddParam("-par",      "Solver parameter file",       &parFile);
    params.AddParam("-mat",      "Coefficient matrix A",        &matFile);
    params.AddParam("-rhs",      "Right-hand-side b",           &rhsFile);
    params.AddParam("-xin",      "Initial guess for iteration", &xinFile);

    // Set solver and preconditioner parameters
    SOLParams solParam, pcParam;
    params.SetSOLParams(solParam, pcParam);

    // Parse and print used parameters
    params.Parse();
    params.Print();

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
    if ( strcmp(xinFile.c_str(), "") != 0 ) ReadVEC(xinFile.c_str(), x);

    timer.StopInfo("Reading Ax = b");

    // Setup preconditioner parameters
    class Jacobi pc;
    pc.SetOutput(PRINT_NONE);
    pc.SetWeight(pcParam.weight);   // weight for weighted Jacobi
    pc.SetMaxIter(pcParam.maxIter); // number of iterations
    pc.SetMinIter(pcParam.maxIter); // for preconditioning, use minIter = maxIter!
    pc.Setup(mat); // setup preconditioner: a different matrix could be used!

    // Setup solver parameters
    class CG solver;
    solver.SetOutput(solParam.verbose);
    solver.SetMaxIter(solParam.maxIter);
    solver.SetMinIter(solParam.minIter);
    solver.SetSafeIter(solParam.safeIter);
    solver.SetRestart(solParam.restart);
    solver.SetRelTol(solParam.relTol);
    solver.SetAbsTol(solParam.absTol);
    solver.SetPC(pc);
    solver.Setup(mat);

    // Solve the linear system using CG
    timer.Start();
    retCode = solver.Solve(b, x);
    solver.PrintTime(timer.Stop());

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/