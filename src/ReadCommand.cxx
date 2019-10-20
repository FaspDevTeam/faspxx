/**
 * a source file about reading command parameters
 */

#include <fstream>
#include <cstring>
#include "Param.hxx"
/*
FaspRetCode ReadCommand(unsigned int argc, char *args[],
                        MAT &mat, VEC &rhs, VEC &initGuess) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    switch (argc) {
        case 2: // only read matrix
            retCode = ReadCSRToMAT(args[1], mat);
            break;
        case 3: // read matrix and right hand item
            retCode = ReadCSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadVEC(args[2], rhs);
            break;
        case 4: // read matrix, right hand item and initial solution
            retCode = ReadCSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadVEC(args[2], rhs);
            if (retCode < 0) break;
            retCode = ReadVEC(args[3], initGuess);
            break;
        default: // if no data file is specified, trigger error mechanism
            std::cout << "### ERROR: No data file specified!" << std::endl;
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            break;
    }

    return retCode;
}
*/

// convert command line choices to integer
INT StringToInt(char *ch){
    INT flag=0;
    if(strcmp(ch,"-mat")==0)
        flag=1;
    else if(strcmp(ch,"-rhs")==0)
        flag=2;
    else if(strcmp(ch,"-lhs")==0)
        flag=3;
    else if(strcmp(ch,"-outLvl")==0)
        flag=4;
    else if(strcmp(ch,"-maxIter")==0)
        flag=5;
    else if(strcmp(ch,"-relTol")==0)
        flag=6;
    else if(strcmp(ch,"-absTol")==0)
        flag=7;
    else if(strcmp(ch,"-restart")==0)
        flag=8;
    else
        flag=0;

    return flag;
}

// read parameters from command lines
void ReadParam(int argc,char *args[],InitParam &init){
    for(int j=1;j<argc-1;j++){
        switch(StringToInt(args[j])){
            case 1:
                if(args[j+1][0]!='-' && args[j+1]!= nullptr)
                    init.data.SetMatName(args[j+1]);
                break;
            case 2:
                if(args[j+1][0]!='-' && args[j+1]!= nullptr)
                    init.data.SetRhsName(args[j+1]);
                break;
            case 3:
                if(args[j+1][0]!='-' && args[j+1]!= nullptr)
                    init.data.SetLhsName(args[j+1]);
                break;
            case 4:
                if(args[j+1][0]!='-' && args[j+1]!= nullptr){
                    if(args[j+1]=="PRINT_NONE")
                        init.param.SetOutLvl(PRINT_NONE);
                    if(args[j+1]=="PRINT_MIN")
                        init.param.SetOutLvl(PRINT_MIN);
                    if(args[j+1]=="PRINT_SOME")
                        init.param.SetOutLvl(PRINT_SOME);
                    if(args[j+1]=="PRINT_MORE")
                        init.param.SetOutLvl(PRINT_MORE);
                    if(args[j+1]=="PRINT_MAX")
                        init.param.SetOutLvl(PRINT_MAX);
                }
                break;
            case 5:
                if(args[j+1][0]!='-' && args[j+1]!= nullptr)
                    init.param.SetMaxIter(atoi(args[j+1]));
                break;
            case 6:
                if(args[j+1][0]!='-' && args[j+1]!= nullptr)
                    init.param.SetRelTol(atof(args[j+1]));
                break;
            case 7:
                if(args[j+1][0]!='-' && args[j+1]!=nullptr)
                    init.param.SetAbsTol(atof(args[j+1]));
                break;
            case 8:
                if(args[j+1][0]!='-' && args[j+1]!=nullptr)
                    init.param.SetRestart(atoi(args[j+1]));
                break;
            default:
                break;
        }
    }
}