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

/// Warning for actual relative residual.
void SOL::WarnRealRes(double relres) const
{
    std::cout << "### WARNING: The actual relative residual = " << relres << '\n';
}

/// Warning for computed relative residual.
void SOL::WarnCompRes(double relres) const
{
    std::cout << "### WARNING: The computed relative residual = "
              << relres << std::endl;
}

/// Output relative difference and residual.
void SOL::WarnDiffRes(double reldiff, double relres) const
{
    std::cout << "### WARNING: ||u-u'|| = " << reldiff
              << " and the comp. rel. res. = " << relres << std::endl;
}

/// Print out iteration information table head.
void SOL::PrintHead(std::ostream& out) const
{
    if ( params.verbose >= PRINT_MIN ) {
        out << "---------------------------------------------\n";
        out << " It Num | ||r||/||b|| |    ||r||    |  Ratio \n";
        out << "---------------------------------------------\n";
    }
}

/// Print out iteration information for iterative solvers
void SOL::PrintInfo(const int& iter, const double& resRel, const double& resAbs,
                    const double& ratio, std::ostream& out) const
{
    if ( params.verbose > PRINT_SOME
    || ( params.verbose >= PRINT_MIN && iter%PRT_STEP_NUM == 0)
    || ( params.verbose >= PRINT_MIN && iter == params.maxIter) ) {
        out.precision(4);
        std::setiosflags(std::ios::scientific);
        if ( iter == params.minIter ) { // Initial iteration
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

/// Print out final status of an iterative method.
void SOL::PrintFinal(const int& iter, const double& resRel, const double& resAbs,
                     const double& ratio, std::ostream& out) const
{
    if ( params.verbose >= PRINT_MIN ) {
        if ( iter > params.minIter ) {
            out << std::setw(7) << std::right << iter << " | "
                << std::scientific << std::setprecision(5) << resRel << " | "
                << std::setw(11) << resAbs << " | "
                << std::fixed << std::setprecision(4) << ratio << std::endl;
        } else {
            out << std::setw(7) << std::right << iter << " | "
                << std::scientific << std::setprecision(5) << resRel << " | "
                << std::setw(11) << resAbs << " | " << std::endl;
        }
    }

    if ( params.verbose >= PRINT_MIN ) {
        out << "---------------------------------------------\n";
        if ( numIter >= params.maxIter )
            std::cout << "### WARNING: maxIter = " << params.maxIter << " reached!\n";
        out << std::scientific << std::setprecision(5);
        out << "Number of iterations : " << numIter << '\n';
        out << "Norm2 of residual    : " << norm2   << '\n';
        out << "NormInf of residual  : " << normInf << '\n';
    } else if ( params.verbose > PRINT_NONE ) {
        out << std::scientific << std::setprecision(5);
        out << GetSolType(params.type) << " takes " << numIter << " iterations";
        out << " to reach L2-norm of residual " << norm2 << '\n';
    }
}

/// Default destructor.
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

/// Set safe iteration
void SOL::SetSafeIter(int safeIter)
{
    params.safeIter = safeIter;
}

/// Set 'restart' 's value
void SOL::SetRestart(int restart)
{
    params.restart = restart;
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

/// Set 'solver' type
void SOL::SetSolTypeFromName(SOLParams& params)
{
    for ( char & c : params.algName ) c = tolower(c); // Change to lowercase
    if ( params.algName == "cg" )
        params.type = SOLType::CG;
    else if ( params.algName == "bicgstab" )
        params.type = SOLType::BICGSTAB;
    else { // default solver type
        params.type = SOLType::CG;
        if ( params.verbose > PRINT_NONE )
            FASPXX_WARNING("Unknown solver type. Using CG instead!");
    }
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

/// Build preconditioner operator
void SOL::SetPC(SOL *pc)
{
    this->pc = pc;
    withPC = true;
}

/// Set 'solver' type
void SOL::SetSolType(SOLType type)
{
    params.type = type;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/