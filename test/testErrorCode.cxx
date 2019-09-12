/** \file testErrorCode.cxx
 *  \brief Test error code in FASP++
 *
 *  Compile:
 *      g++ testErrorCode.cxx -o testErrorCode
 *
 *  Expected results:
 *      ### ERROR: Unknown error type!
 *      ### ERROR: Check testErrorCode.cxx at Line 142
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <string>
#include <iostream>
#include <exception>

/*---------------------------------*/
/*--      Public Functions       --*/
/*---------------------------------*/

/// \brief Error code definition
enum FaspErrorCode {
    SUCCESS                = 0,    //! Everything is fine
    //---------- Input problems ---------------------------------------------------//
    ERROR_OPEN_FILE        = -10,  //! Failed to open a file
    ERROR_INPUT_FILE       = -11,  //! Wrong input file
    ERROR_INPUT_PAR        = -13,  //! Wrong input argument
    //---------- VEC or MAT data problems -----------------------------------------//
    ERROR_VEC_SIZE         = -14,  //! Wrong vector size
    ERROR_MAT_SIZE         = -15,  //! Wrong matrix size
    ERROR_NONMATCH_SIZE    = -16,  //! Two sizes do not match
    ERROR_MAT_ZERODIAG     = -19,  //! MAT has zero diagonal entries
    //---------- Memory or function call problems ---------------------------------//
    ERROR_ALLOC_MEM        = -20,  //! Failed to allocate memory
    ERROR_DUMMY_VAR        = -23,  //! Unknown function dummy variables
    //---------- Iterative method problems ----------------------------------------//
    ERROR_SOLVER_TYPE      = -30,  //! Unknown solver type
    ERROR_SOLVER_PRECTYPE  = -31,  //! Unknown preconditioner type
    ERROR_SOLVER_STAG      = -32,  //! Iterative solver stagnates
    ERROR_SOLVER_SOLSTAG   = -33,  //! Iterative solver's solution is too small
    ERROR_SOLVER_TOLSMALL  = -34,  //! Iterative solver's tolerance is too small
    ERROR_SOLVER_MAXIT     = -39,  //! Maximal iteration number reached
    //---------- AMG method problems ----------------------------------------------//
    ERROR_AMG_INTERP_TYPE  = -40,  //! Unknown AMG interpolation type
    ERROR_AMG_SMOOTH_TYPE  = -41,  //! Unknown AMG smoother type
    ERROR_AMG_COARSE_TYPE  = -42,  //! Unknown AMG coarsening type
    ERROR_AMG_COARSEING    = -43,  //! AMG coarsening step failed to complete
    ERROR_AMG_SETUP        = -49,  //! AMG setup failed to complete
    //---------- ILU method problems ----------------------------------------------//
    ERROR_ILU_SETUP        = -59,  //! ILU setup failed to complete
    //---------- Unknown problems (default) ---------------------------------------//
    ERROR_UNKNOWN          = -99,  //! Unknown error type
};

/// \brief Get error message from FaspErrorCode
const std::string getErrorCode(const FaspErrorCode code) {
    switch(code) {
        case ERROR_OPEN_FILE:
            return "Failed to open a file!";
        case ERROR_INPUT_FILE:
            return "Wrong input file!";
        case ERROR_INPUT_PAR:
            return "Wrong input argument!";
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

/// \brief Standard exception capturing
class FaspException : public std::runtime_error {

private:
    const std::string&  msg;   //! Which exception type caught
    const char*         file;  //! Which file throws exception
    const int           line;  //! Which line throws exception

public:
    // Default constructor
    FaspException (const std::string& msg_,
                   const char* file_,
                   const int line_) : std::runtime_error(msg_),
        	       msg (msg_), file (file_), line (line_)
    { }

    const int getLine() const { return line; } //! return thrown line number

	const char* getFile() const { return file; } //! return thrown file name
};

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

using namespace std;

int main()
{
    FaspErrorCode errorCode = FaspErrorCode::SUCCESS; // Return success if no-throw

	try {
	    errorCode = FaspErrorCode::ERROR_UNKNOWN; // Error type found
    	throw( FaspException(getErrorCode(errorCode), __FILE__, __LINE__) );
    }

    catch (FaspException& ex) {
    	cout << "### ERROR: " << ex.what() << endl;
        cout << "### ERROR: Check " << ex.getFile() << " at Line " << ex.getLine() << endl;
        return EXIT_FAILURE;
    }

    return errorCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/