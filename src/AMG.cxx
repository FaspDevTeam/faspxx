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
#include "Iter.hxx"
#include "LOP.hxx"

/// Setup the one-level "MG" method using linear operator A
FaspRetCode AMG::SetupOneLevel(const MAT &A)
{
    len              = A.GetColSize(); // size of the solution vector
    numLevelsUse     = 1;              // only one level
    useSymmRoperator = true;           // symmetric restriction based on prolongation

    // Step 0. Allocate memory for temporary vectors
    try {
        sizes.resize(numLevelsUse);
        cycles.resize(numLevelsUse);
        preSolvers.resize(numLevelsUse);
        postSolvers.resize(numLevelsUse);
        coeffMatrices.resize(numLevelsUse);
        bVectors.resize(numLevelsUse);
        xVectors.resize(numLevelsUse);
        wVectors.resize(numLevelsUse);

        prolongations.resize(numLevelsUse - 1);
        restrictions.resize(numLevelsUse - 1);
        defaultSolvers.resize(numLevelsUse - 1);
        defaultTrans.resize(numLevelsUse - 1);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Step 1. Set problems and temp space for all levels
    sizes[0] = len;
    for (unsigned i = 0; i < numLevelsUse; ++i) {
        bVectors[i].SetValues(sizes[i], 0.0);
        xVectors[i].SetValues(sizes[i], 0.0);
        wVectors[i].SetValues(sizes[i], 0.0);
    }

    // Step 2. Set transfer operators and problems for all levels
    coeffMatrices[0] = A;
    for (unsigned i = 0; i < numLevelsUse - 1; ++i) {
        prolongations[i] = &defaultTrans[i]; // set prolongation as identity
        restrictions[i]  = &defaultTrans[i]; // set restriction as identity
        // coeffMatrices[i + 1] = ;        // TODO: form coarse level matrices
    }

    dafaultCoarsestSolver.Setup(coeffMatrices[numLevelsUse - 1]);
    coarsestSolver = &dafaultCoarsestSolver; // solver at coarsest level

    // Setp 3. Set solvers for all levels
    if (useDefaultSolver) {
        for (unsigned i = 0; i < numLevelsUse - 1; ++i) {
            defaultSolvers[i].Setup(coeffMatrices[i]);
            preSolvers[i]  = &defaultSolvers[i]; // presmoother
            postSolvers[i] = &defaultSolvers[i]; // postsmoother
        }

        dafaultCoarsestSolver.Setup(coeffMatrices[numLevelsUse - 1]);
        coarsestSolver = &dafaultCoarsestSolver; // solver at coarsest level
    } else {
        FASPXX_ABORT("User-defined smoothers not implemented yet!");
    }

    return FaspRetCode::SUCCESS;
}

/// Allocate memory, setup multigrid hierarical structure of the linear system.
FaspRetCode AMG::Setup(const MAT &A)
{
    // Set solver type
    SetSolType(SOLType::MG);

    // Allocate memory for temporary vectors
    try {
        SetupOneLevel(A); // TODO: Need to be replaced!
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_AMG_SETUP;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Clean up temp memory allocated for MG.
void AMG::Clean()
{
    for (unsigned i = 0; i < numLevelsUse; ++i) {
        bVectors[i].SetValues(len, 0.0);
        xVectors[i].SetValues(len, 0.0);
        wVectors[i].SetValues(len, 0.0);
    }
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/12/2021      Create file                          */
/*----------------------------------------------------------------------------*/