/**
 * Print source codes
 */

#include "Print.hxx"

//! Warning for actual relative residual
void RealRes(DBL relres){
    std::cout<<"### WARNING: The actual relative residual = "
             <<relres<<std::endl;
}

//! Warning for computed relative residual
void Compres(DBL relres){
    std::cout<<"### WARNING: The computed relative residual = "
             <<relres<<std::endl;
}

//! Output relative difference and residual
void DiffRes(DBL reldiff,DBL relres){
    std::cout<<"||u-u'|| = "<<reldiff<<
             " and the comp. rel. res. = "<<relres<<std::endl;
}

/// Print out iteration information for iterative solvers
void PrintInfo(const PRTLVL& verbose, const INT& iter, const DBL& resRel,
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
                  << std::scientific << std::setprecision(5) << resRel << " | "
                  << std::setw(11) << resAbs << " | "
                  << std::fixed << std::setprecision(5) << factor
                  << std::endl;
    }
}

/// Print out final status of an iterative method
void PrintFinal(const PRTLVL& verbose, const INT& iter, const INT& maxit,
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