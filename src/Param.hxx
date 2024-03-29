/*! \file    Param.hxx
 *  \brief   Command line input parameter declaration
 *  \author  Ronghong Fan, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __PARAM_HEADER__ /*-- allow multiple inclusions --*/
#define __PARAM_HEADER__ /**< indicate Param.hxx has been included before */

// Standard header files
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using std::string;

// FASPXX header files
#include "Faspxx.hxx"

/// Level of output
enum Output {
    PRINT_NONE = 0, // no output
    PRINT_MIN  = 2, // minimal output
    PRINT_SOME = 4, // some output
    PRINT_MORE = 6, // more output
    PRINT_MAX  = 8  // maximal output
};

/// Solver types avaiable.
enum SOLType {
    SOLVER_CG       = 1,  ///< Conjugate Gradient
    SOLVER_BICGSTAB = 2,  ///< Bi-Conjugate Gradient Stabilized
    SOLVER_MINRES   = 3,  ///< Minimal Residual
    SOLVER_GMRES    = 4,  ///< Generalized Minimal Residual
    SOLVER_FGMRES   = 5,  ///< Flexible GMRES
    SOLVER_VFGMRES  = 6,  ///< Variable-restarting FGMRES
    SOLVER_JACOBI   = 11, ///< Jacobi method
    SOLVER_GS       = 12, ///< Gauss-Seidel method
    SOLVER_SGS      = 13, ///< Symmetrized Gauss-Seidel method
    SOLVER_SOR      = 14, ///< Successive over-relaxation method
    SOLVER_SSOR     = 15, ///< Symmetrized successive over-relaxation method
    SOLVER_MG       = 21, ///< Multigrid method
    SOLVER_FMG      = 22, ///< Full multigrid method
    SOLVER_UMFPACK  = 91, ///< Direct method from UMFPACK
    SOLVER_MUMPS    = 92, ///< Direct method from MUMPS
    SOLVER_SUPERLU  = 93, ///< Direct method from SUPERLU
    SOLVER_PARDISO  = 94  ///< Direct method from PARDISO
};

/// Iterative solver parameters.
struct SOLParams {
    SOLType type;    ///< Algorithm type
    string  algName; ///< Algorithm name
    USI     maxIter; ///< Maximal number of iterations
    USI     minIter; ///< Minimal number of iterations
    USI     savIter; ///< Starting safe-guard iteration for Krylov subspace methods
    USI     restart; ///< Restart number for Krylov subspace methods
    double  relTol;  ///< Tolerance for relative residual
    double  absTol;  ///< Tolerance for absolute residual
    Output  verbose; ///< Output verbosity level

    SOLParams()
        : type(SOLType::SOLVER_CG)
        , algName("cg")
        , maxIter(1)
        , minIter(0)
        , savIter(MAX_ITER_NUM)
        , restart(30)
        , relTol(1e-6)
        , absTol(1e-8)
        , verbose(PRINT_NONE)
    {
    }
};

/// Solver parameters
class Parameters
{
    // parameter priority: command line > param file > user program

private:
    /// Possible parameter types
    enum ParamType {
        BoolType   = 0,
        IntType    = 1,
        DoubleType = 2,
        StringType = 3,
        OutputType = 4
    };

    /// Each parameter is stored in a holder
    struct ParamHolder {
        ParamType   paramType;       ///< Type of parameter
        std::string paramName;       ///< Name of parameter
        std::string paramHelp;       ///< Help message
        void*       paramPtr;        ///< Pointer to parameter data
        int         paramMarker = 0; ///< 0: optional, 1: required, 2: params file

        ParamHolder(ParamType type, std::string name, std::string help, void* ptr,
                    int marker)
            : paramType(type)
            , paramName(std::move(name))
            , paramHelp(std::move(help))
            , paramPtr(ptr)
            , paramMarker(marker)
        {
        }
    };

    std::map<std::string, std::string> paramsCML;     ///< Params from command line
    std::map<std::string, std::string> paramsFile;    ///< Params from file
    std::vector<ParamHolder>           paramsUser;    ///< Params in user code
    std::string                        paramsUserOrg; ///< Old params in user code

    int          argc; ///< Number of command-line arguments
    const char** argv; ///< Command-line arguments

private:
    /// Read all parameters from command-line and save them into a Dictionary.
    void ReadFromCommandLine();
    /* For each param, it becomes a key-value pair of the Dictionary.
     * e.g.:
     *   in command line: -mat /home/temp1/a.csr
     *   key == string("-mat"), value == string("/home/temp1/a.csr") will be added
     *   into the dictionary paramsCML.
     * */

    /// Read all parameters from file and save them into a Dictionary.
    void ReadFromFile();
    /* For each param, it becomes a key-value pair of the Dictionary.
     * e.g.:
     *   in file: -mat /home/temp1/a.csr
     *   key == string("-mat"), value == string("/home/temp1/a.csr") will be added
     *   into the dictionary paramsFile.
     * */

    /// Merge and update the same parameters in terms of priority.
    void MergeParams();
    /* params from command line > from file > in user program.
     * e.g.:
     *  in command line: -mat /home/temp1/a.csr
     *  in file: -mat /home/temp2/b.csr
     *  in user program(like in main()): string mat_file = "/home/temp3/c.csr"
     *
     * After this operation,
     *  mat_file == "/home/temp1/a.csr"
     * */

    /// Save the original user parameters for later record.
    void SaveUserParams(std::string& save) const;

    /// Update the value of a user-defined parameter.
    void UpdateParamValue(std::map<std::string, std::string>::iterator& iter,
                          ParamHolder&                                  prm);
    /* If there is a parameter in command line or file which has the same name as
     * parameter in user program, we update its value.
     * */

public:
    /// Default constructor.
    Parameters(int _argc, const char* _argv[])
        : argc(_argc)
        , argv(_argv)
    {
    }

    /// Default destructor.
    ~Parameters() = default;

    /// Add a bool type parameter.
    void AddParam(const std::string& name, const std::string& help, bool* ptr,
                  int marker = 0);

    /// Add a unsigned int type parameter.
    void AddParam(const std::string& name, const std::string& help, USI* ptr,
                  int marker = 0);

    /// Add an int type parameter.
    void AddParam(const std::string& name, const std::string& help, INT* ptr,
                  int marker = 0);

    /// Add a double type parameter.
    void AddParam(const std::string& name, const std::string& help, double* ptr,
                  int marker = 0);

    /// Add a string type parameter.
    void AddParam(const std::string& name, const std::string& help, std::string* ptr,
                  int marker = 0);

    /// Add a Output type parameter.
    void AddParam(const std::string& name, const std::string& help, Output* ptr,
                  int marker = 0);

    /// Parse the parameters.
    void Parse();

    /// Set SOLParams for solvers.
    void SetSOLParams(SOLParams& solParam);

    /// Set SOLParams for solvers and preconditioners.
    void SetSOLParams(SOLParams& solParam, SOLParams& pcdParam);

    /// Print original params (before merge or parse) in user program.
    void PrintUserParams(std::ostream& out) const;

    /// Print parameters coming from an option file.
    void PrintFileParams(std::ostream& out) const;

    /// Print parameters coming from command line input.
    void PrintCommandLineParams(std::ostream& out) const;

    /// Print parameters used in user code (after merge or parse).
    void Print(std::ostream& out = std::cout) const;

    /// Print the help messages for Param.
    void PrintHelp(std::ostream& out = std::cout) const;
};

#endif /* end if for __PARAM_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Nov/25/2019      Create file                          */
/*  Chensong Zhang      Sep/26/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/