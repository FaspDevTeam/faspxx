/*! \file    Param.hxx
 *  \brief   Command line input parameters
 *  \author  Ronghong Fan
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

enum PRTLVL {
    PRINT_NONE = 0,
    PRINT_MIN  = 2,
    PRINT_SOME = 4,
    PRINT_MORE = 6,
    PRINT_MAX  = 8,
    PRINT_ALL  = 10
};

class Parameters {
    
public:
    enum ParamType {
        BoolType, IntType, DoubleType, StringType, PrintLevelType
    }; // allowed parameter types

private:
    struct Param // each parameter is saved in a struct
    {
        ParamType type;
        void *param_ptr;
        const char *param_name;
        const char *help;

        Param(ParamType _type, void *_ptr, const char *_name, const char *_help)
                : type(_type), param_ptr(_ptr), param_name(_name), help(_help) {}
    };

    std::vector<Param> params; // all parameters
    int argc;
    char **argv;

public:
    Parameters(int _argc, char *_argv[]) : argc(_argc), argv(_argv) {}

    ~Parameters() {}

    // TODO: required or optional for every parameterfff
    void AddParam(bool *_ptr, const char *_name, const char *_help);

    void AddParam(INT *_ptr, const char *_name, const char *_help);

    void AddParam(DBL *_ptr, const char *_name, const char *_help);

    void AddParam(const char **_ptr, const char *_name, const char *_help);

    void AddParam(PRTLVL *_ptr, const char *_name, const char *_help);

    void Parse();

    void PrintParams(std::ostream &out = std::cout) const;

    void PrintHelp(std::ostream &out = std::cout) const;
};

#endif /* end if for __PARAM_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/