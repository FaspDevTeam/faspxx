/*! \file    UnitTestsErrorLog.cxx
 *  \brief   Unit tests for ErrorLog class
 *  \author  Ronghong Fan
 *  \date    Oct/10/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>

#include "../catch.hxx"
#include "ErrorLog.hxx"

TEST_CASE("ErrorLog")
{
    std::cout << "TEST ErrorLog" << std::endl;

    SECTION("test1")
    {
        // Demo for usage of catch.hxx
        REQUIRE(1 == 1);
        REQUIRE(1 != 2);

        //FASPXX_ABORT("This is an abort test!");
        //FASPXX_ASSERT(1 == 2, "This is an assert test!");
        //FASPXX_WARNING("This is a warning test!");
    }
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Ronghong Fan        Oct/10/2021      Create file                          */
/*----------------------------------------------------------------------------*/