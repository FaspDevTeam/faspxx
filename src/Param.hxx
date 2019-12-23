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

#ifndef __PARAM_HEADER__      /*-- allow multiple inclusions --*/
#define __PARAM_HEADER__      /**< indicate Param.hxx has been included before */

#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include "faspxx.hxx"
#include "utils.hxx"

/// Level of output
enum Output {
    PRINT_NONE = 0,  // no output
    PRINT_MIN  = 2,  // minimal output
    PRINT_SOME = 4,  // some output
    PRINT_MORE = 6,  // more output
    PRINT_MAX  = 8,  // even more output
    PRINT_ALL  = 10  // all possible output
};

/// Solver parameters
class Parameters {
    // parameter priority: command line > param file > user program

private:
    /// Possible parameter types
    enum ParamType {
        BoolType    = 0,
        IntType     = 1,
        DoubleType  = 2,
        StringType  = 3,
        OutputType  = 4
    };

    /// Each parameter is stored in a holder
    struct ParamHolder {
        ParamType    paramType;      ///< Type of parameter
        std::string  paramName;      ///< Name of parameter
        std::string  paramHelp;      ///< Help message
        void *       paramPtr;       ///< Pointer to parameter data
        int          paramMarker=0;  ///< 0: optional, 1: required, 2: params file

        ParamHolder(ParamType type, std::string name, std::string help,
                    void * ptr, int marker)
                   :paramType(type), paramName(name), paramHelp(help),
                    paramPtr(ptr), paramMarker(marker) {}
    };

    std::map<std::string, std::string>   paramsCML;     ///< Params from command line
    std::map<std::string, std::string>   paramsFile;    ///< Params from file
    std::vector<ParamHolder>             paramsUser;    ///< Params in user code
    std::string                          paramsUserOrg; ///< Old params in user code

    int      argc;    ///< number of command-line arguments
    char   **argv;    ///< command-line arguments

private:
    /* Read all parameters from command line and save them into a Dictionary.
     * For each param, it becomes a key-value pair of the Dictionary.
     * e.g.:
     *  in command line: -mat /home/temp1/a.csr
     *  key == string("-mat"), value == string("/home/temp1/a.csr"), add into Dictionary paramsCML
     * */
    void ReadCommandLineParams();

    /* Read all parameters from file and save them into a Dictionary.
     * For each param, it becomes a key-value pair of the Dictionary.
     * e.g.:
     *  in file: -mat /home/temp1/a.csr
     *  key == string("-mat"), value == string("/home/temp1/a.csr"), add into Dictionary paramsFile
     * */
    void ReadFileParams();

    /* merge and update the same parameters in terms of priority:
     * params in command line > params in file > params in user program.
     * e.g.:
     *  in command line: -mat /home/temp1/a.csr
     *  in file: -mat /home/temp2/b.csr
     *  in user program(like in main()): string mat_file = "/home/temp3/c.csr"
     *
     * After this operation,
     *  mat_file == "/home/temp1/a.csr"
     * */
    void MergeParams();

    // save original user params for visualize.
    void SaveOriginalUserParams(std::string& save) const;

    /* If there is parameter in command line or file which is same name with parameter in user program,
     * we update its value.
     * */
    void UpdateParamValue(std::map<std::string, std::string>::iterator& iter, ParamHolder& prm);

public:
    /// Default constructor
    Parameters(int _argc, char * _argv[]) : argc(_argc), argv(_argv) {}

    /// Default destructor
    ~Parameters() {}

    /// Add a bool type parameter
    void AddParam(const std::string& name, const std::string& help, bool * ptr, int marker=0);

    /// Add an int type parameter
    void AddParam(const std::string& name, const std::string& help, int * ptr, int marker=0);

    /// Add a double type parameter
    void AddParam(const std::string& name, const std::string& help, double * ptr, int marker=0);

    /// Add a string type parameter
    void AddParam(const std::string& name, const std::string& help, std::string * ptr, int marker=0);

    /// Add a Output type parameter
    void AddParam(const std::string& name, const std::string& help, Output * ptr, int marker=0);

    void Parse();

    // Only print parameters in file
    void PrintFileParams(std::ostream& out) const;

    // Only print parameters in command line
    void PrintCommandLineParams(std::ostream& out) const;

    // Only print original params (before merge or parse) in user program
    void PrintUserParams(std::ostream &out) const;

    // Only print parameters finally used in user program (after merge or parse)
    void Print(std::ostream& out = std::cout) const;

    void PrintHelp(std::ostream &out = std::cout) const;
};


#endif /* end if for __PARAM_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/