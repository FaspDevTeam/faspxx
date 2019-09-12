
#include <gtest/gtest.h>
#include <iostream>
#include "gtest_examples.cxx"

using namespace std;

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

