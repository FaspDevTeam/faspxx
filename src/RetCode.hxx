/** \file    RetCode.hxx
 *  \brief   Exception types and return code definitions
 *  \author  Chensong Zhang
 *  \date    Sep/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __RETCODE_HEADER__ /*-- allow multiple inclusions --*/
#define __RETCODE_HEADER__ /**< indicate RetCode.hxx has been included before */

// Standard header files
#include <iostream>
#include <stdexcept>
#include <string>

// FASPXX header files
#include "Faspxx.hxx"

/// \brief Return code definition
enum FaspRetCode {
    SUCCESS               = 0,   ///< Everything is fine
    ERROR_OPEN_FILE       = -10, ///< Failed to open a file
    ERROR_INPUT_FILE      = -11, ///< Wrong input file
    ERROR_INPUT_PAR       = -12, ///< Wrong input argument
    ERROR_VEC_SIZE        = -14, ///< Wrong vector size
    ERROR_MAT_SIZE        = -15, ///< Wrong matrix size
    ERROR_NONMATCH_SIZE   = -16, ///< Two sizes do not match
    ERROR_MAT_DATA        = -17, ///< Wrong matrix format
    ERROR_DIVIDE_ZERO     = -18, ///< Divided by zero!
    ERROR_MAT_ZERODIAG    = -19, ///< MAT has zero diagonal entries
    ERROR_ALLOC_MEM       = -20, ///< Failed to allocate memory
    ERROR_DUMMY_VAR       = -23, ///< Unknown function dummy variables
    ERROR_SOLVER_TYPE     = -30, ///< Unknown solver type
    ERROR_SOLVER_PCD_TYPE = -31, ///< Unknown preconditioner type
    ERROR_SOLVER_STAG     = -32, ///< Iterative solver stagnates
    ERROR_SOLVER_SOLSTAG  = -33, ///< Iterative solver's solution is too small
    ERROR_SOLVER_TOLSMALL = -34, ///< Iterative solver's tolerance is too small
    ERROR_SOLVER_MAXIT    = -39, ///< Maximal iteration number reached
    ERROR_AMG_INTERP_TYPE = -40, ///< Unknown AMG interpolation type
    ERROR_AMG_SMOOTH_TYPE = -41, ///< Unknown AMG smoother type
    ERROR_AMG_COARSE_TYPE = -42, ///< Unknown AMG coarsening type
    ERROR_AMG_COARSEING   = -43, ///< AMG coarsening step failed to complete
    ERROR_AMG_SETUP       = -49, ///< AMG setup failed to complete
    ERROR_ILU_TYPE        = -50, ///< Unknown ILU method type
    ERROR_ILU_SETUP       = -59, ///< ILU setup failed to complete
    ERROR_SWZ_TYPE        = -60, ///< Unknown Schwarz method type
    ERROR_SWZ_SETUP       = -69, ///< Schwarz method setup failed to complete
    ERROR_DSOLVER_SETUP   = -91, ///< Fail to setup in direct solver
    ERROR_DSOLVER_SOLVE   = -92, ///< Fail to solve in direct solver
    PRINT_HELP            = 0,   ///< Print help message
};

/// \brief Get error message from FaspRetCode
std::string GetRetCode(const FaspRetCode code);

/// \brief Run-time exception capturing class
class FaspRunTime : public std::runtime_error
{

private:
    const char* file; ///< Which file throws exception
    const char* func; ///< Which function throws exception
    const USI   line; ///< Which line throws exception

public:
    const FaspRetCode errorCode; ///< Error Code

public:
    /// Default constructor
    FaspRunTime(const FaspRetCode code_, const char* file_, const char* func_,
                const USI line_)
        : std::runtime_error(GetRetCode(code_))
        , file(file_)
        , func(func_)
        , line(line_)
        , errorCode(code_)
    {
    }

    // Get exception location
    const char* GetFile() const { return file; } ///< return thrown file name
    const char* GetFunc() const { return func; } ///< return thrown function name
    USI         GetLine() const { return line; } ///< return thrown line number

    // Standard output
    void LogExcep(std::ostream& stream = std::cout) const;
};

/// \brief Allocation exception capturing class
class FaspBadAlloc : public std::bad_alloc
{

private:
    const char* file; ///< Which file throws exception
    const char* func; ///< Which function throws exception
    USI         line; ///< Which line throws exception

public:
    const FaspRetCode errorCode = FaspRetCode::SUCCESS; ///< Error Code

public:
    /// Default constructor
    FaspBadAlloc(const char* file_, const char* func_, const USI line_)
        : std::bad_alloc()
        , file(file_)
        , func(func_)
        , line(line_)
    {
    }

    // Get exception location
    const char* GetFile() const { return file; } ///< return thrown file name
    const char* GetFunc() const { return func; } ///< return thrown function name
    USI         GetLine() const { return line; } ///< return thrown line number

    // Standard output
    void LogExcep(std::ostream& stream = std::cout) const;
};

#endif /* end if for __RETCODE_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Sep/25/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/