/**
 * a source file about reading command parameters
 */

#include <cstring>
#include "Param.hxx"
#include "RetCode.hxx"

// print help information
void Help(){
    std::cout<<"The format of the incoming data file and parameters    "<<std::endl
             <<"is as follow and the matrix data file is necessary!    "<<std::endl
             <<"ex1 : ./*.exe -mat matrix_data_file (*.csr or *.mtx)   "<<std::endl
             <<"ex2 : ./*.exe -mat matrix_data_file -rhs rhs_data_file "<<std::endl
             <<"ex3 : ./*.exe -mat matrix_data_file -lhs lhs_data_file "<<std::endl
             <<"ex4 : ./*.exe -mat matrix_data_file -rhs rhs_data_file "<<std::endl
             <<"              -lhs lhs_data_file                       "<<std::endl
             <<"ex5 : ./*.exe -mat matrix_data_file -outLvl PRINT_NONE "<<std::endl
             <<"              -maxIter 100 -relTol 10e-6 -absTol 10e-10"<<std::endl
             <<"              -restart 20                              "<<std::endl;
    std::cout<<"Parameters after '-outLvl' may be 'PRINT_NONE',        "<<std::endl
             <<"'PRINT_MIN','PRINT_SOME','PRINT_MORE' or 'PRINT_MAX'.  "<<std::endl
             <<"'maxIter' is the maximum number of iterations.         "<<std::endl
             <<"'relTol' is the relative tolerance for iterations.     "<<std::endl
             <<"'absTol' is the absolute tolerance for iterations.     "<<std::endl
             <<"'restart' is how many steps restart after iterations.  "<<std::endl;
}

// convert command line choices to integer
INT StringToInt(char *ch){
    INT flag=0;
    if(strcmp(ch,"-help")==0)
        flag=0;
    else if(strcmp(ch,"-mat")==0)
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
        flag=-1;

    return flag;
}

// read parameters from command lines
FaspRetCode ReadParam(int argc,char *args[],InitParam &init){
    FaspRetCode retCode=FaspRetCode ::SUCCESS;
    for(int j=1;j<argc-1;j++){
        switch(StringToInt(args[j])){
            case 0:
                Help();
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
                std::cout<<"### ERROR : Missing file operand      "<<std::endl;
                std::cout<<"Try ./*.exe -help for more information"<<std::endl;
                retCode=FaspRetCode ::ERROR_INPUT_PAR;
                break;
        }
    }

    return retCode;
}