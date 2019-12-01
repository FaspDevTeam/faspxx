/**
 * a test file
 */

#include <iostream>
#include "ReadData.hxx"
#include "Param.hxx"
#include "Timing.hxx"
#include "PCD.hxx"
#include "BiCGStab.hxx"

using namespace std;

int main(int argc, char *argv[]) {
    const char *mat_file = "../data/fdm_10X10.csr";
    const char *vec_file = "";
    const char *initial_guess = "";
    //bool print_level = false;
    PRTLVL print_level = PRINT_MORE;
    const char *opts = "../data/multiple_sol.opts";
    const char *prefix = "-solver1";
    DBL resrel=1e-4;
    DBL resabs=1e-8;
    INT restart=20, maxIter=100;

    Parameters params(argc, argv);
    params.AddParam(&mat_file, "-mat", "Left hand side of linear system");
    params.AddParam(&vec_file, "-vec", "Right hand side of linear system");
    params.AddParam(&initial_guess, "-initial", "Initial guess of solution");
    params.AddParam(&print_level, "-level", "print level");
    params.AddParam(&opts, "-opts_file", "Solver parameter file");
    params.AddParam(&prefix, "-pref", "solver parameter prefix in file");
    params.AddParam(&resrel, "-resrel", "residual relative tolerance");
    params.AddParam(&resabs, "-resabs", "residual relative tolerance");
    params.AddParam(&restart, "-restart", "restart");
    params.AddParam(&maxIter, "-maxIter", "maximum iteration");
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
    BiCGStab bi;
    bi.SetMaxIter(maxIter);
    bi.SetRelTol(resrel);
    bi.SetAbsTol(resabs);
    bi.SetRestart(restart);
    bi.SetPrtLvl(print_level);
    // pcg.SetOptionsFromFile(opts, prefix);
    bi.Setup(mat);

    params.PrintParams(cout<<"\nhhhhhhhhhhhhhhhhhhhhhhhh\n");
    // Setup preconditioner

    IdentityPC pc;
    bi.SetPC(&pc);

    // PCG solve
    timer.Start();
    retCode = bi.Solve(b, x);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

    return retCode;
}