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

// FASPXX header files
#include "Krylov.hxx"
#include "SOL.hxx"

/// All supported Krylov methods can be accessed using this interface
FaspRetCode Krylov(LOP &A, VEC &b, VEC &x, SOL &pc, SOLParams &params) {
    SOL solver;
    solver.SetSolTypeFromName(params); // get solver type
    auto sol = &solver;

    switch (params.type) {
        case SOLType::CG:
            sol = new class CG();
            break;
        case SOLType::BICGSTAB:
            sol = new class BiCGStab();
            break;
        default:
            // Set default solver, should never reach here!!!
            if (params.verbose > PRINT_NONE)
                FASPXX_WARNING("Unknown Krylov method! Use CG instead!");
            sol = new class CG();
    }

    sol->SetOutput(params.verbose);
    sol->SetMaxIter(params.maxIter);
    sol->SetMinIter(params.minIter);
    sol->SetRestart(params.restart);
    sol->SetRelTol(params.relTol);
    sol->SetAbsTol(params.absTol);
    sol->SetSafeIter(params.safeIter);
    sol->Setup(A);
    sol->SetPC(pc);

    return sol->Solve(b, x);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/