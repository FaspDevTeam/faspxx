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

/// All supported Krylov methods can be accessed using this interface
FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, SOL& pcd, SOLParams& params)
{
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
        case SOLType::GMRES:
            sol = new class GMRES();
            break;
        case SOLType::FGMRES:
            sol = new class FGMRES();
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
    sol->SetSavIter(params.savIter);
    sol->SetRestart(params.restart);
    sol->SetRelTol(params.relTol);
    sol->SetAbsTol(params.absTol);
    sol->Setup(A);
    sol->SetupPCD(pcd);

    return sol->Solve(b, x);
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Dec/27/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/