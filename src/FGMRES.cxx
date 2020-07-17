/*! \file    FGMRES.cxx
 *  \brief   Preconditioned Flexible GMRES class definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    July/17/2020
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <stack>
#include <iostream>
#include <fstream>
#include "FGMRES.hxx"

/// Set the maximum and minimum restart
void FGMRES::SetMaxMinRestart(const int maxRestart, const int minRestart) {
    this->maxRestart = maxRestart;
    this->minRestart = minRestart;
}

/// Set up the GMRES method
FaspRetCode FGMRES::Setup(const LOP &A) {

    // Set solver type
    SetSolType(SOLType::GMRES);

    this->len        = A.GetColSize();
    this->restart    = this->params.restart;
    this->maxRestart = this->maxRestart > this->params.maxIter
                     ? this->params.maxIter : this->maxRestart;
    this->minRestart = this->minRestart > this->maxRestart
                     ? this->maxRestart : this->minRestart;

    // Allocate memory for temporary vectors
    try {
        wk.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        safe.SetValues(len, 0.0);

        var.resize(len);

        hh.resize(this->maxRestart + 1);
        for ( int j = 0; j < this->maxRestart + 1; ++j )
            hh[j].resize(this->maxRestart);

        hcos.resize(this->maxRestart + 1);
        hsin.resize(this->maxRestart);
    } catch ( std::bad_alloc &ex ) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    while ( this->maxRestart >= this->minRestart ) {
        try {
            for ( int j = 0; j < this->maxRestart + 1; ++j ) {
                V.push_back(safe);
                Z.push_back(safe);
            }
            break;
        } catch ( std::bad_alloc &ex ) {
            this->maxRestart -= 5;
            while (!V.empty()) V.resize(0);
        }
    }

    if ( this->maxRestart < this->minRestart ) return FaspRetCode::ERROR_ALLOC_MEM;

    if ( this->restart > this->maxRestart ) this->restart = this->maxRestart;

    if ( this->restart < this->minRestart ) this->restart = this->minRestart;

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

// Clean up GMRES data allocated during setup.
void FGMRES::Clean() {

    this->restart = this->params.restart;

    if ( this->restart > this->maxRestart ) this->restart = this->maxRestart;

    if ( this->restart < this->minRestart ) this->restart = this->minRestart;

    wk.SetValues(len, 0.0);
    tmp.SetValues(len, 0.0);
    safe.SetValues(len, 0.0);

    var.assign(len, 0.0);

    hh.resize(this->maxRestart + 1);
    for ( int j = 0; j < this->maxRestart + 1; ++j )
        hh[j].assign(this->maxRestart, 0.0);

    hcos.assign( this->maxRestart + 1, 0.0 );
    hsin.assign( this->maxRestart, 0.0 );

    V.assign( this->maxRestart + 1, safe );
    Z.assign( this->maxRestart + 1, safe );

}

/// Right-preconditioned FGMRES solver
FaspRetCode FGMRES::Solve(const VEC &b, VEC &x) {

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // local variables
    double gamma, t;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld;
    double bNorm;
    int count;
    double ri, rj;

    const double max_cr = 0.9902680687415704;
    const double min_cr = 0.17364817766693041;
    const int decrease = 3;
    double cr = 1.0;

    PrintHead();

    // Initialize iterative method
    numIter = 0;
    safe = x;

    A->Apply(x, V[0]); // A * x -> V[0]

    V[0].XPAY(-1.0, b); // b - V[0] -> V[0]

    resAbs = ri = V[0].Norm2();
    bNorm = b.Norm2();
    denAbs = (bNorm > CLOSE_ZERO) ? bNorm : resAbs;
    resRel = resAbs / denAbs;
    resAbsOld = resAbs;

    if (resRel < params.relTol || resAbs < params.absTol)
        return FaspRetCode::SUCCESS;

    ratio = 0.0;

    // GMRES(m) outer iteration
    while (this->numIter < this->params.maxIter) {

        // Enter the cycle at the first iteration for at least one iteration
        var[0] = resAbs;
        V[0].Scale(1 / resAbs);

        // RESTART CYCLE (right-preconditioning)
        count = 0;
        while (count < this->restart
               && this->numIter < this->params.maxIter) {

            ++count;
            ++this->numIter;

            if (this->numIter > this->params.minIter)
                PrintInfo(numIter - 1, resRel, resAbs, ratio);

            // preconditioner
            tmp.SetValues(len, 0.0);
            pc->Solve(V[count - 1], Z[count - 1]);

            A->Apply(Z[count - 1], V[count]);

            // Modified Gram-Schmidt orthogonalization
            for (int j = 0; j < count; ++j) {
                hh[j][count - 1] = V[count].Dot(V[j]);
                V[count].AXPBY(1.0, -hh[j][count - 1], V[j]);
            }
            t = V[count].Norm2();
            hh[count][count - 1] = t;

            // If t=0, we get solution subspace
            if (fabs(t) > SMALL) V[count].Scale(1.0 / t);

            for (int j = 1; j < count; ++j) {
                t = hh[j - 1][count - 1];
                hh[j - 1][count - 1] =
                        hsin[j - 1] * hh[j][count - 1] + hcos[j - 1] * t;
                hh[j][count - 1] = -hsin[j - 1] * t + hcos[j - 1] * hh[j][count - 1];
            }

            t = hh[count][count - 1] * hh[count][count - 1];
            t += hh[count - 1][count - 1] * hh[count - 1][count - 1];
            t = sqrt(t);
            gamma = t > SMALL ? t : SMALL;
            hcos[count - 1] = hh[count - 1][count - 1] / gamma;
            hsin[count - 1] = hh[count][count - 1] / gamma;
            var[count] = -hsin[count - 1] * var[count - 1];
            var[count - 1] = hcos[count - 1] * var[count - 1];
            hh[count - 1][count - 1] = hsin[count - 1] * hh[count][count - 1]
                                       + hcos[count - 1] * hh[count - 1][count - 1];

            resAbs = fabs(var[count]);
            ratio = resAbs / resAbsOld;
            resAbsOld = resAbs;
            resRel = resAbs / denAbs;

            // Exit restart cycle if reaches tolerance
            if ((resAbs < this->params.absTol || resRel < this->params.relTol)
                && this->numIter > this->params.minIter)
                break;

        } // end of restart cycle

        // Compute solution, first solve upper triangular system
        var[count - 1] = var[count - 1] / hh[count - 1][count - 1];
        for (int k = count - 2; k >= 0; --k) {
            t = 0.0;
            for (int j = k + 1; j < count; ++j) t -= hh[k][j] * var[j];
            t += var[k];
            var[k] = t / hh[k][k];
        }

        wk = Z[count - 1];
        wk.Scale(var[count - 1]);

        for (int j = count - 2; j >= 0; --j) wk.AXPBY(1.0, var[j], Z[j]);

        x.AXPY(1.0, wk);

        // Save the best solution so far
        if (numIter >= params.safeIter && resAbs < resAbsOld) safe = x;

        // Check: prevent false convergence
        if (resRel < this->params.relTol && resAbs < this->params.absTol
            && this->numIter >= this->params.minIter) {
            double computed_resRel = resRel;

            // compute residual
            A->Apply(x, wk); // A * x -> p0
            wk.AXPBY(-1.0, 1.0, b); // b - p0 -> p0

            resAbs = wk.Norm2();
            resRel = resAbs / denAbs;

            if (resRel < this->params.relTol || resAbs < this->params.absTol)
                break;
            else { // Need to restart
                V[0] = tmp;
                count = 0;
            }

            if (this->params.verbose >= PRINT_MORE) {
                FASPXX_WARNING("False convergence!");
                WarnCompRes(computed_resRel);
                WarnRealRes(resRel);
            }

        } // end of convergence check

        // Compute residual vector and continue loop
        A->Apply(x, V[0]); // A * x -> V[0]

        V[0].XPAY(-1.0, b); // b - V[0] -> V[0]

        resAbs = rj = V[0].Norm2();

        // Choose restart number adaptively
        cr = rj / ri;
        ri = rj;

        if (cr > max_cr)
            this->restart = this->maxRestart;
        else if (cr <= max_cr && cr >= min_cr) {
            if (this->restart - decrease > this->minRestart)
                this->restart -= decrease;
            else
                this->restart = this->maxRestart;
        }

    } // end of main while loop

    if (this->numIter > this->params.maxIter)
        errorCode = FaspRetCode::ERROR_SOLVER_MAXIT;

    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (not(numIter == params.minIter && numIter == params.maxIter)) {
        this->norm2 = V[0].Norm2();
        this->normInf = V[0].NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    // Restore the saved best iteration if needed
    if (numIter > params.safeIter) x = safe;

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/