/*! \file    FGMRES.cxx
 *  \brief   Preconditioned flexible GMRES class definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    July/17/2020
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "FGMRES.hxx"

/// Set the maximum and minimum restart
void FGMRES::SetMaxMinRestart(const USI maxRestart, const USI minRestart)
{
    this->maxRestart = maxRestart;
    this->minRestart = minRestart;
}

/// Set up the FGMRES method.
FaspRetCode FGMRES::Setup(const LOP& A)
{

    // Set solver type
    SetSolType(SOLType::SOLVER_FGMRES);

    len        = A.GetColSize();
    restart    = params.restart;
    maxRestart = maxRestart > params.maxIter ? params.maxIter : maxRestart;
    minRestart = minRestart > maxRestart ? maxRestart : minRestart;

    // Allocate memory for temporary vectors
    try {
        wk.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        safe.SetValues(len, 0.0);
        var.resize(len);

        hcos.resize(maxRestart + 1);
        hsin.resize(maxRestart);
        hh.resize(maxRestart + 1);
        for (USI j = 0; j < maxRestart + 1; ++j) hh[j].resize(maxRestart);
    } catch (std::bad_alloc& ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Allocate memory for restart vectors
    while (maxRestart >= minRestart) {
        try {
            for (USI j = 0; j < maxRestart + 1; ++j) {
                V.push_back(safe);
                Z.push_back(safe);
            }
            break;
        } catch (std::bad_alloc& ex) {
            maxRestart -= decrease;
            while (!V.empty()) {
                V.resize(0);
                Z.resize(0);
            }
        }
    }
    if (maxRestart < minRestart) return FaspRetCode::ERROR_ALLOC_MEM;

    // Reset restart value according to possible range
    if (restart > maxRestart)
        restart = maxRestart;
    else if (restart < minRestart)
        restart = minRestart;

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Clean up FGMRES data allocated during setup.
void FGMRES::Clean()
{

    restart = params.restart;

    if (restart > maxRestart)
        restart = maxRestart;
    else if (restart < minRestart)
        restart = minRestart;

    wk.SetValues(len, 0.0);
    tmp.SetValues(len, 0.0);
    safe.SetValues(len, 0.0);
    var.assign(len, 0.0);

    hcos.assign(maxRestart + 1, 0.0);
    hsin.assign(maxRestart, 0.0);
    hh.resize(maxRestart + 1);
    for (USI j = 0; j < maxRestart + 1; ++j) hh[j].assign(maxRestart, 0.0);

    V.assign(maxRestart + 1, safe);
    Z.assign(maxRestart + 1, safe);
}

/// Right-preconditioned FGMRES solver.
FaspRetCode FGMRES::Solve(const VEC& b, VEC& x)
{

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // local variables
    double gamma, t, ri, rj, cr = 1.0;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;
    USI    count = 1, count_1 = 2;

    PrintHead();

    // Initialize iterative method
    numIter = 0;
    safe    = x;

    // Initialize residual norm
    A->Apply(x, V[0]);  // A * x -> V[0]
    V[0].XPAY(-1.0, b); // b - V[0] -> V[0]
    resAbs = ri = V[0].Norm2();
    denAbs      = (resAbs > CLOSE_ZERO) ? resAbs : CLOSE_ZERO;

    // FGMRES(m) outer iteration
    while (numIter < params.maxIter) {

        // Start from minIter instead of 0
        if (numIter == params.minIter) {
            resRel    = resAbs / denAbs;
            resAbsOld = resAbs;
            if (resRel < params.relTol || resAbs < params.absTol) break;
        }

        // Initial search direction: r/||r||
        if (resAbs < SMALL_DBL) break; // Resiudal is too small
        var[0] = resAbs;
        V[0].Scale(1 / resAbs);

        // RESTART CYCLE (right-preconditioning)
        count = 0;
        while (count < restart && numIter < params.maxIter) {

            if (numIter >= params.minIter) PrintInfo(numIter, resRel, resAbs, ratio);

            //---------------------------------------------
            // GMRES(m) inner iteration starts from here
            //---------------------------------------------
            ++numIter;         // total iteration number
            count_1 = count++; // inner iteration number

            // Apply preconditioner
            tmp.SetValues(len, 0.0);
            pcd->Solve(V[count_1], Z[count_1]);

            A->Apply(Z[count_1], V[count]);

            // Modified Gram-Schmidt orthogonalization
            for (USI j = 0; j < count; ++j) {
                hh[j][count_1] = V[count].Dot(V[j]);
                V[count].AXPBY(1.0, -hh[j][count_1], V[j]);
            }
            t                  = V[count].Norm2();
            hh[count][count_1] = t;

            // If t=0, we get solution subspace
            if (fabs(t) > SMALL_DBL)
                V[count].Scale(1.0 / t);
            else
                break;

            for (USI j = 1; j < count; ++j) {
                t                  = hh[j - 1][count_1];
                hh[j - 1][count_1] = hsin[j - 1] * hh[j][count_1] + hcos[j - 1] * t;
                hh[j][count_1]     = -hsin[j - 1] * t + hcos[j - 1] * hh[j][count_1];
            }

            t = hh[count][count_1] * hh[count][count_1];
            t += hh[count_1][count_1] * hh[count_1][count_1];
            t = sqrt(t);

            gamma                = t > SMALL_DBL ? t : SMALL_DBL;
            hcos[count_1]        = hh[count_1][count_1] / gamma;
            hsin[count_1]        = hh[count][count_1] / gamma;
            hh[count_1][count_1] = hsin[count_1] * hh[count][count_1] +
                                   hcos[count_1] * hh[count_1][count_1];

            var[count]   = -hsin[count_1] * var[count_1];
            var[count_1] = hcos[count_1] * var[count_1];
            resAbs       = fabs(var[count]);
            ratio        = resAbs / resAbsOld;
            resAbsOld    = resAbs;
            resRel       = resAbs / denAbs;

            // Exit restart cycle if reaches tolerance
            if ((resAbs < params.absTol || resRel < params.relTol) &&
                numIter > params.minIter)
                break;

        } // end of restart cycle

        // Compute solution, first solve upper triangular system
        var[count_1] = var[count_1] / hh[count_1][count_1];
        for (USI k = count - 2; k >= 0; --k) {
            t = 0.0;
            for (USI j = k + 1; j < count; ++j) t -= hh[k][j] * var[j];
            t += var[k];
            var[k] = t / hh[k][k];
        }

        wk = Z[count_1];
        wk.Scale(var[count_1]);

        for (USI j = count - 2; j >= 0; --j) wk.AXPBY(1.0, var[j], Z[j]);

        x.AXPY(1.0, wk);

        // Save the best solution so far
        if (numIter >= params.savIter && resAbs < resAbsOld) safe = x;

        // Check whether exceeds maximal number of iterations
        if (numIter > params.maxIter) {
            errorCode = FaspRetCode::ERROR_SOLVER_MAXIT;
            break;
        }

        //---------------------------------------------
        // Prepare for the next iteration
        //---------------------------------------------
        // Compute residual vector and continue loop
        A->Apply(x, V[0]);  // A * x -> V[0]
        V[0].XPAY(-1.0, b); // b - V[0] -> V[0]

        // Check whether converged
        resAbs = rj = V[0].Norm2();
        resRel      = resAbs / denAbs;
        if ((resRel < params.relTol || resAbs < params.absTol) &&
            numIter >= params.minIter)
            break;

        // Choose restart number adaptively
        cr = rj / ri;
        ri = rj;
        if (cr > max_cr)
            restart = maxRestart;
        else if (cr <= max_cr && cr >= min_cr) {
            if (restart - decrease > minRestart)
                restart -= decrease;
            else
                restart = maxRestart;
        }

    } // end of main while loop

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (!(numIter == params.minIter && numIter == params.maxIter)) {
        norm2   = V[0].Norm2();
        normInf = V[0].NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    // Restore the saved best iteration if needed
    if (numIter > params.savIter) x = safe;

    return errorCode;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        July/17/2020     Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/