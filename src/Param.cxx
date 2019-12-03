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

void Parameters::AddParam(const char * name, const char * help, int * ptr)
{
    params.push_back(ParamHolder(IntType, name, help, ptr));
}

void Parameters::AddParam(const char * name, const char * help, double * ptr)
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
                        for (int j = 0; j < len; ++j) {
                            if ('A' <= argv[i + 1][j] && argv[i + 1][j] <= 'Z')
                                tmp[j] = (char) (argv[i + 1][j] - 'A' + 'a');
                            else
                                tmp[j] = argv[i + 1][j];
                        }
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
                        *((int *) (params[j].paramPtr)) = std::stoi(argv[i + 1]);
                        break;
                    case DoubleType:
                        *((double *) (params[j].paramPtr)) = std::stod(argv[i + 1]);
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
                            *((int *) (params[j].paramPtr)) =
                                    std::stoi(argv[i + 1]);
                            break;
                        }
                        std::cout<<"Print_level : "<<argv[i+1]<<std::endl;
                        tmp = new char[strlen(argv[i + 1])];
                        len = strlen(argv[i + 1]);
                        for (int j = 0; j < len; ++j){
                            if('a'<=argv[i+1][j] && argv[i+1][j]<='z')
                                tmp[j]=(char)(argv[i+1][j]-'a'+'A');
                            else
                                tmp[j]=argv[i+1][j];
                        }
                        std::cout<<"Output Type : "<<tmp<<std::endl;
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
    static const char *indent = "   ";
    out << "\nInput Parameters:\n";
    out << "---------------------------------------------------\n";
    for ( size_t i = 0; i < params.size(); ++i ) {
        out << indent << std::setw(10) << std::left << params[i].paramName;
        out << " : ";
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
    static const char *types[] = {"<bool>", "<int>", "<double>", "<string>", "<Output>"};

    out << "Usage: " << argv[0] << " [options] ...\n"
        << "Options:\n";
    out << indent << std::setw(21) << std::left
        << "-h, --help" << " : Print help information and exit.\n";

    for ( size_t i = 0; i < params.size(); ++i ) {
        ParamType type = params[i].paramType;
        out << indent << std::setw(12) << std::left << params[i].paramName
            << " " << std::setw(8) << types[type];
        if ( params[i].paramHelp ) out << " : " << params[i].paramHelp;

        out << ", default = [";
        switch ( type ) {
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
        out << "]\n";
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/