
#include <stack>
#include "GMRES.hxx"
#include <iostream>

/// set the maximum and minimum restart
void GMRES::SetMaxMinRestart(int maxRestart, int minRestart) {
    this->maxRestart = maxRestart;
    this->minRestart = minRestart;
}

// Set up the GMRES method
FaspRetCode GMRES::Setup(const LOP &A) {

    // Set solver type
    SetSolType(SOLType::GMRES);

    this->maxRestart = this->maxRestart > this->params.maxIter ? this->params
            .maxIter : this->maxRestart;

    this->minRestart = this->minRestart > this->maxRestart ? this->maxRestart : this
            ->minRestart;

    // Allocate memory for temporary vectors
    try {
        const INT len = A.GetColSize();
        wk.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        safe.SetValues(len, 0.0);

        var.resize(len);

        hh.resize(this->maxRestart + 1);
        for ( int j = 0; j < this->maxRestart + 1; ++j )
            hh[j].resize(this->maxRestart);

        hcos.resize(this->maxRestart + 1);
        hsin.resize(this->maxRestart);

        norms.resize(this->params.maxIter + 1);
    } catch ( std::bad_alloc &ex ) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    while ( this->maxRestart >= this->minRestart ) {
        try {
            for ( int j = 0; j < this->maxRestart + 1; ++j )
                V.push_back(safe);
            break;
        } catch ( std::bad_alloc &ex ) {
            this->maxRestart -= 5;
            while ( !V.empty() )
                V.resize(0);
        }
    }

    if ( this->maxRestart < this->minRestart ) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    if ( this->params.restart > this->maxRestart )
        this->params.restart = this->maxRestart;
    if ( this->params.restart < this->minRestart )
        this->params.restart = this->minRestart;

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

// Solve Ax=b using the GMRES method.
FaspRetCode GMRES::Solve(const VEC &b, VEC &x) {

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // local variables
    double gamma, t;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld;
    int count;
    double ri, rj;
    int len = x.GetSize();

    double max_cr = 0.9902680687415704;
    double min_cr = 0.17364817766693041;
    double cr = 1.0;
    int d = 3;

    // Initialize iterative method
    numIter = 0;
    safe = x;

    A->Apply(x, V[0]); // A * x -> V[0]

    V[0].XPAY(-1.0, b); // b - V[0] -> V[0]

    ri = V[0].Norm2();

    // preconditioner
    tmp.SetValues(len, 0.0);
    pc->Solve(V[0], tmp);

    resAbs = tmp.Norm2();
    denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
    resRel = resAbs / denAbs;
    resAbsOld = resAbs;

    if ( resRel < params.relTol || resAbs < params.absTol )
        return FaspRetCode::SUCCESS;

    // store initial residual
    norms[0] = resRel;

    ratio = 1.0;
    /* GMRES(M) outer iteration */
    while ( this->numIter < this->params.maxIter && resRel > this->params
            .relTol ) {

        var[0] = resAbs;
        V[0].Scale(1 / resAbs);

        /* RESTART CYCLE (right-preconditioning) */
        count = 0;
        while ( count < this->params.restart &&
               this->numIter < this->params.maxIter ) {

            ++count;
            ++this->numIter;

            if( this->numIter>this->params.minIter ) PrintInfo(numIter-1, resRel,
                    resAbs,ratio);

            // preconditioner
            tmp.SetValues(len, 0.0);
            pc->Solve(V[count - 1], tmp);

            A->Apply(tmp, V[count]);

            /* Modified Gram_Schmidt orthogonalization */
            for ( int j = 0; j < count; ++j ) {
                hh[j][count - 1] = V[count].Dot(V[j]);
                V[count].AXPBY(1.0, -hh[j][count - 1], V[j]);
            }
            t = V[count].Norm2();
            hh[count][count - 1] = t;

            if ( fabs(t) > SMALL ) // if t=0, we get solution subspace
                V[count].Scale(1.0 / t);

            for ( int j = 1; j < count; ++j ) {
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
            hh[count - 1][count - 1] =
                    hsin[count - 1] * hh[count][count - 1] + hcos[count - 1] * hh
                    [count - 1][count - 1];

            resAbs = fabs(var[count]);
            ratio = resAbs / resAbsOld;
            resAbsOld = resAbs;
            resRel = resAbs / denAbs;

            norms[this->numIter] = resRel;

            /* exit restart cycle if reaches tolerance */
            if (( resAbs < this->params.absTol || resRel < this->params.relTol ) &&
                this->numIter > this->params.minIter )
                break;

        } /* end of restart cycle */

        /* compute solution, first solve upper triangular system */
        var[count - 1] = var[count - 1] / hh[count - 1][count - 1];
        for ( int k = count - 2; k >= 0; --k ) {
            t = 0.0;
            for ( int j = k + 1; j < count; ++j )
                t -= hh[k][j] * var[j];
            t += var[k];
            var[k] = t / hh[k][k];
        }

        wk = V[count - 1];
        wk.Scale(var[count - 1]);

        for ( int j = count - 2; j >= 0; --j )
            wk.AXPBY(1.0, var[j], V[j]);

        /* apply preconditioner */
        tmp.SetValues(len, 0.0);
        pc->Solve(wk, tmp);

        x.AXPY(1.0, tmp);

        A->Apply(x, tmp);
        tmp.XPAY(-1.0, b);

        rj = tmp.Norm2();
        cr = rj / ri;
        ri = rj;

        if ( cr > max_cr )
            this->params.restart = this->maxRestart;
        else if ( cr <= max_cr && cr >= min_cr ) {
            if ( this->params.restart - d > this->minRestart )
                this->params.restart -= d;
            else
                this->params.restart = this->maxRestart;
        }

        // Save the best solution so far
        if ( numIter >= params.safeIter && resAbs < resAbsOld ) safe = x;

        // Check: prevent false convergence
        if ( resRel < this->params.absTol && this->numIter >= this->params.minIter
        ) {
            double computed_resRel = resRel;

            // compute residual
            A->Apply(x, wk); // A * x -> p0
            wk.AXPBY(-1.0, 1.0, b); // b - p0 -> p0

            /* apply preconditioner */
            tmp.SetValues(len, 0.0);
            pc->Solve(wk, tmp);

            resAbs = tmp.Norm2();
            resRel = resAbs / denAbs;

            norms[this->numIter] = resRel;

            if ( resRel < this->params.relTol )
                break;
            else { // Need to restart
                V[0] = tmp;
                count = 0;
            }

            if ( this->params.verbose >= PRINT_MORE ) {
                FASPXX_WARNING("False convergence!");
                WarnCompRes(computed_resRel);
                WarnRealRes(resRel);
            }

        } /* end of convergence check */

        /* compute residual vector and continue loop */
        for ( int j = count; j > 0; --j ) {
            var[j - 1] = -hsin[j - 1] * var[j];
            var[j] = hcos[j - 1] * var[j];
        }

        if ( count )
            V[count].Scale(var[count]);

        for ( int j = count - 1; j > 0; --j )
            V[count].AXPY(var[j], V[j]);

        if ( count ) {
            V[0].Scale(var[0]);
            V[0].AXPY(1.0, V[count]);
        }

    } /* end of main while loop */

    if ( this->numIter > this->params.maxIter )
        errorCode = FaspRetCode::ERROR_SOLVER_MAXIT;


    A->Apply(x, wk); // A * x -> wk
    wk.XPAY(-1.0, b); // b - p0 -> wk

    this->norm2 = wk.Norm2();
    this->normInf = wk.NormInf();
    PrintFinal(numIter, resRel, resAbs, ratio);


    return errorCode;
}