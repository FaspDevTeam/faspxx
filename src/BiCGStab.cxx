/*! \file    BiCGStab.hxx
 *  \brief   Preconditioned BiCGStab class declaration
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <ios>
#include "Iter.hxx"
#include "BiCGStab.hxx"

/// check and allocate memory
FaspRetCode BiCGStab::Setup(const LOP &A)
{
    const INT len = A.GetColSize();
    try {
        r0star.SetValues(len,0.0);
        tmp.SetValues(len,0.0);
        apj.SetValues(len,0.0);
        asj.SetValues(len,0.0);
        pj.SetValues(len,0.0);
        rj.SetValues(len,0.0);
        sj.SetValues(len,0.0);
        ptmp.SetValues(len,0.0);
        stmp.SetValues(len,0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    this->A = &A;

    /// identical preconditioner operator by default
    if (pc == nullptr) {
        pc = new Identity();
        mark = false;
    }

    return FaspRetCode::SUCCESS;
}

/// solve by BiCGStab
FaspRetCode BiCGStab::Solve(const VEC &b, VEC &x)
{
    if ( x.GetSize() != A->GetColSize() || b.GetSize() != A->GetRowSize() ||
         A->GetRowSize() != A->GetColSize() )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    const unsigned MaxStag = 20;
    const INT len = b.GetSize();
    const double maxdiff = 1e-4 * relTol; // Stagnation tolerance
    const double solinftol = 1e-20;

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    double resAbs, tmpAbs, resRel, denAbs;
    double alphaj, betaj, rjr0star, rjr0startmp, omegaj;
    double tmp12,factor;

    // Initial iteration
    numIter = 0;

    // r0 = b - A * x_{0}
    A->Apply(x, this->tmp);
    this->rj.WAXPBY(1.0, b, -1.0, this->tmp);

    // Compute initial residual
    tmpAbs = this->rj.Norm2();
    denAbs = (1e-20>tmpAbs)?1e-20:tmpAbs;
    resRel = tmpAbs/denAbs;

    // If initial residual is already small, no need to iterate
    if ( resRel < relTol || tmpAbs < absTol ) goto FINISHED;

    // Prepare for the main loop
    PrintInfo(numIter, resRel, tmpAbs, 0.0);

    // r0_{*} = r0
    this->r0star = this->rj;

    // p0 = r0
    this->pj = this->rj;

    while ( numIter<maxIter ) {

        ++numIter;

        // alpha_{j} = (rj,r0star)/(P * A * pj,r0star)
        rjr0star = this->rj.Dot(this->r0star);

        A->Apply(this->pj, this->apj);
        pc->Solve(this->apj,this->ptmp);

        tmp12 = this->ptmp.Dot(this->r0star);
        if ( fabs(tmp12) > 1e-40 ) alphaj = rjr0star / tmp12;
        else {
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        // sj = rj - alpha_{j} * P * A * p_{j}
        this->sj.WAXPBY(1.0, this->rj, -alphaj, this->ptmp);

        // omegaj = (P * A * sj,sj)/(P * A * sj,P * A * sj)
        A->Apply(this->sj, this->asj);
        pc->Solve(this->asj,this->stmp);
        omegaj = this->stmp.Dot(this->sj) / this->stmp.Dot(this->stmp);

        // x_{j+1} = x_{j} + alpha_{j} * P * pj + omegaj * P * s_{j}
        pc->Solve(this->pj,this->mp);
        pc->Solve(this->sj,this->ms);
        this->tmp.WAXPBY(alphaj, this->mp, omegaj, this->ms);
        x.XPAY(1.0, this->tmp);

        // r_{j+1} = sj - omegaj * P * A * sj
        this->rj.WAXPBY(1.0, this->sj, -omegaj, this->stmp);

        // Compute norm of residual
        resAbs = this->rj.Norm2();
        resRel = resAbs / denAbs;
        factor = resAbs / tmpAbs;

        // Output iteration information if needed
        PrintInfo(numIter, resRel, tmpAbs, factor);

        if (factor > 0.9) { // Only check when converge slowly
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            double norminf = x.NormInf();
            if (norminf < solinftol) {
                if (verbose > PRINT_MIN) FASPXX_WARNING(
                        "Iteration stopped -- solution almost zero!");
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference stagnated, try to restart
            double reldiff = fabs(alphaj) * this->pj.Norm2() / x.Norm2();
            if ((stagStep <= MaxStag) && (reldiff < maxdiff)) {
                if (verbose > PRINT_SOME) {
                    WarnDiffRes(reldiff, resRel);
                    FASPXX_WARNING("Iteration restarted -- stagnation!");
                }

                A->Apply(x, this->rj);
                this->rj.XPAY(-1.0, b);
                resAbs = this->rj.Norm2();
                resRel = resAbs / denAbs;

                if (verbose > PRINT_SOME) WarnRealRes(resRel);

                if (resRel < relTol) break;
                else {
                    if (stagStep >= MaxStag) {
                        if (verbose > PRINT_MIN)
                            FASPXX_WARNING("Iteration stopped -- stagnation!");
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->pj.SetValues(len, 0.0);
                    ++stagStep;
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence
        if (resRel < relTol) {
            // Compute true residual r = b - Ax and update residual
            A->Apply(x, this->rj);
            this->rj.XPAY(-1.0, b);

            // Compute residual norms
            double updated_resRel = resRel;
            resAbs = rj.Norm2();
            resRel = resAbs / denAbs;

            // Check convergence
            if (resRel < relTol) break;

            if (verbose >= PRINT_MORE) {
                WarnCompRes(updated_resRel);
                WarnRealRes(resRel);
            }

            if (moreStep >= restart) {
                if (verbose > PRINT_MIN)
                    FASPXX_WARNING("The tolerance might be too small!");
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            this->pj.SetValues(len, 0.0);
            ++moreStep;
        } // End of safe-gurad check!

        // Save residual for next iteration
        tmpAbs = resAbs;

        // Compute betaj = (r_{j+1},r0^{*})/(r_{j},r0^{*}) *
        // \frac{alpha_{j}}{omega_{j}}
        rjr0startmp = rjr0star;
        rjr0star = this->rj.Dot(this->r0star);
        betaj = rjr0star / rjr0startmp * alphaj / omegaj;

        // p_{j+1} = r_{j+1} + betaj * (p_{j} - omegaj * P * A * p_{j})
        this->tmp.WAXPBY(1.0, this->pj, -omegaj, this->ptmp);
        this->pj.WAXPBY(1.0, this->rj, betaj, this->tmp);

    } // End of main PCG loop

FINISHED: // Finish iterative method
    this->normInf = rj.NormInf();
    this->norm2   = rj.Norm2();
    PrintFinal();

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/