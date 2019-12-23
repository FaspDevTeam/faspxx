/*! \file    testJacobi.cxx
 *  \brief   Test performance of Jacobi method
 *  \author  Kailei Zhang
 *  \date    Dec/23/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */


#include <iostream>
#include <string>
#include "CG.hxx"
#include "BiCGStab.hxx"
#include "Timing.hxx"
#include "ReadData.hxx"
#include "Param.hxx"
#include "LOP.hxx"
#include "Iter.hxx"

int main(int argc, char *args[])
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw

    // User default parameters
    Output verbose = PRINT_NONE;
    std::string parFile = "../data/multiple_sol.param";
    std::string matFile = "../data/fdm_10X10.csr";
    std::string rhsFile = "";
    std::string x0File  = "";
    std::string solAlgm = "CG";
    double resRel = 1e-6, resAbs = 1e-8;
    int maxIter = 100, minIter = 0, restart = 20;
    Identity pc; // no preconditioner

    // Command-line parameters
    Parameters params(argc, args);
    params.AddParam("-par", "Solver parameter file", &parFile);
    params.AddParam("-mat", "Left hand side of linear system", &matFile);
    params.AddParam("-rhs", "Right hand side of linear system", &rhsFile);
    params.AddParam("-x0", "Initial guess of solution", &x0File);
    params.AddParam("-solAlgm", "Iterative solver type", &solAlgm);
    params.AddParam("-maxIter", "Maximum iteration steps", &maxIter);
    params.AddParam("-minIter", "Minimum iteration steps", &minIter);
    params.AddParam("-restart", "Iteration restart number", &restart);
    params.AddParam("-resRel", "Relative residual tolerance", &resRel);
    params.AddParam("-resAbs", "Absolute residual tolerance", &resAbs);
    params.AddParam("-verbose", "Verbose level", &verbose);
    params.Parse();

    params.Print();

    // Initiate timer
    GetWallTime timer;
    timer.Start();

    // Read matrix data file
    MAT mat;
    if ( (retCode = ReadMat(matFile.c_str(), mat)) < 0 ) return retCode;
    const INT nrow = mat.GetRowSize();
    const INT mcol = mat.GetColSize();
    std::cout << "nrow: " << nrow << ", mcol: " << mcol << std::endl;

    // Read the right-hand side b; if not specified, use b = 0
    VEC b;
    if ( strcmp(rhsFile.c_str(), "") != 0 )
        ReadVEC(rhsFile.c_str(), b);
    else
        b.SetValues(nrow, 0.0);

    // Read the initial guess x0; if not specified, use x0 = 0
    VEC x;
    if ( strcmp(x0File.c_str(), "") != 0 )
        ReadVEC(x0File.c_str(), x);
    else
        x.SetValues(mcol, 1.0);

    // Print problem size information
    std::cout << "Reading Ax = b costs " << timer.Stop() << "ms" << std::endl;

    // Change all letters of a string to lowercase
    for ( char & c : solAlgm ) c = tolower(c);

    if ( solAlgm == "cg" ) {
        // Setup PCG method
        CG solver;
        solver.SetOutput(verbose);
        solver.SetMaxIter(maxIter);
        solver.SetMinIter(minIter);
        solver.SetRestart(restart);
        solver.SetRelTol(resRel);
        solver.SetAbsTol(resAbs);
        solver.SetPC(&pc);
        solver.Setup(mat);
        solver.PrintParam();

        // PCG solve
        timer.Start();
        retCode = solver.Solve(b, x);
        std::cout << "Solving linear system costs " << std::fixed
                  << std::setprecision(2) << timer.Stop() << "ms" << std::endl;
    }
    else if ( solAlgm == "bicgstab" ) {
        // Setup BiCGStab method
        BiCGStab solver;
        solver.SetOutput(verbose);
        solver.SetMaxIter(maxIter);
        solver.SetMinIter(minIter);
        solver.SetRestart(restart);
        solver.SetRelTol(resRel);
        solver.SetAbsTol(resAbs);
        solver.SetPC(&pc);
        solver.Setup(mat);
        solver.PrintParam();

        // BiCGStab solve
        timer.Start();
        retCode = solver.Solve(b, x);
        std::cout << "Solving linear system costs " << std::fixed
                  << std::setprecision(2) << timer.Stop() << "ms" << std::endl;
    }

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/