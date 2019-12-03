/*! \file    SOL.cxx
 *  \brief   Solver class definition
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

//! Warning for actual relative residual
void SOL::WarnRealRes(DBL relres) const
{
    std::cout << "### WARNING: The actual relative residual = " << relres << '\n';
}

//! Warning for computed relative residual
void SOL::WarnCompRes(DBL relres) const
{
    std::cout << "### WARNING: The computed relative residual = "
              << relres << std::endl;
}

//! Output relative difference and residual
void SOL::WarnDiffRes(DBL reldiff, DBL relres) const
{
    std::cout << "### WARNING: ||u-u'|| = " << reldiff
              << " and the comp. rel. res. = " << relres << std::endl;
}

/// Print out iteration information for iterative solvers
void SOL::PrintInfo(const INT& iter, const DBL& resRel, const DBL& resAbs,
                    const DBL& factor, std::ostream& out) const
{
    if ( verbose > PRINT_SOME || (verbose > PRINT_NONE && iter%20 == 0) ) {
        out.precision(4);
        std::setiosflags(std::ios::scientific);
        if ( iter == 0 ) { // Initial iteration
            out << "---------------------------------------------------\n";
            out << " It Num | ||r||/||b|| |    ||r||    | Conv. Factor \n";
            out << "---------------------------------------------------\n";
            out << std::setw(7) << iter << " | "
                << std::scientific << std::setprecision(5) << resRel << " | "
                << std::setw(11) << resAbs << " | " << std::endl;
        } else { // Later iterations
            out << std::setw(7) << iter << " | "
                << std::scientific << std::setprecision(5) << resRel << " | "
                << std::setw(11) << resAbs << " | "
                << std::fixed << std::setprecision(4) << factor << std::endl;
        }
    }
}

/// Print out final status of an iterative method
void SOL::PrintFinal(std::ostream& out) const
{
    if ( verbose > PRINT_NONE ) {
        out << "---------------------------------------------------\n";
        if ( numIter >= maxIter ) {
            std::cout << "### WARNING: maxIter = " << maxIter << " reached!" << '\n';
        }
        out << std::scientific << std::setprecision(5);
        out << "Number of iterations : " << numIter << '\n';
        out << "Norm2 of residual    : " << norm2   << '\n';
        out << "NormInf of residual  : " << normInf << '\n';
    }
}

/// constructor
SOL::SOL(Output verbose, INT maxIter, DBL relTol, DBL absTol, INT restart) 
{
    this->verbose = verbose;
    this->maxIter = maxIter;
    this->relTol = relTol;
    this->absTol = absTol;
    this->restart = restart;
}

/// destructor
SOL::~SOL()
{
    A = nullptr;
}

/// Set 'verbose' 's value
void SOL::SetOutput(Output verbose) 
{
    this->verbose = verbose;
}

/// Set 'maxIter' 's value
void SOL::SetMaxIter(INT maxIter) 
{
    this->maxIter = maxIter;
}

/// Set 'minIter' 's value
void SOL::SetMinIter(INT minIter)
{
    this->minIter = minIter;
}

/// Set 'relTol' 's value
void SOL::SetRelTol(DBL relTol) 
{
    this->relTol = relTol;
}

/// Set 'absTol' 's value
void SOL::SetAbsTol(DBL absTol) 
{
    this->absTol = absTol;
}

/// Set 'restart' 's value
void SOL::SetRestart(INT restart) 
{
    this->restart = restart;
}

/// Set 'solver' type
void SOL::SetSolType(SOLType type) 
{
    this->type = type;
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
DBL SOL::GetNorm2() const
{
    return this->norm2;
}

/// Get residual 's 'infty-norm'
DBL SOL::GetInfNorm() const
{
    return this->normInf;
}

/// Get iterations
INT SOL::GetIterations() const
{
    return this->numIter;
}

/// Print parameters
void SOL::PrintParam(std::ostream& out) const
{
    out << "\nParameters for iterative method: \n"
        << "-----------------------------------\n"
        << "  Max num of iteration: " << maxIter << '\n'
        << "  Min num of iteration: " << minIter << '\n'
        << "  Relative tolerance:   " << relTol  << '\n'
        << "  Absolute tolerance:   " << absTol  << '\n'
        << "  Restart number:       " << restart << '\n'
        << "  Output level:         " << verbose << '\n'
        << "-----------------------------------\n";
};

/// Process a line. Return false if the line is a comment or empty.
static bool ProcessLine(std::string& line, std::string& param, std::string& value)
{
    // Remove spaces in the beginning and end of each line
    line.erase(0, line.find_first_not_of(' '));
    line.erase(line.find_last_not_of(' ') + 1);

    if ( line.empty()   ) return false; // Skip empty lines
    if ( line[0] == '#' ) return false; // Skip comments
    if ( line[0] == '%' ) return false; // Skip comments

    std::string buff;
    std::stringstream ss(line);
    ss >> param >> value >> buff;
    return true;
}

/// Set 'verbose', 'maxIter', 'relTol', 'absTol', 'restart' 's values from file
void SOL::SetSolFromFile(const char *file, const char *prefix) 
{

    std::ifstream input(file);
    if ( !input.is_open() ) FASPXX_WARNING("Not found sol option file!");
    if ( verbose > PRINT_MIN )
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
            if (value == "cg") type = CG;
            else if (value == "gmres") type = GMRES;
            else if (value == "none") type = CG;
            else type = CG;
        } else if (param == "_sol_rtol") {
            relTol = std::stod(value);
        } else if (param == "_sol_atol") {
            absTol = std::stod(value);
        } else if (param == "_sol_maxiter") {
            maxIter = std::stoi(value);
        } else if (param == "_sol_printlvl") {
            //verbose = std::stoi(value);
            if (value == "PRINT_NONE")
                verbose = PRINT_NONE;
            else if (value == "PRINT_MIN")
                verbose = PRINT_MIN;
            else if (value == "PRINT_SOME")
                verbose = PRINT_SOME;
            else if (value == "PRINT_MORE")
                verbose = PRINT_MORE;
            else if (value == "PRINT_MAX")
                verbose = PRINT_MAX;
            else if (value == "PRINT_ALL")
                verbose = PRINT_ALL;
        } else if (param == "_sol_restart") {
            restart = std::stoi(value);
        }
    }
    input.close();
    if ( verbose > PRINT_MIN ) {
        std::cout << "Parameters for sol "
                  << "Krylov method type: " << type << '\n'
                  << "relative tolerance: " << relTol << '\n'
                  << "absolute tolerance: " << absTol << '\n'
                  << "maximum iterations: " << maxIter << '\n';
    }

}

/// build preconditioner operator
void SOL::SetPC(SOL *pc)
{
    this->pc=pc;
    mark=true;
}

/// Release temporary memory
void SOL::Clean()
{
    if (mark == false)
        delete pc;
    else
        pc = nullptr;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/