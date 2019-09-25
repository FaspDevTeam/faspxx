/** \file testErrorCode.cxx
 *  \brief Test error code output
 *
 *  To compile this test, you may (after cmake) use
 *      $ make testErrorCode
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include "error.hxx"

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

using namespace std;

int main()
{
    FaspErrorCode errorCode = FaspErrorCode::SUCCESS; // Return success if no-throw

    cout << "Initial error code : " << errorCode << endl;

    try {
	    errorCode = FaspErrorCode::ERROR_UNKNOWN; // Error type found
    	throw( FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__, __LINE__) );
    }
    catch (FaspException& ex) {
    	cout << "### ERROR: " << ex.what() << endl;
        cout << "### ERROR: Check " << ex.getFile() << " at Line " << ex.getLine() << endl;
        return errorCode;
    }

    // This line should never be reached!
    cout << "Final error code : " << errorCode << endl;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/