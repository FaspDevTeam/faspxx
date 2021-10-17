
#include "../src/ParVEC.hxx"
#include <iostream>

using namespace std;


int main()
{
    int global_size = 10;
    int num_procs = 3;

    int rank=1;

    int* partitioning = new int[2];
    GenerateLocalPartitioning(global_size, num_procs, rank, partitioning);
    cout << "partitioning: " << partitioning[0] << ", " << partitioning[1] << endl;
}