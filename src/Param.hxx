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
#include <iostream>
#include "faspxx.hxx"

/// Level of output
enum Output {
    PRINT_NONE = 0,  // no output
    PRINT_MIN  = 2,  // minimal output
    PRINT_SOME = 4,  // some output
    PRINT_MORE = 6,  // more output
    PRINT_MAX  = 8,  // even more output
    PRINT_ALL  = 10  // all possible output
};

/// Command line parameters
class Parameters {
    
public:
    enum ParamType {
        BoolType, IntType, DoubleType, StringType, OutputType
    }; ///< Possible parameter types

private:
    struct ParamHolder {
        ParamType      paramType;  ///< Type of parameter
        const char *   paramName;  ///< Name of parameter
        const char *   paramHelp;  ///< Help message
        void *         paramPtr;   ///< Pointer to parameter data

        ParamHolder(ParamType type, const char * name, const char * help, void * ptr)
                : paramType(type), paramName(name), paramHelp(help), paramPtr(ptr) {}
    }; ///< Each parameter is saved in a holder

    std::vector<ParamHolder> params; ///< all parameters

    int    argc;
    char **argv;

public:
    Parameters(int _argc, char * _argv[]) : argc(_argc), argv(_argv) {}

    ~Parameters() {}

    // TODO: required or optional for every parameter -fff
    void AddParam(const char * name, const char * help, bool * ptr);

    void AddParam(const char * name, const char * help, INT * ptr);

    void AddParam(const char * name, const char * help, DBL * ptr);

    void AddParam(const char * name, const char * help, const char ** ptr);

    void AddParam(const char * name, const char * help, Output * ptr);

    void Parse();

    void Print(std::ostream &out = std::cout) const;

    void PrintHelp(std::ostream &out = std::cout) const;
};

#endif /* end if for __PARAM_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/