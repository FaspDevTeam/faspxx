#include "ParVEC.hxx"


//
void GeneratePartitioning(int global_size, int num_procs, int* partitioning)
{

}

// partitioning is a dim=2 int array
void GenerateLocalPartitioning(int global_size, int numprocs, int rank, int* partitioning)
{
    int avg_size, rest;
    avg_size = global_size / numprocs;
    rest = global_size - avg_size * numprocs;
//        rest = global_size % numprocs;

    partitioning[0] = rank * avg_size;
    partitioning[0] += std::min(rank, rest);

    partitioning[1] = avg_size * (rank + 1);
    partitioning[1] += std::min(rank + 1, rest);
}


ParVEC::ParVEC(MPI_Comm _comm, int _global_size, int* _partitioning)
{
    comm = _comm;
    global_size = _global_size;

    if (!_partitioning)
    {
        int rank, numprocs;
        MPI_Comm_rank(comm, &rank);
        MPI_Comm_size(comm, &numprocs);


        _partitioning = new int [2];
        GenerateLocalPartitioning(_global_size, numprocs, rank, _partitioning);
        owns_part = true;
    }

    first_index = _partitioning[0];
    last_index = _partitioning[1] - 1;
    partitioning = _partitioning;

    local_vec = new VEC(_partitioning[1] - _partitioning[0]);
    owns_vec = true;
}

ParVEC::~ParVEC()
{
    if (owns_vec)
        delete local_vec;
}