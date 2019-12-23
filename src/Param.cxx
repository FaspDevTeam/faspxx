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
#include "ErrorLog.hxx"

void Parameters::SaveUserParams(std::string& save) const {
    int max_len = 0;
    for ( const auto& itm: paramsUser ) {
        if ( itm.paramName.length() > max_len ) max_len = itm.paramName.length();
    }

    std::ostringstream out;
    static std::string indent = "   ";
    out << "Original parameters in user program:\n"
        << "---------------------------------------------\n";

    for ( auto& itm: paramsUser ) {
        out << indent << std::setw(max_len) << std::left << itm.paramName << " [";
        switch ( itm.paramType ) {
            case BoolType:
                out << std::boolalpha << *((bool *) (itm.paramPtr))
                    << std::resetiosflags(out.flags());
                break;
            case IntType:
                out << *((int*) itm.paramPtr);
                break;
            case DoubleType:
                out << *((double*) itm.paramPtr);
                break;
            case StringType:
                out << *((std::string*) itm.paramPtr);
                break;
            case OutputType:
                out << *((Output *) (itm.paramPtr));
                break;
        }
        out << "]\n";
    }
    out << '\n';
    save = out.str();
}

void Parameters::ReadFromCommandLine()
{
    if (argc == 1) return;

    for (int i = 1; i < argc; ++i) {
        // -h, --help will trigger program terminate
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            PrintHelp();
            FASPXX_ABORT("-h, --help trigger abort");
        }

        // each param is a key-value pair of a Dictionary
        paramsCML.insert(std::pair<std::string, std::string>(argv[i], argv[i+1]));
        ++i;
    }
}

void Parameters::ReadFromFile()
{
    for (auto& itm: paramsUser) {
        if (itm.paramMarker == 2)
        {
            std::ifstream file;

            // first try to find param_file from command line params
            if (paramsCML.find(itm.paramName) != paramsCML.end()) {
                file.open(paramsCML.find(itm.paramName)->second);
            }
            else {
                file.open(*((std::string*)itm.paramPtr)); // second try to find param_file from user program
            }

            if (!file.is_open()) FASPXX_WARNING("Not found file");

            std::string line, name, value;
            while (std::getline(file, line)) {
                if (ProcessLine(line, name, value)) {
                    // each param is a key-value pair of a Dictionary
                    paramsFile.insert(std::pair<std::string, std::string>(name, value));
                }
            }
            file.close();
            break; // at most 1 param_file
        }
    }
}

void Parameters::MergeParams()
{
    std::map<std::string, std::string>::iterator iter;
    for (auto& prm: paramsUser) {
        iter = paramsFile.find(prm.paramName);
        if (iter != paramsFile.end()) UpdateParamValue(iter, prm);

        iter = paramsCML.find(prm.paramName);
        if (iter != paramsCML.end()) UpdateParamValue(iter, prm);
    }
}

void Parameters::UpdateParamValue(std::map<std::string, std::string>::iterator& iter, ParamHolder& prm)
{
    switch (prm.paramType) {
        case BoolType:
            *(bool*) prm.paramPtr = JudgeBool(iter->second);
            break;
        case IntType:
            *(int*) prm.paramPtr = std::stoi(iter->second);
            break;
        case DoubleType:
            *(double*) prm.paramPtr = std::stod(iter->second);
            break;
        case StringType:
            *(std::string*) prm.paramPtr = iter->second;
            break;
        case OutputType:
            *(Output*) prm.paramPtr = (Output) std::stoi(iter->second);
    }
}

void Parameters::AddParam(const std::string& name, const std::string& help, bool * ptr, int marker) {
    paramsUser.push_back(ParamHolder(BoolType, name, help, ptr, marker));
}

void Parameters::AddParam(const std::string& name, const std::string& help, int * ptr, int marker) {
    paramsUser.push_back(ParamHolder(IntType, name, help,
                                      ptr, marker));
}

void Parameters::AddParam(const std::string& name, const std::string& help, double * ptr, int marker) {
    paramsUser.push_back(ParamHolder(DoubleType, name, help,
                                      ptr, marker));
}

void Parameters::AddParam(const std::string& name, const std::string& help, std::string * ptr, int marker) {
    paramsUser.push_back(ParamHolder(StringType, name, help,
                                      ptr, marker));
}

void Parameters::AddParam(const std::string& name, const std::string& help, Output * ptr, int marker) {
    paramsUser.push_back(ParamHolder(OutputType, name, help, ptr, marker));
}

void Parameters::Parse()
{
    // Read parameters
    ReadFromCommandLine();
    ReadFromFile();

    // Save the original and then merge
    SaveUserParams(paramsUserOrg);
    MergeParams();
}

void Parameters::PrintFileParams(std::ostream& out) const {
    int max_len = 0;
    for ( const auto& itm: paramsFile ) {
        if (itm.first.length() > max_len) max_len = itm.first.length();
    }

    std::string options_file;
    for ( const auto& itm: paramsUser ) {
        if ( itm.paramMarker == 2 ) options_file = *(std::string * )itm.paramPtr;
    }

    static std::string indent = "   ";
    out << "Parameters from file " << options_file << ":\n"
        << "---------------------------------------------\n";
    for ( const auto& itm: paramsFile ) {
        out << indent << std::setw(max_len) << std::left
            << itm.first << " [" << itm.second << "]\n";
    }
    out << std::endl;
}

void Parameters::PrintCommandLineParams(std::ostream& out) const {
    int max_len = 0;
    for ( const auto& itm: paramsCML ) {
        if (itm.first.length() > max_len) max_len = itm.first.length();
    }

    static std::string indent = "   ";
    out << "Parameters from the command line input:\n"
        << "---------------------------------------------\n";
    for (const auto& itm: paramsCML) {
        out << indent << std::setw(max_len) << std::left
            << itm.first << " [" << itm.second << "]\n";
    }
    out << std::endl;
}

void Parameters::PrintUserParams(std::ostream &out) const {
    out << paramsUserOrg << std::endl;
}

void Parameters::Print(std::ostream &out) const {
    int max_len = 0;
    for ( const auto& itm: paramsUser ) {
        if ( itm.paramName.length() > max_len ) max_len = itm.paramName.length();
    }

    static std::string indent = "   ";
    out << "Parameters used in program:\n"
        << "---------------------------------------------\n";

    for ( auto& itm: paramsUser ) {
        out << indent << std::setw(max_len) << std::left << itm.paramName << " [";
        switch (itm.paramType) {
            case BoolType:
                out << std::boolalpha << *((bool *) (itm.paramPtr))
                    << std::resetiosflags(out.flags());
                break;
            case IntType:
                out << *((int*) itm.paramPtr);
                break;
            case DoubleType:
                out << *((double*) itm.paramPtr);
                break;
            case StringType:
                out << *((std::string*) itm.paramPtr);
                break;
            case OutputType:
                out << *((Output *) (itm.paramPtr));
                break;
        }
        out << "]\n";
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
        << "-h, --help" << "              : print help information and exit\n";

    for (const auto& prm: paramsUser ) {
        ParamType type = prm.paramType;
        out << indent << std::setw(12) << std::left << prm.paramName
            << " " << std::setw(8) << types[type];
        if ( prm.paramMarker == 0)      out << ", optional   ";
        else if ( prm.paramMarker == 1) out << ", required   ";
        else                                  out << ", params file";
        if ( !prm.paramHelp.empty() ) out << " : " << prm.paramHelp;

        out << ", default = [";
        switch ( type ) {
            case BoolType:
                out << std::boolalpha << *(bool *) (prm.paramPtr)
                    << std::setiosflags(out.flags());
                break;
            case IntType:
                out << *(int *) (prm.paramPtr);
                break;
            case DoubleType:
                out << *(double *) (prm.paramPtr);
                break;
            case StringType:
                out << *(char **) (prm.paramPtr);
                break;
            case OutputType:
                out << *(Output *) (prm.paramPtr);
                break;
        }
        out << "]\n";
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/