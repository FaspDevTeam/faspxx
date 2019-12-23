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

/// Test three functions for error logging
TEST(ErrorLog, ABORT)
{
    ASSERT_DEATH({FASPXX_ABORT("This is an abort test!");}, "");
}

TEST(ErrorLog, ASSERT)
{
    FASPXX_ASSERT(1==2, "This is an assert test!");
}

TEST(ErrorLog, WARNING)
{
    FASPXX_WARNING("This is a warning test!");
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/