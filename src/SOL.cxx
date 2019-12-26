/*! \file    SOL.cxx
 *  \brief   Iterative solver class definition
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <sstream>
#include <cstring>
#include "SOL.hxx"
#include "utils.hxx"

//! Warning for actual relative residual
void SOL::WarnRealRes(double relres) const
{
    std::cout << "### WARNING: The actual relative residual = " << relres << '\n';
}

//! Warning for computed relative residual
void SOL::WarnCompRes(double relres) const
{
    std::cout << "### WARNING: The computed relative residual = "
              << relres << std::endl;
}

//! Output relative difference and residual
void SOL::WarnDiffRes(double reldiff, double relres) const
{
    std::cout << "### WARNING: ||u-u'|| = " << reldiff
              << " and the comp. rel. res. = " << relres << std::endl;
}

/// Print out iteration information for iterative solvers
void SOL::PrintInfo(const int& iter, const double& resRel, const double& resAbs,
                    const double& ratio, std::ostream& out) const
{
    if ( params.verbose > PRINT_SOME || (params.verbose > PRINT_NONE &&
        iter%PRT_STEP_NUM == 0) ) {
        out.precision(4);
        std::setiosflags(std::ios::scientific);
        if ( iter == 0 ) { // Initial iteration
            out << "---------------------------------------------\n";
            out << " It Num | ||r||/||b|| |    ||r||    |  Ratio \n";
            out << "---------------------------------------------\n";
            out << std::setw(7) << std::right << iter << " | "
                << std::scientific << std::setprecision(5) << resRel << " | "
                << std::setw(11) << resAbs << " | " << std::endl;
        } else { // Later iterations
            out << std::setw(7) << std::right << iter << " | "
                << std::scientific << std::setprecision(5) << resRel << " | "
                << std::setw(11) << resAbs << " | "
                << std::fixed << std::setprecision(4) << ratio << std::endl;
        }
    }
}

/// Print out final status of an iterative method
void SOL::PrintFinal(std::ostream& out) const
{
    if ( params.verbose > PRINT_MIN ) {
        out << "---------------------------------------------\n";
        if ( numIter >= params.maxIter ) {
            std::cout << "### WARNING: maxIter = " << params.maxIter << " reached!"
            << '\n';
        }
        out << std::scientific << std::setprecision(5);
        out << "Number of iterations : " << numIter << '\n';
        out << "Norm2 of residual    : " << norm2   << '\n';
        out << "NormInf of residual  : " << normInf << '\n';
    } else if ( params.verbose > PRINT_NONE ) {
        out << "---------------------------------------------\n";
        out << std::scientific << std::setprecision(5);
        out << GetSolType(params.type) << " takes " << numIter << " iterations ";
        out << "to reach residual of level " << norm2 << '\n';
    }
}

/// destructor
SOL::~SOL()
{
    A = nullptr;
}

/// Set 'verbose' 's value
void SOL::SetOutput(Output verbose) 
{
    params.verbose = verbose;
}

/// Set 'maxIter' 's value
void SOL::SetMaxIter(int maxIter)
{
    params.maxIter = maxIter;
}

/// Set 'minIter' 's value
void SOL::SetMinIter(int minIter)
{
    params.minIter = minIter;
}

/// Set 'relTol' 's value
void SOL::SetRelTol(double relTol)
{
    params.relTol = relTol;
}

/// Set 'absTol' 's value
void SOL::SetAbsTol(double absTol)
{
    params.absTol = absTol;
}

/// Set 'restart' 's value
void SOL::SetRestart(int restart)
{
    params.restart = restart;
}

/// Set 'solver' type
void SOL::SetSolType(SOLType type) 
{
    params.type = type;
}

/// Set safe iteration
void SOL::SetSafeIteration(int safeIter){
    params.safeIter=safeIter;
}



/// Select solver
const char *SOL::GetSolType(SOLType type) const
{
    switch (type) {
        case CG:
            return "CG";
        case BICGSTAB:
            return "BiCGStab";
        case MINRES:
            return "MinRes";
        case GMRES:
            return "GMRES";
        case FGMRES:
            return "FGMRES";
        case VFGMRES:
            return "VFGMRES";
        default:
            return "Unknown solver type!";
    }
}

/// Get residual 's 'l2-norm'
double SOL::GetNorm2() const
{
    return this->norm2;
}

/// Get residual 's 'infty-norm'
double SOL::GetInfNorm() const
{
    return this->normInf;
}

/// Get iterations
int SOL::GetIterations() const
{
    return this->numIter;
}

/// Print parameters
void SOL::PrintParam(std::ostream& out) const
{
    out << "\nParameters for " << GetSolType(params.type) << " method\n"
        << "---------------------------------------------\n"
        << "    Max num of iteration: " << params.maxIter  << '\n'
        << "    Min num of iteration: " << params.minIter  << '\n'
        << "    Safe-guard iteration: " << params.safeIter << '\n'
        << "    Relative tolerance:   " << params.relTol   << '\n'
        << "    Absolute tolerance:   " << params.absTol   << '\n'
        << "    Restart number:       " << params.restart  << '\n'
        << "    Output level:         " << params.verbose  << '\n' << "\n";
};

/// Set 'verbose', 'maxIter', 'relTol', 'absTol', 'restart' 's values from file
void SOL::SetSolFromFile(const char *file, const char *prefix) 
{

    std::ifstream input(file);
    if ( !input.is_open() ) FASPXX_WARNING("Not found sol option file!");
    if ( params.verbose > PRINT_MIN )
        std::cout << "Reading SOL parameters from disk file " << file << std::endl;

    std::string line, param, value;
    while (std::getline(input, line)) {
        if ( !ProcessLine(line, param, value) ) continue;

        if ( prefix ) {
            if ( param.find(prefix) == param.npos ) continue;
        }

        if ( prefix ) {
            std::string prefix_(prefix);
            size_t pos = param.find(prefix_);
            if ( pos != std::string::npos ) param.erase(pos, prefix_.length());
        } else {
            param[0] = '_'; // 把-sol改成_sol,方便下面的赋值
        }

        if ( param == "_sol_type" ) {
            if (value == "cg") params.type = CG;
            else if (value == "gmres") params.type = GMRES;
            else if (value == "none") params.type = CG;
            else params.type = CG;
        } else if (param == "_sol_rtol") {
            params.relTol = std::stod(value);
        } else if (param == "_sol_atol") {
            params.absTol = std::stod(value);
        } else if (param == "_sol_maxiter") {
            params.maxIter = std::stoi(value);
        } else if (param == "_sol_printlvl") {
            //verbose = std::stoi(value);
            if (value == "PRINT_NONE")
                params.verbose = PRINT_NONE;
            else if (value == "PRINT_MIN")
                params.verbose = PRINT_MIN;
            else if (value == "PRINT_SOME")
                params.verbose = PRINT_SOME;
            else if (value == "PRINT_MORE")
                params.verbose = PRINT_MORE;
            else if (value == "PRINT_MAX")
                params.verbose = PRINT_MAX;
            else if (value == "PRINT_ALL")
                params.verbose = PRINT_ALL;
        } else if (param == "_sol_restart") {
            params.restart = std::stoi(value);
        }
    }
    input.close();
    if ( params.verbose > PRINT_MIN ) {
        std::cout << "Parameters for sol "
                  << "Krylov method type: " << params.type << '\n'
                  << "relative tolerance: " << params.relTol << '\n'
                  << "absolute tolerance: " << params.absTol << '\n'
                  << "maximum iterations: " << params.maxIter << '\n';
    }

}

/// build preconditioner operator
void SOL::SetPC(SOL *pc)
{
    this->pc = pc;
    withPC = true;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/