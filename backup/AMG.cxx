/*! \file    AMG.cxx
 *  \brief   Algebraic MG class definition
 *  \author  Chensong Zhang
 *  \date    Sep/12/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "AMG.hxx"

/// Setup the one-level "MG" method using linear operator A
FaspRetCode AMG::SetupOneLevel(const MAT& A)
{
    const INT probSize = A.GetColSize();
    numLevelsCoarse = 1;    // only one coarse level used
    useSymmOper     = true; // symmetric restriction based on prolongation

    // Step 0. Allocate memory for temporary vectors
    try {
        infoHL.resize(numLevelsCoarse);
        defaultTrans.resize(numLevelsCoarse);
        defaultSolvers.resize(numLevelsCoarse);
        defaultCoarseSolvers.resize(numLevelsCoarse);
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Step 1. Set problems and temp space for all levels
    work.SetValues(probSize, 0.0);
    infoHL[0].fineSpaceSize = infoHL[0].coarSpaceSize = probSize;
    infoHL[0].b.SetValues(infoHL[0].coarSpaceSize, 0.0);
    infoHL[0].x.SetValues(infoHL[0].coarSpaceSize, 0.0);
    infoHL[0].work.SetValues(infoHL[0].coarSpaceSize, 0.0);

    // Step 2. Set transfer operators and problems for all levels
    infoHL[0].prolongation = &defaultTrans[0]; // set prolongation as identity
    infoHL[0].restriction  = &defaultTrans[0]; // set restriction as identity

    // Setp 3. Set solvers for all levels
    if (useDefaultSolver) {
        defaultSolvers[0].Setup(A);
        infoHL[0].preSolver  = &defaultSolvers[0]; // presmoother
        infoHL[0].postSolver = &defaultSolvers[0]; // postsmoother
        defaultCoarseSolvers[0].Setup(A);
        infoHL[0].coarseSolver = &defaultCoarseSolvers[0]; // solver at coarsest level
    } else {
        FASPXX_ABORT("User-defined smoothers not implemented yet!");
    }

    return FaspRetCode::SUCCESS;
}

/// Allocate memory, setup multigrid hierarical structure of the linear system.
FaspRetCode AMG::Setup(const MAT& A)
{
    // Set solver type
    SetSolType(SOLType::MG);

    // Allocate memory for temporary vectors
    try {
        SetupOneLevel(A); // TODO: Need to be replaced!
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_AMG_SETUP;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Clean up temp memory allocated for MG.
void AMG::Clean() {}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/12/2021      Create file                          */
/*----------------------------------------------------------------------------*/