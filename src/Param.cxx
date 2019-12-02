/*! \file    Param.cxx
 *  \brief   Command line input parameter definition
 *  \author  Ronghong Fan, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cstring>
#include <iostream>
#include <iomanip>
#include "Param.hxx"

void Parameters::AddParam(const char * name, const char * help, bool * ptr)
{
    params.push_back(ParamHolder(BoolType, name, help, ptr));
}

void Parameters::AddParam(const char * name, const char * help, INT * ptr)
{
    params.push_back(ParamHolder(IntType, name, help, ptr));
}

void Parameters::AddParam(const char * name, const char * help, DBL * ptr)
{
    params.push_back(ParamHolder(DoubleType, name, help, ptr));
}

void Parameters::AddParam(const char * name, const char * help, const char ** ptr)
{
    params.push_back(ParamHolder(StringType, name, help, ptr));
}

void Parameters::AddParam(const char * name, const char * help, Output * ptr)
{
    params.push_back(ParamHolder(OutputType, name, help, ptr));
}

void Parameters::Parse()
{
    char *tmp;
    int len;
    for ( int i = 1; i < argc; i++ ) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            PrintHelp(std::cout);
            return;
        }

        for ( size_t j = 0; j < params.size(); ++j ) {
            if (strcmp(argv[i], params[j].paramName) == 0) {
                switch (params[j].paramType) {
                    case BoolType:
                        tmp = new char[strlen(argv[i + 1])];
                        len = strlen(argv[i + 1]);
                        for (int j = 0; j < len; ++j)
                            tmp[j] = argv[i + 1][j] - 'A' + 'a';
                        if (!strcmp(tmp, "true"))
                            *((bool *) (params[j].paramPtr)) = 1;
                        else if (!strcmp(tmp, "false"))
                            *((bool *) (params[j].paramPtr)) = 0;
                        else // 其余情况就是int型整数拿来给bool类型赋值
                            *((bool *) (params[j].paramPtr)) = (bool) (std::stoi(
                                    argv[i + 1]));
                        delete[] tmp;
                        break;
                    case IntType:
                        *((INT *) (params[j].paramPtr)) = std::stoi(argv[i + 1]);
                        break;
                    case DoubleType:
                        *((DBL *) (params[j].paramPtr)) = std::stod(argv[i + 1]);
                        break;
                    case StringType:
                        *((char **) (params[j].paramPtr)) = argv[i + 1];
                        break;
                    case OutputType:
                        int n = strlen(argv[i + 1]);
                        int k = 0;
                        while (k < n && '0' <= argv[i + 1][k] &&
                               argv[i + 1][k] <= '9') { k++; }
                        if (k == n) {
                            *((INT *) (params[j].paramPtr)) =
                                    std::stoi(argv[i + 1]);
                            break;
                        }
                        tmp = new char[strlen(argv[i + 1])];
                        len = strlen(argv[i + 1]);
                        for (int j = 0; j < len; ++j)
                            tmp[j] = (char) (argv[i + 1][j] - 'a' + 'A');
                        if (strcmp(tmp, "PRINT_NONE") == 0)
                            *((Output *) (params[j].paramPtr)) = PRINT_NONE;
                        else if (strcmp(tmp, "PRINT_MIN") == 0)
                            *((Output *) (params[j].paramPtr)) = PRINT_MIN;
                        else if (strcmp(tmp, "PRINT_SOME") == 0)
                            *((Output *) (params[j].paramPtr)) = PRINT_SOME;
                        else if (strcmp(tmp, "PRINT_MORE") == 0)
                            *((Output *) (params[j].paramPtr)) = PRINT_MORE;
                        else if (strcmp(tmp, "PRINT_MAX") == 0)
                            *((Output *) (params[j].paramPtr)) = PRINT_MAX;
                        else if (strcmp(tmp, "PRINT_ALL") == 0)
                            *((Output *) (params[j].paramPtr)) = PRINT_ALL;
                        delete[] tmp;
                        break;
                }
                break;
            }
        }
    }
}

void Parameters::Print(std::ostream &out) const
{
    out << "\nInput Parameters:\n";
    out << "---------------------------------------------------\n";
    for ( size_t i = 0; i < params.size(); ++i ) {
        out << "  " << std::setw(10) << params[i].paramName << " : ";
        switch (params[i].paramType) {
            case BoolType:
                out << std::boolalpha << *((bool *) (params[i].paramPtr))
                    << std::resetiosflags(out.flags());
                break;
            case IntType:
                out << *((int *) (params[i].paramPtr));
                break;
            case DoubleType:
                out << *((double *) (params[i].paramPtr));
                break;
            case StringType:
                out << *((char **) (params[i].paramPtr));
                break;
            case OutputType:
                out << *((Output *) (params[i].paramPtr));
                break;
        }
        out << '\n';
    }
    out << '\n';
}

void Parameters::PrintHelp(std::ostream &out) const
{
    static const char *indent = "   ";
    static const char *newline_indent = "\n    ";
    static const char *types[] = {"<bool>", "<INT>", "<DBL>", "<string>", "<Output>"};

    out << "\nUsage: " << argv[0] << " [parameters] ...\n"
        << "Parameters:\n";
    out << indent << "-h, --help"
        << newline_indent << "Print this help message and exit.\n";
    for ( size_t i = 0; i < params.size(); ++i ) {
        ParamType type = params[i].paramType;
        out << indent << params[i].paramName << " " << types[type]
            << ", current value: ";
        switch (type) {
            case BoolType:
                out << std::boolalpha << *(bool *) (params[i].paramPtr)
                    << std::setiosflags(out.flags());
                break;
            case IntType:
                out << *(int *) (params[i].paramPtr);
                break;
            case DoubleType:
                out << *(double *) (params[i].paramPtr);
                break;
            case StringType:
                out << *(char **) (params[i].paramPtr);
                break;
            case OutputType:
                out << *(Output *) (params[i].paramPtr);
                break;
        }
        if (params[i].paramHelp) {
            out << newline_indent << params[i].paramHelp;
        }
        out << '\n';
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/