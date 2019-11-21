/**
 * SOL source code file
 */

#include "SOL.hxx"
#include "utils.hxx"
#include <sstream>
#include <cstring>

//! Warning for actual relative residual
void SOL::RealRes(DBL relres) {
    std::cout << "### WARNING: The actual relative residual = "
              << relres << std::endl;
}

//! Warning for computed relative residual
void SOL::Compres(DBL relres) {
    std::cout << "### WARNING: The computed relative residual = "
              << relres << std::endl;
}

//! Output relative difference and residual
void SOL::DiffRes(DBL reldiff, DBL relres) {
    std::cout << "||u-u'|| = " << reldiff <<
              " and the comp. rel. res. = " << relres << std::endl;
}

/// Print out iteration information for iterative solvers
void SOL::PrintInfo(const PRTLVL &verbose, const INT &iter, const DBL &resRel,
                    const DBL &resAbs, const DBL &factor) {
    if (verbose > PRINT_SOME || (verbose > PRINT_NONE && iter % 20 == 0)) {
        if (iter == 0)
        {
            std::cout << "--------------------------------------------------\n";
            std::cout << "It Num | ||r||/||b|| |    ||r||    | Conv. Factor \n";
            std::cout << "--------------------------------------------------\n";
        } // end if iter
        std::cout.precision(4);
        std::setiosflags(std::ios::scientific);
        std::cout << std::setw(6) << iter << " | "
                  << std::scientific << std::setprecision(5) << resRel << " | "
                  << std::setw(11) << resAbs << " | "
                  << std::fixed << std::setprecision(5) << factor
                  << std::endl;
    }
}

/// Print out final status of an iterative method
void SOL::PrintFinal(const PRTLVL &verbose, const INT &iter, const INT &maxit,
                     const DBL &resRel) {
    if (verbose > PRINT_NONE) {
        if (iter > maxit)
            std::cout << "### WARNING: MaxIt = " << maxit
                      << " reached with relative residual " << resRel << std::endl;
        else if (iter >= 0) {
            std::cout << "Number of iterations = " << iter
                      << " with relative residual "
                      << resRel << std::endl;
        }
    }
}

/// constructor
SOL::SOL(PRTLVL verbose, INT maxIter, DBL relTol, DBL absTol, INT restart) {
    this->verbose = verbose;
    this->maxIter = maxIter;
    this->relTol = relTol;
    this->absTol = absTol;
    this->restart = restart;
}

/// Set 'verbose' 's value
void SOL::SetPrtLvl(PRTLVL verbose) {
    this->verbose = verbose;
}


/// Set 'maxIter' 's value
void SOL::SetMaxIter(INT maxIter) {
    this->maxIter = maxIter;
}

/// Set 'relTol' 's value
void SOL::SetRelTol(DBL relTol) {
    this->relTol = relTol;
}

/// Set 'absTol' 's value
void SOL::SetAbsTol(DBL absTol) {
    this->absTol = absTol;
}

/// Set 'restart' 's value
void SOL::SetRestart(INT restart) {
    this->restart = restart;
}

/// Set 'verbose', 'maxIter', 'relTol', 'absTol', 'restart' 's values from file
FaspRetCode SOL::SetOptionsFromFile(const char* file, const char* prefix)
{
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    std::cout << "Reading SOL parameters from disk file " << file << std::endl; //fff这类输出最好有一个全局的控制变量,比如宏 FASPXX_VERBOSE,如果把这类输出无差别地都编译(个人觉得应该用宏控制编译),会有大量的输出

//    std::ifstream in(file);
//
//    // Check whether file is openedd successfully
//    if (!in.is_open()) {
//        retCode = FaspRetCode::ERROR_OPEN_FILE;
//        return retCode;
//    }
//
//    // compute total bytes of file
//    in.seekg(0, std::ios::end);
//    long long int length = in.tellg();
//    in.seekg(0, std::ios::beg);
//
//    char* buffer; // char * fff char*
//    char decimal[128];
//    long long int position; // mark the position of file pointer
//
//    try { // catch bad allocation error if it happen
//        buffer = new char[length]; // apply for dynamic memory
//    } catch (std::bad_alloc &ex) {
//        in.close();
//        retCode = FaspRetCode::ERROR_ALLOC_MEM;
//        return retCode;
//    }
//
//    in.read(buffer,length); // read the total bytes of file
//    in.close(); // close the file pointer

    std::ifstream input(file);
    if (!input.is_open())
    {
        FASPXX_WARNING("Not found sol option file!");
    }

    // TODO: 把下面的对每行处理的代码写成一个独立的函数
    std::string line, param, value;
    while (std::getline(input, line))
    {
        if (!ProcessLine(line, param, value)) continue;

        if (prefix)
        {
            if (param.find(prefix) == param.npos) continue;
        }

        if (prefix)
        {
            std::string prefix_(prefix);
            size_t pos = param.find(prefix_);
            if (pos != std::string::npos)
            {
                param.erase(pos, prefix_.length());
            }
        }
        else
        {
            param[0] = '_'; // 把-sol改成_sol,方便下面的赋值
        }

        if (param == "_sol_type")
        {
            if (value == "cg")         type = _PCG;
            else if (value == "gmres") type = _GMRES;
            else if (value == "none")  type = _COUNT;
            else                       type = _COUNT;
        }
        else if (param == "_sol_rtol")
        {
            relTol = std::stod(value);
        }
        else if (param == "_sol_atol")
        {
            absTol = std::stod(value);
        }
        else if (param == "_sol_maxiter")
        {
            maxIter = std::stoi(value);
        }
        else if (param == "_sol_printlvl")
        {
            //verbose = std::stoi(value);
            if(value=="PRINT_NONE")
                verbose=PRINT_NONE;
            else if(value=="PRINT_MIN")
                verbose=PRINT_MIN;
            else if(value=="PRINT_SOME")
                verbose=PRINT_SOME;
            else if(value=="PRINT_MORE")
                verbose=PRINT_MORE;
            else if(value=="PRINT_MAX")
                verbose=PRINT_MAX;
            else if(value=="PRINT_ALL")
                verbose=PRINT_ALL;
        }
        else if (param == "_sol_restart")
        {
            restart = std::stoi(value);
        }
        else if (param == "_sol_view")
        {
            view = true;
        }
//        else
//        {
//            FASPXX_WARNING("Not supported parameter!");
//        }
    }
    input.close();
    if (view)
    {
        cout << "Parameters for sol " //<< prefix << '\n'
            << "Krylov method type: " << type << '\n'
            << "relative tolerance: " << relTol << '\n'
            << "absolute tolerance: " << absTol << '\n'
            << "maximum iterations: " << maxIter << '\n'
            << "view: " << view << endl;
    }
    return retCode;
}

///// build preconditioner operator
//void SOL::SetupPCD(LOP* pc){
//    this->pc = pc;
//}

/// clean preconditioner operator
void SOL::CleanPCD(){
    if(pc != nullptr)
        delete pc;
}