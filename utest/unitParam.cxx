//
// Created by fan on 2019/10/22.
//

#include <gtest/gtest.h>
#include <iostream>

#include "../src/Param.hxx"

TEST(IterParam_Default, Default_Values)
{
    InitParam initparam;
    initparam.param.Print(std::cout);
    EXPECT_EQ(1, 1);
}







