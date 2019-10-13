/**
 * a source file for PCG
 */

#include <iostream>
#include <iomanip>
#include "PCG.hxx"

// Assign param to this->param
FaspRetCode PCG::Setup(const MAT& A, const VEC& b, VEC& x, const IterParam& param) {
    if (x.GetSize() != A.GetColSize() || b.GetSize() != A.GetRowSize() ||
        A.GetRowSize() != A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len=b.GetSize();
    try{
        zk.SetValues(len,0.0);
        pk.SetValues(len,0.0);
        rk.SetValues(len,0.0);
        tmp.SetValues(len,0.0);
    }catch(std::bad_alloc& ex){
        return FaspRetCode ::ERROR_ALLOC_MEM;
    }

    /// identical preconditioner operator by default
    LOP lop(len,len);
    this->lop=lop;

    return FaspRetCode::SUCCESS;
}

// Assign lop to *this
void PCG::SetupPCD(const LOP& lop) {
    this->lop = lop;
}

/// Solve by PCG
FaspRetCode PCG::Solve(const MAT& A, const VEC& b, VEC& x, IterParam& param) {
    const INT MaxStag = 20;
    const INT maxdiff = 1e-4 * param.relTol; // Stagnation tolerance
    const DBL solinftol = 1e-20; // Infinity norm tolerance
    const INT len=b.GetSize();

    // Local variables
    INT stag = 1, morestep = 1;
    DBL absres = 1e+20, abstmp = 1e+20;
    DBL relres = 1e+20, norm = 1e+20, normtmp = 1e+20;
    DBL reldiff, norminf, factor;
    DBL alpha, beta, tmpa, tmpb;
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Output some info for debuging
    if (param.outLvl > PRINT_NONE)
        std::cout << "\nCalling PCG solver (CSR) ...\n";

    // rk = b - A * x
    //A.MultVec(x,this->tmp);
    //this->rk=b;
    //this->rk-=this->tmp;
    //this->rk.Add(1.0, b, -1.0, this->tmp);
    A.MinusMult(b,x,this->rk);

    this->lop.Apply(this->rk, this->zk);

    // Compute initial residual
    abstmp = this->rk.Norm2();
    normtmp = (1e-20 > abstmp) ? 1e-20 : abstmp;
    relres = abstmp / normtmp;

    // If initial residual is small, no need to iterate
    if (relres < param.relTol || abstmp < param.absTol) goto FINISHED;

    // Output iteration information if needed
    PrintInfo(param.outLvl, param.numIter, relres, abstmp, 0.0);

    this->pk = this->zk;
    tmpa = this->zk.Dot(this->rk);

    // Main PCG loop
    while (param.numIter++ < param.maxIter) {
        // tmp = A * pk
        A.MultVec(this->pk,this->tmp);

        // alpha_k = (z_{k-1},r_{k-1})/(A*p_{k-1},p_{k-1})
        tmpb = this->tmp.Dot(this->pk);
        if (fabs(tmpb) > 1e-40)
            alpha = tmpa / tmpb;
        else {
            DivZero(); // Possible breakdown
            errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
            goto FINISHED;
        }

        // u_k = u_{k-1} + alpha_k*p_{k-1}
        x.Add(1.0, alpha, this->pk);
        // r_k = r_{k-1} - alpha_k*A*p_{k-1}
        this->rk.Add(1.0, -alpha, this->tmp);

        // Compute norm of residual
        absres = this->rk.Norm2();
        relres = absres / normtmp;
        // compute reduction factor of residual ||r||
        factor = absres / abstmp;

        // Output iteration information if needed
        PrintInfo(param.outLvl,param.numIter, relres, abstmp, factor);

        if (factor > 0.9) { // Only check when converge slowly
            // Check I : if solution is close to zero, return ERROR_SOLVER_SOLSTAG
            norminf = x.NormInf();
            if (norminf <= solinftol) {
                if (param.outLvl > PRINT_MIN) ZeroSol();
                errorCode = FaspRetCode::ERROR_SOLVER_SOLSTAG;
                break;
            }

            // Check II: if stagnated, try to restart
            norm = x.Norm2();
            // Compute relative difference
            reldiff = fabs(alpha) * this->pk.Norm2() / norm;

            if ((stag <= MaxStag)&  (reldiff < maxdiff)) {
                if (param.outLvl >= PRINT_MORE) {
                    DiffRes(reldiff, relres);
                    Restart();
                }

                // A.MultVec(x,this->tmp);
                // this->rk.Add(1.0, b, -1.0, this->tmp);
                A.MinusMult(b,x,this->rk);

                absres = this->rk.Norm2();
                relres = absres / normtmp;

                if (param.outLvl >= PRINT_MORE) RealRes(relres);

                if (relres < param.relTol)
                    break;
                else {
                    if (stag >= MaxStag) {
                        if (param.outLvl > PRINT_MIN) Stagged();
                        errorCode = FaspRetCode::ERROR_SOLVER_STAG;
                        break;
                    }
                    this->pk.SetValues(len, 0.0);
                    ++stag;
                }
            } // End of stagnation check!
        }// End of check I and II

        // Check III: prevent false convergence
        if (relres < param.relTol) {
            DBL updated_relres = relres;

            // Compute true residual r = b - Ax and update residual
            // A.MultVec(x,this->tmp);
            // this->rk.Add(1.0, b, -1.0, this->tmp);
            A.MinusMult(b,x,this->rk);

            // Compute residual norms
            absres = rk.Norm2();
            relres = absres / normtmp;

            // Check convergence
            if (relres < param.relTol) break;

            if (param.outLvl >= PRINT_MORE) {
                Compres(updated_relres);
                RealRes(relres);
            }

            if (morestep >= param.restart) {
                if (param.outLvl > PRINT_MIN) ZeroTol();
                errorCode = FaspRetCode::ERROR_SOLVER_TOLSMALL;
                break;
            }

            // Prepare for restarting method
            this->pk.SetValues(len, 0.0);
            ++morestep;
        } // End of safe-guard check!

        // Save residual for next iteration
        abstmp = absres;

        // Compute z_k = B(r_k)
        this->lop.Apply(this->rk, this->zk);

        // Compute beta_k = (z_k, r_k)/(z_{k-1}, r_{k-1})
        tmpb = this->zk.Dot(this->rk);
        beta = tmpb / tmpa;
        tmpa = tmpb;

        // Compute p_k = z_k + beta_k*p_{k-1}
        this->pk.Add(beta, 1.0, this->zk);
    }// End of main PCG loop

    FINISHED:// Finish iterative method
    if (param.outLvl > PRINT_NONE)
        Final(param.numIter, param.maxIter, relres);

    param.normInf = rk.NormInf();
    param.norm2 = rk.Norm2();
    param.numIter--;

    return errorCode;
}

// Clean preconditioner
void PCG::CleanPCD() {
    LOP lop;
    this->lop = lop;
}

/// Release temporary memory
void PCG::Clean() {
    VEC zero;
    this->pk=zero;
    this->rk=zero;
    this->zk=zero;
    this->tmp=zero;
}

/// Print out iteration information for iterative solvers
void PCG::PrintInfo(const PRTLVL& prtlvl, const INT& iter,const DBL& relres,
                    const DBL& absres, const DBL& factor) {
    if ( prtlvl >= PRINT_SOME ) {
        if (iter == 0) {
            std::cout<< "--------------------------------------------------\n";
            std::cout<< "It Num | ||r||/||b|| |    ||r||    | Conv. Factor \n";
            std::cout<< "--------------------------------------------------\n";
        } // end if iter
        std::cout.precision(4);
        std::setiosflags(std::ios::scientific);
        std::cout << std::setw(6) << iter << " | "
                  << std::scientific << std::setw(11) << relres << " | "
                  << std::setw(11) <<  absres << " | "
                  << std::fixed << std::setprecision(5) << factor
                  << std::endl;
    }
}

/// Print out final status of an iterative method
void PCG::Final(const INT& iter, const INT& maxit, const DBL& relres) {
    if (iter > maxit)
        std::cout << "### WARNING: MaxIt = " << maxit
                  << " reached with relative residual " << relres << std::endl;
    else if (iter >= 0) {
        std::cout << "Number of iterations = " << iter << " with relative residual "
                  << relres << std::endl;
    }
}
