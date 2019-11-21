
#include "Parameters.hxx"
#include <cstring>
#include <iomanip>

void Parameters::AddParam(bool *_ptr, const char *_name, const char *_help) {
    params.push_back(Param(_BOOL, _ptr, _name, _help));
}


void Parameters::AddParam(INT *_ptr, const char *_name, const char *_help) {
    params.push_back(Param(_INT, _ptr, _name, _help));
}


void Parameters::AddParam(DBL *_ptr, const char *_name, const char *_help) {
    params.push_back(Param(_DBL, _ptr, _name, _help));
}


void Parameters::AddParam(const char **_ptr, const char *_name, const char *_help) {
    params.push_back(Param(_STRING, _ptr, _name, _help));
}


void Parameters::AddParam(PRTLVL *_ptr, const char *_name, const char *_help) {
    params.push_back(Param(_PRTLVL, _ptr, _name, _help));
}


void Parameters::Parse() {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            PrintHelp(std::cout);
            return;
        }

        for (int j = 0; j < params.size(); ++j) {
            if (strcmp(argv[i], params[j].param_name) == 0) {
                switch (params[j].type) {
                    case _BOOL:
                        if (!strcmp(argv[i + 1], "true") ||
                            !strcmp(argv[i + 1], "True") ||
                            !strcmp(argv[i + 1], "TRUE"))
                            *((bool *) (params[j].param_ptr)) = 1;
                        else if (!strcmp(argv[i + 1], "false") ||
                                 !strcmp(argv[i + 1], "False") ||
                                 !strcmp(argv[i + 1], "FALSE"))
                            *((bool *) (params[j].param_ptr)) = 0;
                        else // 其余情况就是int型整数拿来给bool类型赋值
                            *((bool *) (params[j].param_ptr)) = (bool) (std::stoi(
                                    argv[i + 1]));
                        break;
                    case _INT:
                        *((INT *) (params[j].param_ptr)) = std::stoi(argv[i + 1]);
                        break;
                    case _DBL:
                        *((DBL *) (params[j].param_ptr)) = std::stod(argv[i + 1]);
                        break;
                    case _STRING:
                        *((char **) (params[j].param_ptr)) = argv[i + 1];
                        break;
                    case _PRTLVL:
                        int n = strlen(argv[i + 1]);
                        int k = 0;
                        while (k < n && '0' <= argv[i + 1][k] &&
                               argv[i + 1][k] <= '9') { k++; }
                        if (k == n) {
                            *((INT *) (params[j].param_ptr)) =
                                    std::stoi(argv[i + 1]);
                            break;
                        }
                        if (strcmp(argv[i + 1], "PRINT_NONE") == 0)
                            *((PRTLVL *) (params[j].param_ptr)) = PRINT_NONE;
                        else if (strcmp(argv[i + 1], "PRINT_MIN") == 0)
                            *((PRTLVL *) (params[j].param_ptr)) = PRINT_MIN;
                        else if (strcmp(argv[i + 1], "PRINT_SOME") == 0)
                            *((PRTLVL *) (params[j].param_ptr)) = PRINT_SOME;
                        else if (strcmp(argv[i + 1], "PRINT_MORE") == 0)
                            *((PRTLVL *) (params[j].param_ptr)) = PRINT_MORE;
                        else if (strcmp(argv[i + 1], "PRINT_MAX") == 0)
                            *((PRTLVL *) (params[j].param_ptr)) = PRINT_MAX;
                        else if (strcmp(argv[i + 1], "PRINT_ALL") == 0)
                            *((PRTLVL *) (params[j].param_ptr)) = PRINT_ALL;
                        break;
                }
                break;
            }
        }
    }
}


void Parameters::PrintParams(std::ostream &out) const {
    out << "\nInput Parameters:\n";
    for (int i = 0; i < params.size(); ++i) {
        out << "  " << params[i].param_name << ' ';
        switch (params[i].type) {
            case _BOOL:
                out << std::boolalpha << *((bool *) (params[i].param_ptr))
                    << std::resetiosflags(out.flags());
                break;
            case _INT:
                out << *((int *) (params[i].param_ptr));
                break;
            case _DBL:
                out << *((double *) (params[i].param_ptr));
                break;
            case _STRING:
                out << *((char **) (params[i].param_ptr));
                break;
            case _PRTLVL:
                out << *((PRTLVL *) (params[i].param_ptr));
                break;
        }
        std::cout << '\n';
    }
}

void Parameters::PrintHelp(std::ostream &out) const {
    static const char *indent = "   ";
    static const char *newline_indent = "\n\t";
    static const char *types[] = {" <bool>", " <INT>", " <DBL>", " <string>",
                                  "<PRTLVL>"};

    out << "\nUsage: " << argv[0] << " [parameters] ...\n"
        << "Parameters:\n";
    out << indent << "-h, --help"
        << newline_indent << "Print this help message and exit.\n";
    for (int i = 0; i < params.size(); ++i) {
        ParamType type = params[i].type;
        out << indent << params[i].param_name << types[type] << ", current value: ";
        switch (type) {
            case _BOOL:
                out << std::boolalpha << *(bool *) (params[i].param_ptr)
                    << std::setiosflags(out.flags());
                break;
            case _INT:
                out << *(int *) (params[i].param_ptr);
                break;
            case _DBL:
                out << *(double *) (params[i].param_ptr);
                break;
            case _STRING:
                out << *(char **) (params[i].param_ptr);
                break;
            case _PRTLVL:
                out << *(PRTLVL *) (params[i].param_ptr);
                break;
        }
        if (params[i].help) {
            out << newline_indent << params[i].help;
        }
        out << '\n';
    }
}
