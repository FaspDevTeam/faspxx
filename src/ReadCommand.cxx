/*! \file ReadCommand.cxx
 *  \brief Read command line input parameters
 *  \author Kailei Zhang, Chensong Zhang
 *  \date Nov/02/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cstring>
#include "RetCode.hxx"
#include "Param.hxx"

/// \brief Print help message for command line
static void Help(char* funcname) {
    std::cout << "Command line options include: \n"
              << "  -mat:     matrix data file (not optional)\n"
              << "  -rhs:     right-hand side data file\n"
              << "  -lhs:     initial solution data file\n"
              << "  -maxIter: maximum number of iterations\n"
              << "  -relTol:  relative tolerance for iterations\n"
              << "  -absTol:  absolute tolerance for iterations\n"
              << "  -restart: restart number\n"
              << "  -verbose:  output level (0--10)\n\n"
              << "Sample usages of command line options: \n"
              << "  " << funcname << " -mat MatrixDataFile \n"
              << "  " << funcname << " -mat MatrixDataFile -rhs RHSDataFile \n"
              << "  " << funcname << " -mat MatrixDataFile -maxIter 50 -relTol 10e-8\n";
}

/// \brief Convert command line choices to an integer flag
static int StringToInt(char* ch) {

    int flag = 0; // default: print help messages

    if      (strcmp(ch, "-mat")     == 0) flag = 1;
    else if (strcmp(ch, "-rhs")     == 0) flag = 2;
    else if (strcmp(ch, "-lhs")     == 0) flag = 3;
    else if (strcmp(ch, "-verbose") == 0) flag = 4;
    else if (strcmp(ch, "-maxIter") == 0) flag = 5;
    else if (strcmp(ch, "-relTol")  == 0) flag = 6;
    else if (strcmp(ch, "-absTol")  == 0) flag = 7;
    else if (strcmp(ch, "-restart") == 0) flag = 8;

    return flag;
}

/// \brief Read parameters from command line
FaspRetCode ReadParam(int argc, char* args[], InitParam& init) {

    FaspRetCode retCode = FaspRetCode::SUCCESS;

    // Exit if no parameter specified -- At least provide matrix data file
    if ( argc == 1 ) {
        std::cout << "### ERROR: Missing file operands\n";
        std::cout << "Usage: " << args[0] << " [-options]\n";
        std::cout << "Try '" << args[0] << " -help' for more information\n";
        retCode = FaspRetCode::ERROR_INPUT_PAR;
        return retCode;
    }

    bool noMatFile = true; // get matrix file name or not
    bool missInput = true; // missing input value

    for ( int j = 1; j < argc; j++ ) {
        const int flag = StringToInt(args[j]);

        if ( flag == 0 ) {
            Help(args[0]);
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            return retCode; // early exit if asking for help
        }

        if ( j < argc-1 ) { // check whether have enough input

            switch ( flag ) {
                case 1:
                    noMatFile = false;
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.data.SetMatName(args[j + 1]);
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 2:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.data.SetRhsName(args[j + 1]);
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 3:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.data.SetLhsName(args[j + 1]);
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 4:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        if (strcmp(args[j + 1],"PRINT_NONE")==0)
                            init.param.SetVerbose(PRINT_NONE);
                        if (strcmp(args[j + 1],"PRINT_MIN")==0)
                            init.param.SetVerbose(PRINT_MIN);
                        if (strcmp(args[j + 1],"PRINT_SOME")==0)
                            init.param.SetVerbose(PRINT_SOME);
                        if (strcmp(args[j + 1],"PRINT_MORE")==0)
                            init.param.SetVerbose(PRINT_MORE);
                        if (strcmp(args[j + 1],"PRINT_MAX")==0)
                            init.param.SetVerbose(PRINT_MAX);
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 5:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.param.SetMaxIter(atoi(args[j + 1]));
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 6:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.param.SetRelTol(atof(args[j + 1]));
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 7:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.param.SetAbsTol(atof(args[j + 1]));
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                case 8:
                    if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                        missInput = false;
                        init.param.SetRestart(atoi(args[j + 1]));
                        ++j;
                    }
                    else {
                        retCode = FaspRetCode::ERROR_INPUT_PAR;
                    }
                    break;

                default:
                    std::cout << "### ERROR: bad file operands!\n";
                    std::cout << "Try " << args[0] << " -help for more information.\n";
                    retCode = FaspRetCode::ERROR_INPUT_PAR;
                    return retCode;
            }
        }

        if ( missInput ) {
            std::cout << "### ERROR: Missing input argument!\n";
            std::cout << "Try '" << args[0] << " -help' for more information\n";
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            return retCode;
        }
        missInput = true; // Always check whether an option has argument or not

    }

    if ( noMatFile ) {
        std::cout << "### ERROR: Missing matrix data file name!\n";
        std::cout << "Try '" << args[0] << " -help' for more information\n";
        retCode = FaspRetCode::ERROR_INPUT_PAR;
    }

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/