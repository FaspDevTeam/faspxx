/*! \file    testGMRES.cxx
 *  \brief   Test performance of GMRES method
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Standard header files
#include <fstream>

// FASPXX header files
#include "FGMRES.hxx"
#include "GMRES.hxx"
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
    if (retCode < 0) return retCode;

    // Print problem size information
    const USI nrow = mat.GetRowSize(), mcol = mat.GetColSize();
    std::cout << "nrow: " << nrow << ", mcol: " << mcol << std::endl;

    // Read the right-hand-side b; if not specified, use b = 0.0
    VEC b;
    b.SetValues(nrow, 1.0);
    if (strcmp(rhsFile.c_str(), "") != 0) ReadVEC(rhsFile.c_str(), b);

    timer.StopInfo("Reading Ax = b");

    // Setup preconditioner parameters
    Identity pcd; // no preconditioning used

    // Setup GMRES parameters
    class GMRES solver;
    solver.SetOutput(solParam.verbose);
    solver.SetMaxIter(solParam.maxIter);
    solver.SetMinIter(solParam.minIter);
    solver.SetSavIter(solParam.savIter);
    solver.SetRestart(solParam.restart);
    solver.SetRelTol(solParam.relTol);
    solver.SetAbsTol(solParam.absTol);
    solver.SetMaxMinRestart(solParam.restart, 5);
    solver.SetupPCD(pcd);
    solver.Setup(mat);

    VEC x;
    x.SetValues(mcol, 1.0); // If initial guess not specified, set x0 = 1.0
    if (strcmp(xinFile.c_str(), "") != 0) ReadVEC(xinFile.c_str(), x);

    // Solve the linear system using GMRES with right preconditioner
    timer.Start();
    retCode = solver.Solve(b, x);
    solver.PrintTime(timer.Stop());
    std::cout << "Right preconditioned GMRES: residual = " << solver.GetNorm2()
              << "\n---------------------------------------------" << std::endl;
    solver.Clean();

    x.SetValues(mcol, 1.0); // If initial guess not specified, set x0 = 1.0
    if (strcmp(xinFile.c_str(), "") != 0) ReadVEC(xinFile.c_str(), x);

    // Solve the linear system using GMRES with right preconditioner
    timer.Start();
    solver.SetLeftPrecond();
    retCode = solver.Solve(b, x);
    solver.PrintTime(timer.Stop());
    std::cout << "Left preconditioned GMRES: residual = " << solver.GetNorm2()
              << "\n---------------------------------------------" << std::endl;
    solver.Clean();

    // Setup FGMRES parameters
    class FGMRES fsolver;
    fsolver.SetOutput(solParam.verbose);
    fsolver.SetMaxIter(solParam.maxIter);
    fsolver.SetMinIter(solParam.minIter);
    fsolver.SetSavIter(solParam.savIter);
    fsolver.SetRestart(solParam.restart);
    fsolver.SetRelTol(solParam.relTol);
    fsolver.SetAbsTol(solParam.absTol);
    fsolver.SetMaxMinRestart(solParam.restart, 5);
    fsolver.SetupPCD(pcd);
    fsolver.Setup(mat);

    x.SetValues(mcol, 1.0); // If initial guess not specified, set x0 = 1.0
    if (strcmp(xinFile.c_str(), "") != 0) ReadVEC(xinFile.c_str(), x);

    // Solve the linear system using GMRES with right preconditioner
    timer.Start();
    retCode = fsolver.Solve(b, x);
    fsolver.PrintTime(timer.Stop());
    std::cout << "Right preconditioned FGMRES: residual = " << fsolver.GetNorm2()
              << "\n---------------------------------------------" << std::endl;
    fsolver.Clean();

    return retCode;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Oct/12/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/