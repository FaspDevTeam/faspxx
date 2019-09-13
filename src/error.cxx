/** \file error.hxx
 *  \brief Output error code
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <string>
#include <iostream>
#include "error.hxx"

/*---------------------------------*/
/*--      Public Functions       --*/
/*---------------------------------*/

 /// \brief Get error message from FaspErrorCode
const std::string getErrorCode(const FaspErrorCode code) {
    switch(code) {
        case ERROR_OPEN_FILE:
            return "Failed to open a file!";
        case ERROR_INPUT_FILE:
            return "Wrong input file!";
        case ERROR_INPUT_PAR:
            return "Wrong input argument!";
	case ERROR_DIVIDE_ZERO:
	    return "Incorrect divisor!"
        case ERROR_VEC_SIZE:
            return "Wrong vector size!";
        case ERROR_MAT_SIZE:
            return "Wrong matrix size!";
        case ERROR_NONMATCH_SIZE:
            return "Two sizes do not match!";
        case ERROR_MAT_ZERODIAG:
            return "MAT has zero diagonal entries!";
        case ERROR_ALLOC_MEM:
            return "Failed to allocate memory!";
        case ERROR_DUMMY_VAR:
            return "Unknown function dummy variables!";
        case ERROR_SOLVER_TYPE:
            return "Unknown solver type!";
        case ERROR_SOLVER_PRECTYPE:
            return "Unknown preconditioner type!";
        case ERROR_SOLVER_STAG:
            return "Iterative solver stagnates!";
        case ERROR_SOLVER_SOLSTAG:
            return "Iterative solver's solution is too small!";
        case ERROR_SOLVER_TOLSMALL:
            return "Iterative solver's tolerance is too small!";
        case ERROR_SOLVER_MAXIT:
            return "Maximal iteration number reached!";
        case ERROR_AMG_INTERP_TYPE:
            return "Unknown AMG interpolation type!";
        case ERROR_AMG_SMOOTH_TYPE:
            return "Unknown AMG smoother type!";
        case ERROR_AMG_COARSE_TYPE:
            return "Unknown AMG coarsening type!";
        case ERROR_AMG_COARSEING:
            return "AMG coarsening step failed to complete!";
        case ERROR_AMG_SETUP:
            return "AMG setup failed to complete!";
        case ERROR_ILU_SETUP:
            return "ILU setup failed to complete!";
        default:
            return "Unknown error type!";
    }
}

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/
