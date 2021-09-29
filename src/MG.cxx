/*! \file    MG.cxx
 *  \brief   Abstract MG class definition
 *  \author  Chensong Zhang
 *  \date    Sep/12/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "MG.hxx"

using std::vector;

/// Set number of cycles for each coarse level
template <class TTT>
void MG<TTT>::SetNumCycles(USI ncycle)
{
    numCycles.resize(numLevelsCoarse);
    for (USI i = 0; i < numLevelsCoarse; ++i) numCycles[i] = ncycle;
}

/// Setup the one-level "MG" method using linear operator A
template <class TTT>
FaspRetCode MG<TTT>::SetupSimple(const TTT& A)
{
    const USI probSize = A.GetColSize();
    numLevelsCoarse    = 1;    // only one coarse level used
    useSymmOper        = true; // symmetric restriction based on prolongation

    // Step 0. Allocate memory for temporary vectors
    try {
        infoHL.resize(numLevelsCoarse);
        // defaultTrans.resize(numLevelsCoarse);
        // defaultSolvers.resize(numLevelsCoarse);
        // defaultCoarseSolvers.resize(numLevelsCoarse);
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Step 1. Set problems and temp space for all levels
    r.SetValues(probSize, 0.0);
    infoHL[0].fineSpaceSize = infoHL[0].coarSpaceSize = probSize;
    infoHL[0].b.SetValues(infoHL[0].coarSpaceSize, 0.0);
    infoHL[0].x.SetValues(infoHL[0].coarSpaceSize, 0.0);
    infoHL[0].r.SetValues(infoHL[0].coarSpaceSize, 0.0);

    // Step 2. Set transfer operators and problems for all levels
    // infoHL[i].prolongation = &defaultTrans[i]; // set prolongation as identity
    // infoHL[i].restriction  = &defaultTrans[i]; // set restriction as identity

    // Setp 3. Set solvers for all levels
    // defaultSolvers[i].Setup(A);
    // infoHL[i].preSolver  = &defaultSolvers[i]; // presmoother
    // infoHL[i].postSolver = &defaultSolvers[i]; // postsmoother
    // defaultCoarseSolvers[i].Setup(A);
    // infoHL[i].coarseSolver = &defaultCoarseSolvers[i]; // solver at coarsest level

    return FaspRetCode::SUCCESS;
}

/// Setup multilevel solver level by level.
template <class TTT>
FaspRetCode MG<TTT>::SetupLevel(const TTT& A, const USI level, TTT* tranOpers,
                                SOL* smoothers, SOL* coarseSolvers)
{
    if (level > numLevelsCoarse) FASPXX_ABORT("Too many levels specified!");

    // Step 1. Set problem sizes and work spaces for coarse levels
    infoHL[level].fineSpaceSize = tranOpers->GetColSize();
    infoHL[level].coarSpaceSize = tranOpers->GetRowSize();
    infoHL[level].b.SetValues(infoHL[level].coarSpaceSize, 0.0);
    infoHL[level].x.SetValues(infoHL[level].coarSpaceSize, 0.0);
    infoHL[level].r.SetValues(infoHL[level].coarSpaceSize, 0.0);

    // Step 2. Set transfer operators and problems for all levels
    infoHL[level].prolongation = tranOpers; // set prolongation as identity
    infoHL[level].restriction  = tranOpers; // set restriction as identity'

    // Setp 3. Set smoothers for all levels
    infoHL[level].preSolver = smoothers; // set presmoother
    infoHL[level].postSolver = smoothers; // set postsmoother

    // Setp 4. Set coarse solvers for all levels
    infoHL[level].coarseSolver = coarseSolvers; // set coarse solver

    return FaspRetCode::SUCCESS;
}

/// Setup the one-level "MG" method using linear operator A
template <class TTT>
FaspRetCode MG<TTT>::SetupALL(const TTT& A, const USI numLevels)
{
    const USI probSize = A.GetColSize();
    useSymmOper        = true;      // symmetric restriction based on prolongation
    numLevelsCoarse    = numLevels; // only one coarse level used

    // Set solver type
    SetSolType(SOLType::SOLVER_MG);

    // Step 0. Allocate memory for temporary vectors
    try {
        infoHL.resize(numLevelsCoarse);
        r.SetValues(probSize, 0.0);
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Allocate memory, setup multigrid hierarical structure of the linear system.
template <class TTT>
FaspRetCode MG<TTT>::Setup(const TTT& A)
{
    // Set solver type
    SetSolType(SOLType::SOLVER_MG);

    // Allocate memory for temporary vectors
    try {
        SetupSimple(A); // TODO: Need to be replaced!
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_AMG_SETUP;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// One multigrid V or W or variable cycle.
template <class TTT>
void MG<TTT>::MGCycle(const VEC& b, VEC& x)
{
    if (numLevelsCoarse > 0) {

        ++level;

        // return if out of HL range
        if (level - numLevelsCoarse == 0) return;

        // pre-smoothing
        infoHL[level].preSolver->Solve(b, x);

        // form residual r = b - A x
        A->Residual(b, x, r);

        // restrict residual to coarser level r1 = R*r0
        infoHL[level].restriction->Apply(r, infoHL[level].b);

        // prepare for coarser level
        infoHL[level].x.SetValues(infoHL[level].coarSpaceSize, 0.0);

        if (level + 1 - numLevelsCoarse == 0) {
            // call coarsest space solver
            infoHL[level].coarseSolver->Solve(infoHL[level].b, infoHL[level].x);
        } else {
            // call multigrid cycle recursivley
            MGCycle(infoHL[level].b, infoHL[level].x);
        }

        // prolongation P*e1
        infoHL[level].prolongation->Apply(infoHL[level].x, r);

        // correction x = x + P*e1
        x.AXPY(1.0, r);

        // post-smoothing
        infoHL[level].postSolver->Solve(b, x);

        --level;

    } else {
        // if no HL information, skip MG cycle and return
        FASPXX_WARNING("No multilevel info available! Do nothing!!!");
        x = b;
    }
}

/// Using the multigrid method. Don't check problem sizes.
template <class TTT>
FaspRetCode MG<TTT>::Solve(const VEC& b, VEC& x)
{
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;

    PrintHead();

    // Initialize iterative method
    numIter = 0;

    // Main MG loop
    while (numIter < params.maxIter) {

        // Update residual r = b - A*x
        A->Residual(b, x, r);

        // Compute norm of residual and check whether it converges
        if (numIter >= params.minIter) {
            resAbs = r.Norm2();
            if (numIter == params.minIter)
                denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if (resRel < params.relTol || resAbs < params.absTol) break;

            ratio     = resAbs / resAbsOld;
            resAbsOld = resAbs;
            PrintInfo(numIter, resRel, resAbs, ratio);
        }

        //---------------------------------------------
        // Multigrid iteration starts from here
        //---------------------------------------------

        MGCycle(b, x); // MG cycle
        ++numIter;     // iteration count

        //---------------------------------------------
        // One step of Multigrid iteration ends here
        //---------------------------------------------
    } // End of main loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (not(numIter == params.minIter && numIter == params.maxIter)) {
        A->Residual(b, x, r); // Update final residual
        this->norm2 = resAbs = r.Norm2();
        this->normInf        = r.NormInf();
        resRel               = resAbs / denAbs;
        ratio                = resAbs / resAbsOld;
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    return errorCode;
}

/// Clean up temp memory allocated for MG.
template <class TTT>
void MG<TTT>::Clean()
{
    // TODO: do something here
}

// Explicitly instantiate the MG template
// template class MG<LOP>;
template class MG<MAT>;

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/12/2021      Create file                          */
/*  Chensong Zhang      Sep/29/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/