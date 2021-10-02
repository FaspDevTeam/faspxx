#include <iostream>

#include "ErrorLog.hxx"

#include "../catch.hxx"

TEST_CASE("ErrorLog")
{
    std::cout << "TEST ErrorLog" << std::endl;

    SECTION("test1")
    {
        // Demo for usage of catch.hxx
        REQUIRE(1 == 1);
        REQUIRE(1 != 2);

//        ASSERT_DEATH({ FASPXX_ABORT("This is an abort test!"); }, "");
//        FASPXX_ASSERT(1 == 2, "This is an assert test!");
//        FASPXX_WARNING("This is a warning test!");
    }

}