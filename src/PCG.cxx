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
#include "PCG.hxx"

// Assign param to this->param
FaspRetCode PCG::Setup(const LOP& A, const VEC& b, VEC& x, const IterParam& param) {
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
    if (lop == nullptr)
        lop = new IdentityLOP(len); //fff这样不太好,容易忘记delete这个lop. 可以强制传入 pc

    return FaspRetCode::SUCCESS;
}

// Assign lop to *this
void PCG::SetupPCD(const LOP* lop) {
    this->lop = lop;
}

/// Solve by PCG
FaspRetCode PCG::Solve(const LOP& A, const VEC& b, VEC& x, IterParam& param) {

    const unsigned MaxStag = 20;
    const INT len = b.GetSize();
    const DBL maxdiff = 1e-4 * param.relTol; // Stagnation tolerance
    const DBL solinftol = 1e-20; // Infinity norm tolerance

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    DBL resAbs = 1e+20, tmpAbs = 1e+20;
    DBL resRel = 1e+20, denAbs = 1e+20;
    DBL factor, alpha, beta, tmpa, tmpb;

    // Output iterative method info
    if (param.verbose > PRINT_NONE) std::cout << "\nCalling PCG solver ...\n";

    // Compute r_k = b - A * x
    //A->YMAX(b, x, this->rk);
    A.Apply(x,this->rk);
    this->rk.XPAY(-1.0,b);

    // Apply preconditioner z_k = B(r_k)
    this->lop->Apply(this->rk, this->zk);

    // Compute initial residual
    tmpAbs = this->rk.Norm2();
    denAbs = (1e-20 > tmpAbs) ? 1e-20 : tmpAbs;
    resRel = tmpAbs / denAbs;

    // If initial residual is already small, no need to iterate
    if (resRel < param.relTol || tmpAbs < param.absTol) goto FINISHED;

    // Prepare for the main loop
    PrintInfo(param.verbose, param.numIter, resRel, tmpAbs, 0.0);
    this->pk = this->zk;
    tmpa = this->zk.Dot(this->rk);

    // Main PCG loop
    while (param.numIter < param.maxIter) {

        ++param.numIter; // iteration count

        // ax = A * p_k
        A.Apply(this->pk, this->ax);

        // alpha_k = (z_{k-1},r_{k-1})/(A*p_{k-1},p_{k-1})
        tmpb = this->ax.Dot(this->pk);
        if (fabs(tmpb) > 1e-40) alpha = tmpa / tmpb;
        else {
            FASPXX_WARNING("Divided by zero!"); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        // x_k = x_{k-1} + alpha_k*p_{k-1}
        x.AXPY(alpha, this->pk);

        // r_k = r_{k-1} - alpha_k*A*p_{k-1}
        this->rk.AXPY(-alpha, this->ax);

        // Compute norm of residual
        resAbs = this->rk.Norm2();
        resRel = resAbs / denAbs;
        factor = resAbs / tmpAbs;

        // Output iteration information if needed
        PrintInfo(param.verbose, param.numIter, resRel, tmpAbs, factor);

        if (factor > 0.9) { // Only check when converge slowly
            // Check I: if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            DBL norminf = x.NormInf();
            if (norminf < solinftol) {
                if (param.verbose > PRINT_MIN)
                    FASPXX_WARNING("Iteration stopped -- solution almost zero!");
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if relative difference stagnated, try to restart
            DBL reldiff = fabs(alpha) * this->pk.Norm2() / x.Norm2();
            if ((stagStep <= MaxStag) && (reldiff < maxdiff)) {
                if (param.verbose > PRINT_SOME) {
                    DiffRes(reldiff, resRel);
                    FASPXX_WARNING("Iteration restarted -- stagnation!");
                }

                //A->YMAX(b, x, this->rk);
                A.Apply(x,this->rk);
                this->rk.XPAY(-1.0,b);
                resAbs = this->rk.Norm2();
                resRel = resAbs / denAbs;
                if (param.verbose > PRINT_SOME) RealRes(resRel);

                if (resRel < param.relTol) break;
                else {
                    if (stagStep >= MaxStag) {
                        if (param.verbose > PRINT_MIN)
                            FASPXX_WARNING("Iteration stopped -- staggnation!");
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->pk.SetValues(len, 0.0);
                    ++stagStep;
                }
            } // End of stagnation check!
        } // End of check I and II

        // Check III: prevent false convergence
        if (resRel < param.relTol) {
            // Compute true residual r = b - Ax and update residual
            //A->YMAX(b, x, this->rk);
            A.Apply(x,this->rk);
            this->rk.XPAY(-1.0,b);

            // Compute residual norms
            DBL updated_resRel = resRel;
            resAbs = rk.Norm2();
            resRel = resAbs / denAbs;

            // Check convergence
            if (resRel < param.relTol) break;

            if (param.verbose >= PRINT_MORE) {
                Compres(updated_resRel);
                RealRes(resRel);
            }

            if (moreStep >= param.restart) {
                if (param.verbose > PRINT_MIN)
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
        this->lop->Apply(this->rk, this->zk);

        // Compute beta_k = (z_k, r_k)/(z_{k-1}, r_{k-1})
        tmpb = this->zk.Dot(this->rk);
        beta = tmpb / tmpa;
        tmpa = tmpb;

        // Compute p_k = z_k + beta_k*p_{k-1}
        this->pk.XPAY(beta, this->zk);

    } // End of main PCG loop

    FINISHED: // Finish iterative method
    PrintFinal(param.verbose, param.numIter, param.maxIter, resRel);

    // Compute final residual norms
    param.normInf = rk.NormInf();
    param.norm2 = rk.Norm2();

    return errorCode;
}

/// Clean up preconditioner. fff应该不需要这个:pc由外面换进来,应该由外面清理它的内存
void PCG::CleanPCD() {
    LOP* lop;
    this->lop = lop;
}

/// Release temporary memory
void PCG::Clean() {
    VEC zero;
    this->pk = zero;
    this->rk = zero;
    this->zk = zero;
    this->ax = zero;
}

/// Print out iteration information for iterative solvers
void PCG::PrintInfo(const PRTLVL& verbose, const INT& iter, const DBL& resRel,
                    const DBL& resAbs, const DBL& factor) {
    if (verbose > PRINT_SOME || (verbose > PRINT_NONE && iter % 20 == 0)) {
        if (iter == 0) {
            std::cout << "--------------------------------------------------\n";
            std::cout << "It Num | ||r||/||b|| |    ||r||    | Conv. Factor \n";
            std::cout << "--------------------------------------------------\n";
        } // end if iter
        std::cout.precision(4);
        std::setiosflags(std::ios::scientific);
        std::cout << std::setw(6) << iter << " | "
                  << std::scientific << std::setw(11) << resRel << " | "
                  << std::setw(11) << resAbs << " | "
                  << std::fixed << std::setprecision(5) << factor
                  << std::endl;
    }
}

/// Print out final status of an iterative method
void PCG::PrintFinal(const PRTLVL& verbose, const INT& iter, const INT& maxit,
                     const DBL& resRel) {
    if (verbose > PRINT_NONE) {
        if (iter > maxit)
            std::cout << "### WARNING: MaxIt = " << maxit
                      << " reached with relative residual " << resRel << std::endl;
        else if (iter >= 0) {
            std::cout << "Number of iterations = " << iter
                      << " with relative residual "
                      << resRel << std::endl;
        }
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
