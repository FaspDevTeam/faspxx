/** \file unitVEC.cxx
 *  \brief Unit test for VEC class implementations
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "vec.hxx"

VEC v0;
VEC v1(4,2.312);

int g = 0;

std::vector<double> v2{1.23, 0.98, 234.67, 456};

/*---------------------------------*/
/*--     Beginning of TEST       --*/
/*---------------------------------*/

TEST(VEC_size, Size)
{
    EXPECT_EQ(v0.GetSize(), 0);
    EXPECT_EQ(v1.GetSize(), 4);
}

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