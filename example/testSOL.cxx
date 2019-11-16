
#include <cassert>
#include <iostream>
using namespace std;

#include "../src/SOL.hxx"
#include "../src/Parameters.hxx"
#include "../src/Timing.hxx"
#include "../src/MAT.hxx"
#include "../src/ReadData.hxx"
#include "../src/ReadCommand.hxx"
#include "../src/PCG.hxx"

void Test_Parameter1()
{
    // ---------------- 3种输入参数的优先级: 命令行参数 > 运行时参数 > 文件参数? -------------------
    // 模拟user在C++源码里面给定的参数
    bool bool_param = 0;
    int  int_param = 11;
    double double_param = 3.14159;
    const char* char_param = "running_parameters";

    // 模拟用户输入的文件参数
    const char* params_file = "./parameters_file";

    // 模拟命令行输入参数
    int _argc = 11;
    char* _argv[11] = {"execute_file",
                       "-bool_param", "TRUE",
                       "-int_param", "22",
                       "-double_param", "1.41414",
                       "-char_param", "commandline_parameters",
                       "-faspxx_opts", "/home/faspxx_file_parameters.txt"};

    assert(bool_param == false);
    assert(int_param == 11);
    assert(double_param == 3.14159);
    assert(char_param == "running_parameters");
    assert(params_file == "./parameters_file");

    Parameters params(_argc, _argv);
    params.AddParam(&bool_param, "-bool_param", "bool param help");
    params.AddParam(&int_param, "-int_param", "int param help");
    params.AddParam(&double_param, "-double_param", "double param help");
    params.AddParam(&char_param, "-char_param", "char* param help");
    params.AddParam(&params_file, "-faspxx_opts", "additional parameter file");
    params.Parse();
//    params.PrintParams(cout);
//    params.PrintHelp(cout << "\n");

    assert(bool_param == true);
    assert(int_param == 22);
    assert(double_param == 1.41414);
    assert(char_param == "commandline_parameters");
    assert(params_file == "/home/faspxx_file_parameters.txt");
}

void Test_Parameter2(int argc, char** argv)
{
    // ---------------- 3种输入参数的优先级: 命令行参数 > 运行时参数 > 文件参数? -------------------
    // 模拟程序运行时参数
    bool bool_param = 0;
    int  int_param = 0;
    double double_param = 0.0;
    const char* char_param = "";

    // 模拟文件参数
    const char* params_file = "";

    assert(bool_param == false);
    assert(int_param == 0);
    assert(double_param == 0.0);
    assert(char_param == "");
    assert(params_file == "");

    Parameters params(argc, argv);  // 读取命令行参数
    params.AddParam(&bool_param, "-bool_param", "bool param help");
    params.AddParam(&int_param, "-int_param", "int param help");
    params.AddParam(&double_param, "-double_param", "double param help");
    params.AddParam(&char_param, "-char_param", "char* param help");
    params.AddParam(&params_file, "-faspxx_opts", "additional parameter file");
    params.Parse();
//    params.PrintParams(cout);
//    params.PrintHelp(cout << "\n");
}

class Scaling:public LOP {
private:
    VEC diag;
public:
    Scaling(const std::vector<double>& diag) {
        this->diag.SetValues(diag);
        this->nrow = diag.size();
        this->ncol = diag.size();
    }
    void Apply(const VEC& x, VEC& y) const {
        y = x;
        y.PointwiseDivide(diag);
    }
};

int Test_PCG(int argc, char** argv)
{
//    Test_Parameter1();
//    Test_Parameter2(argc, argv);

    const char* mat_file = "../data/fdm_10X10.csr";
    const char* vec_file = "";
    const char* initial_guess = "";
    bool print_level = false;
    const char* opts = "../data/multiple_sol.opts";
    const char* prefix = "-solver1";

    Parameters params(argc, argv);
    params.AddParam(&mat_file, "-mat", "Left hand side of linear system");
    params.AddParam(&vec_file, "-vec", "Right hand side of linear system");
    params.AddParam(&initial_guess, "-initial", "Initial guess of solution");
    params.AddParam(&print_level, "-level", "print level");
    params.AddParam(&opts, "-opts_file", "Solver parameter file");
    params.AddParam(&prefix, "-pref", "solver parameter prefix in file");
    params.Parse();

    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;
    timer.Start();

    // Read matrix data file
    MAT mat;
    if ( (retCode = ReadMat(mat_file, mat)) < 0 ) return retCode;
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
    PCG pcg;
    pcg.SetOptionsFromFile(opts, prefix);
    pcg.Setup(mat, b, x);

    // Setup preconditioner
#if 1
    IdentityLOP lop(row);
    pcg.SetPC(&lop);
#else // Todo: Add choices for preconditioner
    std::vector<double> vt;
mat.GetDiag(vt);
Scaling jac(vt);
pcg.SetupPCD(&jac);
#endif

    // PCG solve
    timer.Start();
    retCode = pcg.Solve(mat, b, x);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

    // Clean up preconditioner and solver data
    pcg.CleanPCD();
    pcg.Clean();
    return retCode;
}

int main(int argc, char** argv)
{
//    Test_Parameter1();
//    Test_Parameter2(argc, argv);

    Test_PCG(argc, argv);

    cout << "------------------- testSOL.cxx is Good! --------------------" << endl;
    return 0;
}