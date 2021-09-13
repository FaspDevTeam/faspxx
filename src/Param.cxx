/*! \file    Param.cxx
 *  \brief   Command line input parameter definition
 *  \author  Ronghong Fan, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright=(C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Standard header files
#include <algorithm>

// FASPXX header files
#include "ErrorLog.hxx"
#include "Param.hxx"
#include "RetCode.hxx"

/// Process a line. Return false if the line is a comment or empty.
static bool ProcessLine(std::string &line, std::string &param, std::string &value)
{
    // Remove spaces in the beginning and end of each line
    line.erase(0, line.find_first_not_of(' '));
    line.erase(line.find_last_not_of(' ') + 1);

    if (line.empty()) return false;   // Skip empty lines
    if (line[0] == '#') return false; // Skip comments
    if (line[0] == '%') return false; // Skip comments

    std::string       buff;
    std::stringstream ss(line);
    ss >> param >> value >> buff;
    return true;
}

/// Convert a string of true or false to bool.
static bool JudgeBool(const std::string &value)
{
    std::string copy(value);
    std::transform(value.begin(), value.end(), copy.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (copy == "true") {
        return true;
    } else if (copy == "false") {
        return false;
    } else { // otherwise, an integer
        return (bool)(std::stoi(copy));
    }
}

/// Save the user specified parameters.
void Parameters::SaveUserParams(std::string &save) const
{
    size_t max_len = 0;
    for (const auto &itm : paramsUser) {
        if (itm.paramName.length() > max_len) max_len = itm.paramName.length();
    }

    std::ostringstream out;
    static std::string indent = "   ";
    out << "Original parameters in user program:\n"
        << "---------------------------------------------\n";

    for (auto &itm : paramsUser) {
        out << indent << std::setw(max_len) << std::left << itm.paramName << " [";
        switch (itm.paramType) {
            case BoolType:
                out << std::boolalpha << *((bool *)(itm.paramPtr))
                    << std::resetiosflags(out.flags());
                break;
            case IntType:
                out << *((int *)itm.paramPtr);
                break;
            case DoubleType:
                out << *((double *)itm.paramPtr);
                break;
            case StringType:
                out << *((std::string *)itm.paramPtr);
                break;
            case OutputType:
                out << *((Output *)(itm.paramPtr));
                break;
        }
        out << "]\n";
    }
    out << '\n';
    save = out.str();
}

/// Read user parameters from command line input.
void Parameters::ReadFromCommandLine()
{
    if (argc == 1) return; // no command line arguments

    for (int i = 1; i < argc; ++i) {
        // -h, --help will trigger program terminate
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            PrintHelp();
            std::exit(FaspRetCode::PRINT_HELP);
        }

        // check whether the parameters come in pairs: it is still possible the
        // arguments are in pairs but mismatch!!!
        if (i >= argc - 1) {
            std::cout << "Check the argument: " << argv[i] << std::endl;
            PrintHelp();
            std::exit(FaspRetCode::PRINT_HELP);
        }

        // each param is a key-value pair of a Dictionary
        paramsCML.insert(std::pair<std::string, std::string>(argv[i], argv[i + 1]));
        ++i;
    }
}

/// Read user parameters from an input file.
void Parameters::ReadFromFile()
{
    for (auto &itm : paramsUser) {
        if (itm.paramMarker == 2) {
            std::ifstream file;

            // first try to find param_file from command line params
            if (paramsCML.find(itm.paramName) != paramsCML.end()) {
                file.open(paramsCML.find(itm.paramName)->second);
            } else {
                file.open(*((std::string *)itm.paramPtr)); // find param_file
            }

            if (!file.is_open()) FASPXX_WARNING("File not found!");

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

/// Merge parameters from different sources.
void Parameters::MergeParams()
{
    std::map<std::string, std::string>::iterator iter;
    for (auto &prm : paramsUser) {
        iter = paramsFile.find(prm.paramName);
        if (iter != paramsFile.end()) UpdateParamValue(iter, prm);

        iter = paramsCML.find(prm.paramName);
        if (iter != paramsCML.end()) UpdateParamValue(iter, prm);
    }
}

/// Update parameter value and give warning if unknown type is specified.
void Parameters::UpdateParamValue(std::map<std::string, std::string>::iterator &iter,
                                  ParamHolder &                                 prm)
{
    switch (prm.paramType) {
        case BoolType:
            *(bool *)prm.paramPtr = JudgeBool(iter->second);
            break;
        case IntType:
            *(int *)prm.paramPtr = std::stoi(iter->second);
            break;
        case DoubleType:
            *(double *)prm.paramPtr = std::stod(iter->second);
            break;
        case StringType:
            *(std::string *)prm.paramPtr = iter->second;
            break;
        case OutputType:
            *(Output *)prm.paramPtr = (Output)std::stoi(iter->second);
            break;
        default:
            FASPXX_WARNING("Unknown parameter type!");
    }
}

/// Bool type parameter.
void Parameters::AddParam(const std::string &name, const std::string &help, bool *ptr,
                          int marker)
{
    paramsUser.emplace_back(BoolType, name, help, ptr, marker);
}

/// Int type parameter.
void Parameters::AddParam(const std::string &name, const std::string &help, int *ptr,
                          int marker)
{
    paramsUser.emplace_back(IntType, name, help, ptr, marker);
}

/// Double type parameter.
void Parameters::AddParam(const std::string &name, const std::string &help, double *ptr,
                          int marker)
{
    paramsUser.emplace_back(DoubleType, name, help, ptr, marker);
}

/// String type parameter.
void Parameters::AddParam(const std::string &name, const std::string &help,
                          std::string *ptr, int marker)
{
    paramsUser.emplace_back(StringType, name, help, ptr, marker);
}

/// Output type parameter.
void Parameters::AddParam(const std::string &name, const std::string &help, Output *ptr,
                          int marker)
{
    paramsUser.emplace_back(OutputType, name, help, ptr, marker);
}

/// Main entrance point for reading and handling parameters.
void Parameters::Parse()
{
    // Read parameters
    ReadFromCommandLine();
    ReadFromFile();

    // Save the original and then merge
    SaveUserParams(paramsUserOrg);
    MergeParams();
}

/// Set SOLParams for solvers.
void Parameters::SetSOLParams(SOLParams &solParam)
{
    this->AddParam("-maxIter", "Max iteration steps", &solParam.maxIter);
    this->AddParam("-minIter", "Min iteration steps", &solParam.minIter);
    this->AddParam("-safeIter", "Safe-guard steps", &solParam.safeIter);
    this->AddParam("-restart", "Restart number", &solParam.restart);
    this->AddParam("-resRel", "Relative residual tolerance", &solParam.relTol);
    this->AddParam("-resAbs", "Absolute residual tolerance", &solParam.absTol);
    this->AddParam("-verbose", "Verbose level", &solParam.verbose);
}

/// Set SOLParams for solvers and preconditioners.
void Parameters::SetSOLParams(SOLParams &solParam, SOLParams &pcParam)
{
    SetSOLParams(solParam);

    this->AddParam("-pcIter", "Preconditioner steps", &pcParam.maxIter);
    this->AddParam("-pcWeight", "Preconditioner weigth", &pcParam.weight);
}

void Parameters::PrintFileParams(std::ostream &out) const
{
    size_t max_len = 0;
    for (const auto &itm : paramsFile) {
        if (itm.first.length() > max_len) max_len = itm.first.length();
    }

    std::string options_file;
    for (const auto &itm : paramsUser) {
        if (itm.paramMarker == 2) options_file = *(std::string *)itm.paramPtr;
    }

    static std::string indent = "   ";
    out << "---------------------------------------------\n"
        << "Parameters from file " << options_file << ":\n"
        << "---------------------------------------------\n";
    for (const auto &itm : paramsFile) {
        out << indent << std::setw(max_len) << std::left << itm.first << " ["
            << itm.second << "]\n";
    }
    out << std::endl;
}

void Parameters::PrintCommandLineParams(std::ostream &out) const
{
    size_t max_len = 0;
    for (const auto &itm : paramsCML) {
        if (itm.first.length() > max_len) max_len = itm.first.length();
    }

    static std::string indent = "   ";
    out << "---------------------------------------------\n"
        << "Parameters from the command line input\n"
        << "---------------------------------------------\n";
    for (const auto &itm : paramsCML) {
        out << indent << std::setw(max_len) << std::left << itm.first << " ["
            << itm.second << "]\n";
    }
    out << std::endl;
}

void Parameters::PrintUserParams(std::ostream &out) const
{
    out << paramsUserOrg << std::endl;
}

/// Print parameters used with width adapt to the names.
void Parameters::Print(std::ostream &out) const
{
    size_t max_len = 0;
    for (const auto &itm : paramsUser) {
        if (itm.paramName.length() > max_len) max_len = itm.paramName.length();
    }

    static std::string indent = "   ";
    out << "---------------------------------------------\n"
        << "Parameters used in the program\n"
        << "---------------------------------------------\n";

    for (const auto &itm : paramsUser) {
        out << indent << std::setw(max_len) << std::left << itm.paramName << " [";
        switch (itm.paramType) {
            case BoolType:
                out << std::boolalpha << *((bool *)(itm.paramPtr))
                    << std::resetiosflags(out.flags());
                break;
            case IntType:
                out << *((int *)itm.paramPtr);
                break;
            case DoubleType:
                out << *((double *)itm.paramPtr);
                break;
            case StringType:
                out << *((std::string *)itm.paramPtr);
                break;
            case OutputType:
                out << *((Output *)(itm.paramPtr));
                break;
        }
        out << "]\n";
    }
    out << '\n';
}

/// Print out usage help for command line.
void Parameters::PrintHelp(std::ostream &out) const
{
    static const char *indent  = "   ";
    static const char *types[] = {"<bool>", "<int>", "<double>", "<string>",
                                  "<Output>"};

    size_t max_len = 0;
    for (const auto &itm : paramsUser) {
        if (itm.paramName.length() > max_len) max_len = itm.paramName.length();
    }

    out << "Usage: " << argv[0] << " [options] ...\n"
        << "Options:\n";

    for (const auto &itm : paramsUser) {
        out << indent << std::setw(max_len) << std::left << itm.paramName << " "
            << std::setw(8) << types[itm.paramType];
        if (itm.paramMarker == 0)
            out << ", optional  ";
        else if (itm.paramMarker == 1)
            out << ", required  ";
        else
            out << ", from file ";
        if (!itm.paramHelp.empty()) out << " :  " << itm.paramHelp;

        out << ", default = [";
        switch (itm.paramType) {
            case BoolType:
                out << std::boolalpha << *(bool *)(itm.paramPtr)
                    << std::setiosflags(out.flags());
                break;
            case IntType:
                out << *(int *)(itm.paramPtr);
                break;
            case DoubleType:
                out << *(double *)(itm.paramPtr);
                break;
            case StringType:
                out << *((std::string *)itm.paramPtr);
                break;
            case OutputType:
                out << *(Output *)(itm.paramPtr);
                break;
        }
        out << "]\n";
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/