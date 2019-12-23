/*! \file    testErrorLog.cxx
 *  \brief   This example shows how to log error messages
 *  \author  Ronghong Fan
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "ErrorLog.hxx"
#include <gtest/gtest.h>

/// Test three functions for error logging
TEST(ErrorLog, ErrorLog_E_Test)
{
    int* tmp = NULL;
    if ( tmp == NULL )
        FASPXX_ASSERT(1==2, "This is an assert test!");

    if ( tmp == NULL )
    FASPXX_WARNING("This is a warning test!");

    if ( tmp == NULL )
    FASPXX_ABORT("This is an abort test!");
}


/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/