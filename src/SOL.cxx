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

// FASPXX header files
#include "SOL.hxx"

/// Warning for actual relative residual.
void SOL::WarnRealRes(double relres) const
{
    std::cout << "### WARNING: The actual relative residual = " << relres << '\n';
}

/// Warning for computed relative residual.
void SOL::WarnCompRes(double relres) const
{
    std::cout << "### WARNING: The computed relative residual = " << relres
              << std::endl;
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
    if (params.verbose >= PRINT_MIN && params.minIter < params.maxIter) {
        out << "---------------------------------------------\n";
        out << " It Num | ||r||/||b|| |    ||r||    |  Ratio \n";
        out << "---------------------------------------------\n";
    }
}

/// Print out iteration information for iterative solvers
void SOL::PrintInfo(const int& iter, const double& resRel, const double& resAbs,
                    const double& ratio, std::ostream& out) const
{
    if (params.verbose > PRINT_SOME ||
        (params.verbose >= PRINT_MIN && iter % PRT_STEP_NUM == 0) ||
        (params.verbose >= PRINT_MIN && iter == params.maxIter)) {
        out.precision(4);
        std::setiosflags(std::ios::scientific);
        if (iter == params.minIter) { // Initial iteration
            out << std::setw(7) << std::right << iter << " | " << std::scientific
                << std::setprecision(5) << resRel << " | " << std::setw(11) << resAbs
                << " | " << std::endl;
        } else { // Later iterations
            out << std::setw(7) << std::right << iter << " | " << std::scientific
                << std::setprecision(5) << resRel << " | " << std::setw(11) << resAbs
                << " | " << std::fixed << std::setprecision(4) << ratio << std::endl;
        }
    }
}

/// Print out final status of an iterative method.
void SOL::PrintFinal(const int& iter, const double& resRel, const double& resAbs,
                     const double& ratio, std::ostream& out) const
{
    if (params.minIter >= params.maxIter) return;

    if (params.verbose >= PRINT_MIN) {
        if (iter > params.minIter) {
            out << std::setw(7) << std::right << iter << " | " << std::scientific
                << std::setprecision(5) << resRel << " | " << std::setw(11) << resAbs
                << " | " << std::fixed << std::setprecision(4) << ratio << std::endl;
        } else {
            out << std::setw(7) << std::right << iter << " | " << std::scientific
                << std::setprecision(5) << resRel << " | " << std::setw(11) << resAbs
                << " | " << std::endl;
        }
    }

    if (params.verbose >= PRINT_MIN) {
        out << "---------------------------------------------\n";
        if (numIter >= params.maxIter)
            std::cout << "### WARNING: maxIter = " << params.maxIter << " reached!\n";
        out << std::scientific << std::setprecision(5);
        out << "Number of iterations : " << numIter << '\n';
        out << "Norm2 of residual    : " << norm2 << '\n';
        out << "NormInf of residual  : " << normInf << '\n';
    } else if (params.verbose > PRINT_NONE) {
        out << std::scientific << std::setprecision(5);
        out << GetSolType(params.type) << " takes " << numIter << " iterations";
        out << " to reach L2-norm of residual " << norm2 << '\n';
    }
}

/// Default destructor.
SOL::~SOL() { A = nullptr; }

/// Set output level verbose.
void SOL::SetOutput(Output verbose) { params.verbose = verbose; }

/// Set value for maxIter.
void SOL::SetMaxIter(int maxIter) { params.maxIter = maxIter; }

/// Set value for minIter.
void SOL::SetMinIter(int minIter) { params.minIter = minIter; }

/// Set value for safeIter.
void SOL::SetSafeIter(int safeIter) { params.safeIter = safeIter; }

/// Set value for restart.
void SOL::SetRestart(int restart) { params.restart = restart; }

/// Set value for relTol.
void SOL::SetRelTol(double relTol) { params.relTol = relTol; }

/// Set value for absTol.
void SOL::SetAbsTol(double absTol) { params.absTol = absTol; }

/// Set value for weight.
void SOL::SetWeight(double alpha) { params.weight = alpha; }

/// Set SOLType.
void SOL::SetSolType(SOLType type) { params.type = type; }

/// Set value for SOLType using algName.
void SOL::SetSolTypeFromName(SOLParams& params)
{
    for (char& c : params.algName) c = std::tolower(c); // Change to lowercase
    if (params.algName == "cg")
        params.type = SOLType::CG;
    else if (params.algName == "bicgstab")
        params.type = SOLType::BICGSTAB;
    else if (params.algName == "minres")
        params.type = SOLType::MINRES;
    else if (params.algName == "gmres")
        params.type = SOLType::GMRES;
    else if (params.algName == "fgmres")
        params.type = SOLType::FGMRES;
    else if (params.algName == "vfgmres")
        params.type = SOLType::VFGMRES;
    else if (params.algName == "jacobi")
        params.type = SOLType::Jacobi;
    else if (params.algName == "gs")
        params.type = SOLType::GS;
    else if (params.algName == "sgs")
        params.type = SOLType::SGS;
    else if (params.algName == "sor")
        params.type = SOLType::SOR;
    else if (params.algName == "ssor")
        params.type = SOLType::SSOR;
    else if (params.algName == "mg")
        params.type = SOLType::MG;
    else if (params.algName == "fmg")
        params.type = SOLType::FMG;
    else {
        params.type = SOLType::CG; // default solver type
        if (params.verbose > PRINT_NONE)
            FASPXX_WARNING("Unknown solver type. Using default solver!");
    }
}

/// Get solver type.
const char* SOL::GetSolType(SOLType type) const
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
        case Jacobi:
            return "Jacobi";
        case GS:
            return "GS";
        case SGS:
            return "SGS";
        case SOR:
            return "SOR";
        case SSOR:
            return "SSOR";
        case MG:
            return "MG";
        case FMG:
            return "FMG";
        default:
            FASPXX_ABORT("Unknown solver type!");
    }
}

/// Get L2 norm of the residual vector.
double SOL::GetNorm2() const { return this->norm2; }

/// Get Inf norm of the residual vector.
double SOL::GetInfNorm() const { return this->normInf; }

/// Get the value of numIter.
int SOL::GetIterations() const { return this->numIter; }

/// Print parameters.
void SOL::PrintParam(std::ostream& out) const
{
    // General solver parameters
    out << "\n---------------------------------------------\n"
        << GetSolType(params.type) << " solver with the following parameters"
        << "\n---------------------------------------------\n"
        << "    Output level:         " << params.verbose << "\n"
        << "    Max num of iteration: " << params.maxIter << "\n"
        << "    Min num of iteration: " << params.minIter << "\n"
        << "    Safe-guard iteration: " << params.safeIter << "\n"
        << std::scientific << std::setprecision(5)
        << "    Relative tolerance:   " << params.relTol << "\n"
        << "    Absolute tolerance:   " << params.absTol << "\n";

    // Parameters for Krylov solvers
    if (0 < params.type && params.type < 10) {
        out << "    Restart number:       " << params.restart << "\n";
    }

    // Parameters for relaxation solvers
    if (10 < params.type && params.type < 20) {
        out << std::fixed << std::setprecision(3)
            << "    Relaxation weight:    " << params.weight << "\n";
    }

    out << std::endl;
}

/// Print solver time.
void SOL::PrintTime(const double duration, std::ostream& out) const
{
    if (duration < CLOCK_USE_SEC) {
        std::cout << "Solving linear system with " << GetSolType(params.type)
                  << " costs " << std::fixed << std::setprecision(3) << duration << "ms"
                  << std::endl;
    } else if (duration < CLOCK_USE_MIN) {
        std::cout << "Solving linear system with " << GetSolType(params.type)
                  << " costs " << std::fixed << std::setprecision(3)
                  << duration / 1000.0 << "s" << std::endl;
    } else {
        std::cout << "Solving linear system with " << GetSolType(params.type)
                  << " costs " << std::fixed << std::setprecision(3)
                  << duration / 60000.0 << "m" << std::endl;
    }
}

/// Build preconditioner operator.
void SOL::SetupPCD(SOL& precond) { this->pcd = &precond; }

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Nov/25/2019      Create file                          */
/*  Chensong Zhang      Sep/17/2021      Add more Krylov methods as choices   */
/*----------------------------------------------------------------------------*/