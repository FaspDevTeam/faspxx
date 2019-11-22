/*! \file PCG.cxx
 *  \brief PCG class definition
 *  \author Chensong Zhang, Kailei Zhang
 *  \date Oct/13/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include <iomanip>
#include "PCD.hxx"
#include "Print.hxx"
#include "PCG.hxx"

// Assign param to this->param
FaspRetCode PCG::Setup(const LOP& A, const VEC& b, VEC& x)
{
    if (x.GetSize() != A.GetColSize() || b.GetSize() != A.GetRowSize() ||
        A.GetRowSize() != A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len = b.GetSize();
    try {
        zk.SetValues(len, 0.0);
        pk.SetValues(len, 0.0);
        rk.SetValues(len, 0.0);
        ax.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    /// identical preconditioner operator by default
    if (pc == nullptr)
        pc = new IdentityLOP(len);

    return FaspRetCode::SUCCESS;
}

// Assign lop to *this
void PCG::SetPC(LOP* lop)
{
    this->pc = lop;
}

/// Solve by PCG
FaspRetCode PCG::Solve(const LOP& A, const VEC& b, VEC& x)
{
    const unsigned MaxStag = 20;
    const INT len = b.GetSize();
    const DBL maxdiff = 1e-4 * relTol; // Stagnation tolerance
    const DBL solinftol = 1e-20; // Infinity norm tolerance

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    DBL resAbs = 1e+20, tmpAbs = 1e+20;
    DBL resRel = 1e+20, denAbs = 1e+20;
    DBL factor, alpha, beta, tmpa, tmpb;

    // Output iterative method info
    if (verbose > PRINT_NONE) std::cout << "\nCalling PCG solver ...\n"; // fff建议不要有这些输出,否则程序运行起来会议一堆这样的输出.如果非要输出可以定义宏,全局可以打开关闭

    // Initial iteration
    int iter = 0;

    A.Apply(x, rk); // A * x -> rk
    rk.XPAY(-1.0, b); // b - rk -> rk

    pc->Apply(rk, zk); // Apply preconditioner z_k = B(r_k)

    // Compute initial residual
    tmpAbs = rk.Norm2();
    denAbs = (1e-20 > tmpAbs) ? 1e-20 : tmpAbs;
    resRel = tmpAbs / denAbs;

    // If initial residual is already small, no need to iterate
    if (resRel < relTol || tmpAbs < absTol) goto FINISHED;

    // Prepare for the main loop
    PrintInfo(verbose,iter,resRel,tmpAbs,0.0);
    pk = zk;
    tmpa = zk.Dot(rk);

    // Main PCG loop
    while (iter < maxIter)
    {
        iter++; // iteration count

        A.Apply(pk, ax); // ax = A * p_k

        // alpha_k = (z_{k-1}, r_{k-1})/(A*p_{k-1},p_{k-1})
        tmpb = ax.Dot(pk);
        if (fabs(tmpb) > 1e-40) alpha = tmpa / tmpb;
        else
        {
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        x.AXPY(alpha, pk); // x_k = x_{k-1} + alpha_k*p_{k-1}
        rk.AXPY(-alpha, ax); // r_k = r_{k-1} - alpha_k*A*p_{k-1}

        // Compute norm of residual
        resAbs = rk.Norm2();
        resRel = resAbs / denAbs;
        factor = resAbs / tmpAbs;

        // Output iteration information if needed
        PrintInfo(verbose, iter, resRel, tmpAbs, factor);

        if (factor > 0.9) // Only check when converge slowly
        {
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            DBL norminf = x.NormInf();
            if (norminf < solinftol) {
                if (verbose > PRINT_MIN)
                    FASPXX_WARNING("Iteration stopped -- solution almost zero!");
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference stagnated, try to restart
            DBL reldiff = fabs(alpha) * this->pk.Norm2() / x.Norm2();
            if ((stagStep <= MaxStag) && (reldiff < maxdiff)) {
                if (verbose > PRINT_SOME) {
                    DiffRes(reldiff, resRel);
                    FASPXX_WARNING("Iteration restarted -- stagnation!");
                }

                //A->YMAX(b, x, this->rk);
                A.Apply(x,this->rk);
                this->rk.XPAY(-1.0,b);
                resAbs = this->rk.Norm2();
                resRel = resAbs / denAbs;
                if (verbose > PRINT_SOME) RealRes(resRel);

                if (resRel < relTol) break;
                else {
                    if (stagStep >= MaxStag) {
                        if (verbose > PRINT_MIN)
                            FASPXX_WARNING("Iteration stopped -- stagnation!");
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->pk.SetValues(len, 0.0);
                    ++stagStep;
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence
        if (resRel < relTol) {
            // Compute true residual r = b - Ax and update residual
            //A->YMAX(b, x, this->rk);
            A.Apply(x,this->rk);
            this->rk.XPAY(-1.0,b);

            // Compute residual norms
            DBL updated_resRel = resRel;
            resAbs = rk.Norm2();
            resRel = resAbs / denAbs;

            // Check convergence
            if (resRel < relTol) break;

            if (verbose >= PRINT_MORE) {
                Compres(updated_resRel);
                RealRes(resRel);
            }

            if (moreStep >= restart) {
                if (verbose > PRINT_MIN)
                    FASPXX_WARNING("The tolerence might be too small!");
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            this->pk.SetValues(len, 0.0);
            ++moreStep;
        } // End of safe-guard check!

        // Save residual for next iteration
        tmpAbs = resAbs;

        // Apply preconditioner z_k = B(r_k)
        pc->Apply(rk, zk);

        // Compute beta_k = (z_k, r_k)/(z_{k-1}, r_{k-1})
        tmpb = zk.Dot(rk);
        beta = tmpb / tmpa;
        tmpa = tmpb;

        // Compute p_k = z_k + beta_k*p_{k-1}
        pk.XPAY(beta, zk);

    } // End of main PCG loop

    FINISHED: // Finish iterative method
    PrintFinal(verbose, iter, maxIter, resRel);

    // Compute final residual norms
//    param.normInf = rk.NormInf();
//    param.norm2 = rk.Norm2();

    return errorCode;
}

/// Clean up preconditioner. fff应该不需要这个:pc由外面换进来,应该由外面清理它的内存
void PCG::CleanPCD()
{
    LOP* lop;
    pc = lop;
}

/// Release temporary memory
void PCG::Clean()
{
    VEC zero;
    pk = zero;
    rk = zero;
    zk = zero;
    ax = zero;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
