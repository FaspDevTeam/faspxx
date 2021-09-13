/*! \file    MG.cxx
 *  \brief   Abstract MG class definition
 *  \author  Chensong Zhang
 *  \date    Oct/12/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "MG.hxx"
#include "Iter.hxx"
#include "LOP.hxx"

using std::vector;

void MG::oneCycleMultigrid(const VEC &b, VEC &x) {
    // local variables
    unsigned l                      = 0; // level index
    unsigned numCycle[numLevelsUse] = {0};

    // initialize linear system information
    bVectors[0] = b;
    xVectors[0] = x;

ForwardSweep:
    while (l < numLevelsUse - 1) {
        numCycle[l]++;

        // apply pre-smoothing solver
        preSolvers[l]->Solve(bVectors[l], xVectors[l]);

        // form residual r = b - A x
        A->Apply(xVectors[l], wVectors[l]);  // SpMV, main computational work
        bVectors[l].AXPY(-1.0, wVectors[l]); // b = b - A*x

        // restrict residual to coarser level r1 = R*r0
        restrictions[l]->Apply(bVectors[l], bVectors[l + 1]);

        // prepare for next coarser level
        ++l;
        xVectors[l].SetValues(sizes[l], 0.0);
    }

    // Call the coarsest space solver
    coarsestSolver->Solve(bVectors[numLevelsUse - 1], xVectors[numLevelsUse - 1]);

    ////BackwardSweep:
    while (l > 0) {
        --l;

        // prolongate to the next finer level x = x + P*e1
        prolongations[l]->Apply(xVectors[l + 1], wVectors[l]);
        xVectors[l].AXPY(1.0, wVectors[l]);

        // apply post-smoothing solver
        postSolvers[l]->Solve(bVectors[l], xVectors[l]);

        // General cycling on each level
        if (numCycle[l] < cycles[l])
            break;
        else
            numCycle[l] = 0;
    }

    if (l > 0) goto ForwardSweep;

    x = xVectors[0];
}

/// Using the multigrid method. Don't check problem sizes.
FaspRetCode MG::Solve(const VEC &b, VEC &x) {
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Declaration and definition of local variables
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;

    PrintHead();

    // Initialize iterative method
    numIter = 0;

    // Main Jacobi loop
    while (numIter < params.maxIter) {
        // Update residual r = b - A*x
        A->Residual(b, x, wVectors[0]);

        // Compute norm of residual and check whether it converges
        if (numIter >= params.minIter) {
            resAbs = wVectors[0].Norm2();
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

        ++numIter;               // iteration count
        oneCycleMultigrid(b, x); // MG cycle

        //---------------------------------------------
        // One step of Jacobi iteration ends here
        //---------------------------------------------

    } // End of main loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (not(numIter == params.minIter && numIter == params.maxIter)) {
        this->norm2   = resAbs;
        this->normInf = wVectors[0].NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    return errorCode;
}

/// Clean up temp memory allocated for MG.
void MG::Clean() {
    for (unsigned i = 0; i < numLevelsUse; ++i) {
        bVectors[i].SetValues(len, 0.0);
        xVectors[i].SetValues(len, 0.0);
        wVectors[i].SetValues(len, 0.0);
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/