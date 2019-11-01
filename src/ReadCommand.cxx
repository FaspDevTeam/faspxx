/*! \file ReadCommand.cxx
 *  \brief Read command line input parameters
 *  \author Kailei Zhang, Chensong Zhang
 *  \date Oct/26/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cstring>
#include "RetCode.hxx"
#include "Param.hxx"

// print help information
static void Help() {
    std::cout << "Command line options include: \n"
              << "  -mat:     matrix data file (not optional)\n"
              << "  -rhs:     right-hand side data file\n"
              << "  -lhs:     initial solution data file\n"
              << "  -maxIter: maximum number of iterations\n"
              << "  -relTol:  relative tolerance for iterations\n"
              << "  -absTol:  absolute tolerance for iterations\n"
              << "  -restart: restart number\n"
              << "  -outLvl:  output level (0--10)\n\n"
              << "Sample usages of command-line options: \n"
              << "  ./executable -mat MatrixDataFile \n"
              << "  ./executable -mat MatrixDataFile -rhs RHSDataFile \n"
              << "  ./executable -mat MatrixDataFile -maxIter 50 -relTol 10e-8\n";
}

// convert command line choices to integer
static int StringToInt(char *ch) {
    int flag = -1;
    if (strcmp(ch, "-help") == 0)
        flag = 0;
    else if (strcmp(ch, "-mat") == 0)
        flag = 1;
    else if (strcmp(ch, "-rhs") == 0)
        flag = 2;
    else if (strcmp(ch, "-lhs") == 0)
        flag = 3;
    else if (strcmp(ch, "-outLvl") == 0)
        flag = 4;
    else if (strcmp(ch, "-maxIter") == 0)
        flag = 5;
    else if (strcmp(ch, "-relTol") == 0)
        flag = 6;
    else if (strcmp(ch, "-absTol") == 0)
        flag = 7;
    else if (strcmp(ch, "-restart") == 0)
        flag = 8;

    return flag;
}

// read parameters from command lines
FaspRetCode ReadParam(int argc, char *args[], InitParam& init) {

    FaspRetCode retCode = FaspRetCode::SUCCESS;

    // At least provide matrix data file to start
    if ( argc == 1 ) {
        std::cout << "### ERROR: Missing file operands\n";
        std::cout << "Usage: " << args[0] << " [-options]\n";
        std::cout << "Try '"   << args[0] << " -help' for more information\n";
        retCode = FaspRetCode::ERROR_INPUT_PAR;
        return retCode;
    }

    bool NoInput = true; // marker for minimum input

    for ( int j = 1; j <= argc - 1; j++ ) {
        int flag = StringToInt(args[j]);
        if ( flag == 1 ) NoInput = false;
        switch (flag) {
            case 0:
                Help();
                retCode = FaspRetCode::ERROR_INPUT_PAR;
                return retCode;
            case 1:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.data.SetMatName(args[j + 1]);
                ++j;
                break;
            case 2:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.data.SetRhsName(args[j + 1]);
                ++j;
                break;
            case 3:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.data.SetLhsName(args[j + 1]);
                ++j;
                break;
            case 4:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr) {
                    if (strcmp(args[j + 1],"PRINT_NONE")==0)
                        init.param.SetOutLvl(PRINT_NONE);
                    if (strcmp(args[j + 1],"PRINT_MIN")==0)
                        init.param.SetOutLvl(PRINT_MIN);
                    if (strcmp(args[j + 1],"PRINT_SOME")==0)
                        init.param.SetOutLvl(PRINT_SOME);
                    if (strcmp(args[j + 1],"PRINT_MORE")==0)
                        init.param.SetOutLvl(PRINT_MORE);
                    if (strcmp(args[j + 1],"PRINT_MAX")==0)
                        init.param.SetOutLvl(PRINT_MAX);
                }
                ++j;
                break;
            case 5:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.param.SetMaxIter(atoi(args[j + 1]));
                ++j;
                break;
            case 6:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.param.SetRelTol(atof(args[j + 1]));
                ++j;
                break;
            case 7:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.param.SetAbsTol(atof(args[j + 1]));
                ++j;
                break;
            case 8:
                if (args[j + 1][0] != '-' && args[j + 1] != nullptr)
                    init.param.SetRestart(atoi(args[j + 1]));
                ++j;
                break;
            default:
                std::cout << "### ERROR: bad file operands!          " << std::endl;
                std::cout << "Try ./*.exe -help for more information." << std::endl;
                retCode = FaspRetCode::ERROR_INPUT_PAR;
                return retCode;
        }
    }

    if ( NoInput ) {
        std::cout << "### ERROR: Missing matrix data file " << std::endl;
        retCode = FaspRetCode::ERROR_INPUT_PAR;
    }

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/