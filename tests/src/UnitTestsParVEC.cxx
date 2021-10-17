#include "../catch.hxx"
#include "ParVEC.hxx"

TEST_CASE("ParVEC")
{

    SECTION("ParVEC: GenerateLocalPartitioning()")
    {
        std::cout << "TEST ParVEC: GenerateLocalPartitioning()" << std::endl;

        int global_size = 10;
        int num_procs = 3;

        int* partitioning = new int[2];

        int rank=0;
        GenerateLocalPartitioning(global_size, num_procs, rank, partitioning);
        REQUIRE(partitioning[0] == 0);
        REQUIRE(partitioning[1] == 4);

        rank = 1;
        GenerateLocalPartitioning(global_size, num_procs, rank, partitioning);
        REQUIRE(partitioning[0] == 4);
        REQUIRE(partitioning[1] == 7);

        rank = 2;
        GenerateLocalPartitioning(global_size, num_procs, rank, partitioning);
        REQUIRE(partitioning[0] == 7);
        REQUIRE(partitioning[1] == 10);

    }
}
