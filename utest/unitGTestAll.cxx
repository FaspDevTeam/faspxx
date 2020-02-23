/** \file    testGTestAll.cxx
 *  \brief   GTest unit-test suite
 *  \author  Ronghong Fan
 *  \date    Oct/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>

/*---------------------------------*/
/*--       Unit Test Suite       --*/
/*---------------------------------*/

#include "unitVEC.cxx"
#include "unitMAT.cxx"
#include "unitParam.cxx"
#include "unitJacobi.cxx"

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