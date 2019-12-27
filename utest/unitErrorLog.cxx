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

#include <gtest/gtest.h>
#include "ErrorLog.hxx"

/// Test FASPXX_ABORT error logging.
TEST(ErrorLog, ABORT)
{
    ASSERT_DEATH({FASPXX_ABORT("This is an abort test!");}, "");
}

/// Test FASPXX_ASSERT error logging.
TEST(ErrorLog, ASSERT)
{
    FASPXX_ASSERT(1==2, "This is an assert test!");
}

/// Test FASPXX_WARNING error logging.
TEST(ErrorLog, WARNING)
{
    FASPXX_WARNING("This is a warning test!");
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/