/** \file testErrorCode.cxx
 *  \brief Test error code output
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <string>
#include <iostream>
#include <exception>
#include "error.hxx"

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