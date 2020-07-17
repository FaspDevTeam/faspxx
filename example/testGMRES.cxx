
#include <fstream>
#include "Param.hxx"
#include "Timing.hxx"
#include "ReadData.hxx"
#include "LOP.hxx"
#include "Iter.hxx"
#include "GMRES.hxx"
#include "FGMRES.hxx"

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

    // Read solver parameters
    SOLParams solParam;
    params.AddParam("-maxIter",  "Max iteration steps",         &solParam.maxIter);
    params.AddParam("-minIter",  "Min iteration steps",         &solParam.minIter);
    params.AddParam("-safeIter", "Safe-guard steps",            &solParam.safeIter);
    params.AddParam("-restart",  "Restart number",              &solParam.restart);
    params.AddParam("-resRel",   "Relative residual tolerance", &solParam.relTol);
    params.AddParam("-resAbs",   "Absolute residual tolerance", &solParam.absTol);
    params.AddParam("-verbose",  "Verbose level",               &solParam.verbose);

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

    // Read the right-hand-side b; if not specified, use b = 0.0
    VEC b;
    b.SetValues(nrow, 1.0);
    if ( strcmp(rhsFile.c_str(), "") != 0 ) ReadVEC(rhsFile.c_str(), b);

    timer.StopInfo("Reading Ax = b");

    // Setup preconditioner parameters
    Identity pc;  // pc = identity, no preconditioning used

    // Setup GMRES parameters
    class GMRES solver;
    solver.SetOutput(solParam.verbose);
    solver.SetMaxIter(solParam.maxIter);
    solver.SetMinIter(solParam.minIter);
    solver.SetSafeIter(solParam.safeIter);
    solver.SetRestart(solParam.restart);
    solver.SetRelTol(solParam.relTol);
    solver.SetAbsTol(solParam.absTol);
    solver.SetMaxMinRestart(solParam.restart, 5);
    solver.SetPC(pc);

    solver.Setup(mat);

    VEC x;
    x.SetValues(mcol, 1.0); // If initial guess not specified, set x0 = 1.0
    if ( strcmp(xinFile.c_str(), "") != 0 ) ReadVEC(xinFile.c_str(), x);

    // Solve the linear system using GMRES with right preconditioner
    timer.Start();
    retCode = solver.RSolve(b, x);
    solver.PrintTime(timer.Stop());
    std::cout << "Right preconditioned GMRES: residual = " << solver.GetNorm2()
              << "\n--------------------------------------------" << std::endl;
    solver.Clean();

    x.SetValues(mcol, 1.0); // If initial guess not specified, set x0 = 1.0
    if ( strcmp(xinFile.c_str(), "") != 0 ) ReadVEC(xinFile.c_str(), x);

    // Solve the linear system using GMRES with right preconditioner
    timer.Start();
    retCode = solver.LSolve(b, x);
    solver.PrintTime(timer.Stop());
    std::cout << "Left preconditioned GMRES: residual = " << solver.GetNorm2()
              << "\n--------------------------------------------" << std::endl;
    solver.Clean();

    // Setup FGMRES parameters
    class FGMRES fsolver;
    fsolver.SetOutput(solParam.verbose);
    fsolver.SetMaxIter(solParam.maxIter);
    fsolver.SetMinIter(solParam.minIter);
    fsolver.SetSafeIter(solParam.safeIter);
    fsolver.SetRestart(solParam.restart);
    fsolver.SetRelTol(solParam.relTol);
    fsolver.SetAbsTol(solParam.absTol);
    fsolver.SetMaxMinRestart(solParam.restart, 5);
    fsolver.SetPC(pc);

    fsolver.Setup(mat);

    x.SetValues(mcol, 1.0); // If initial guess not specified, set x0 = 1.0
    if ( strcmp(xinFile.c_str(), "") != 0 ) ReadVEC(xinFile.c_str(), x);

    // Solve the linear system using GMRES with right preconditioner
    timer.Start();
    retCode = fsolver.Solve(b, x);
    fsolver.PrintTime(timer.Stop());
    std::cout << "Right preconditioned FGMRES: residual = " << fsolver.GetNorm2()
              << "\n--------------------------------------------" << std::endl;
    fsolver.Clean();

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/