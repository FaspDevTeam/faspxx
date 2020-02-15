/*! \file    Krylov.cxx
 *  \brief   General interface for Krylov subspace methods
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Dec/27/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "SOL.hxx"
#include "Krylov.hxx"

/// All supported Krylov methods can be accessed using this interface
FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, SOL& pc, SOLParams& params)
{
    FaspRetCode retCode;

    SOL solver;
    solver.SetSolTypeFromName(params); // set solver type

    auto sol = &solver;

    switch (params.type) {
        case SOLType::CG :
            sol = new class CG();
            sol->SetOutput(params.verbose);
            sol->SetMaxIter(params.maxIter);
            sol->SetMinIter(params.minIter);
            sol->SetRestart(params.restart);
            sol->SetRelTol(params.relTol);
            sol->SetAbsTol(params.absTol);
            sol->SetSafeIter(params.safeIter);
            sol->Setup(A);
            sol->SetPC(pc);
            retCode = sol->Solve(b, x);
            break;
        case SOLType::BICGSTAB :
            sol = new class BiCGStab();
            sol->SetOutput(params.verbose);
            sol->SetMaxIter(params.maxIter);
            sol->SetMinIter(params.minIter);
            sol->SetRestart(params.restart);
            sol->SetRelTol(params.relTol);
            sol->SetAbsTol(params.absTol);
            sol->SetSafeIter(params.safeIter);
            sol->Setup(A);
            sol->SetPC(pc);
            retCode = sol->Solve(b, x);
            break;
        default: // should never reach here!!!
            if ( params.verbose > PRINT_NONE )
                FASPXX_WARNING("Unknown Krylov method type")
            std::cout << sol->GetSolType(params.type) << "is not supported!\n";
    }

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/