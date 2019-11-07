/**
 * print iterative information
 */

#ifndef PRINT_HXX
#define PRINT_HXX

#include <iostream>
#include "Param.hxx"
#include <iomanip>

//! Warning for actual relative residual
void RealRes(DBL relres);

//! Warning for computed relative residual
void Compres(DBL relres);

void DiffRes(DBL reldiff,DBL relres);

/// Print out iteration information for iterative solvers
void PrintInfo(const PRTLVL& verbose, const INT& iter, const DBL& resRel,
               const DBL& resAbs, const DBL& factor);

/// Print out final status of an iterative method
void PrintFinal(const PRTLVL& verbose, const INT& iter, const INT& maxit,
                const DBL& resRel);

#endif // PRINT_HXX
