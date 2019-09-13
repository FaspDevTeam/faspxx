/** \file testGoogleTest.cxx
 *  \brief Test GTest unit-test suite
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include "simple.cxx"

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
