/*! \file    testErrorLog.cxx
 *  \brief   Test error log messages
 *  \author  Ronghong Fan
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "ErrorLog.hxx"

/// Test three functions for error logging
void test( int* tmp = NULL )
{
    if ( tmp == NULL )
        FASPXX_ASSERT(false, "This is an assert test!");

    if ( tmp == NULL )
        FASPXX_WARNING("This might cause a problem!");

    if ( tmp == NULL )
        FASPXX_ABORT("This is an abort test!");
}

int main(int argc, char* argv[])
{
    test();
    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/