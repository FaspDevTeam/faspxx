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

/// Setup the one-level "MG" method using linear operator A
template <class TTT>
FaspRetCode MG<TTT>::SetupSimple(const TTT& A)
{
    const INT probSize = A.GetColSize();
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
    work.SetValues(probSize, 0.0);
    infoHL[0].fineSpaceSize = infoHL[0].coarSpaceSize = probSize;
    infoHL[0].b.SetValues(infoHL[0].coarSpaceSize, 0.0);
    infoHL[0].x.SetValues(infoHL[0].coarSpaceSize, 0.0);
    infoHL[0].work.SetValues(infoHL[0].coarSpaceSize, 0.0);

    // Step 2. Set transfer operators and problems for all levels
    // infoHL[0].prolongation = &defaultTrans[0]; // set prolongation as identity
    // infoHL[0].restriction  = &defaultTrans[0]; // set restriction as identity

    // Setp 3. Set solvers for all levels
    // defaultSolvers[0].Setup(A);
    // infoHL[0].preSolver  = &defaultSolvers[0]; // presmoother
    // infoHL[0].postSolver = &defaultSolvers[0]; // postsmoother
    // defaultCoarseSolvers[0].Setup(A);
    // infoHL[0].coarseSolver = &defaultCoarseSolvers[0]; // solver at coarsest level

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

template <class TTT>
void MG<TTT>::MGCycle(const VEC& b, VEC& x)
{
    ++level;

    // apply pre-smoothing solver
    infoHL[level].preSolver->Solve(b, x);

    // skip coarse levels if there is only one level
    if (numLevelsCoarse > 0) {
        // form residual r = b - A x
        A->Residual(b, x, work);

        // restrict residual to coarser level r1 = R*r0
        infoHL[level].restriction->Apply(work, infoHL[level].b);

        // set coarse initial guess to zero
        infoHL[level].x.SetValues(infoHL[level].coarSpaceSize, 0.0);

        // call the coarse space solver
        infoHL[level].coarseSolver->Solve(infoHL[level].b, infoHL[level].x);

        // prolongate the coarse correction back x = x + P*e1
        infoHL[level].prolongation->Apply(infoHL[level].x, work);
        x.AXPY(1.0, work);
    }

    // apply post-smoothing solver
    infoHL[level].postSolver->Solve(b, x);

    --level;
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
        A->Residual(b, x, work);

        // Compute norm of residual and check whether it converges
        if (numIter >= params.minIter) {
            resAbs = work.Norm2();
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
        A->Residual(b, x, work); // Update final residual
        this->norm2 = resAbs = work.Norm2();
        this->normInf        = work.NormInf();
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

template class MG<LOP>;
template class MG<MAT>;

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/12/2021      Create file                          */
/*  Chensong Zhang      Sep/27/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/