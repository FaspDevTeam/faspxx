#ifndef FASPXX_PARVEC_HXX
#define FASPXX_PARVEC_HXX

#include "mpi.h"

#include "VEC.hxx"

void GeneratePartitioning(int global_size, int num_procs, int* partitioning);
void GenerateLocalPartitioning(int global_size, int numprocs, int rank, int* partitioning);

class ParVEC
{

private:
    MPI_Comm comm;

    int global_size;
    int first_index; // local vector in a parallel vector must be stored at a continuous chunk of memory
    int last_index;
    int* partitioning;

    VEC* local_vec;

    bool owns_vec = false;
    bool owns_part = false;

public:
    ParVEC(MPI_Comm _comm, int _global_size, int* _partitioning);

    ~ParVEC();

    const MPI_Comm& GetComm() const { return comm; }
    const VEC* GetLocalVEC() const { return local_vec; }
    const int* GetPartitioning() const { return partitioning; }

    const int GetGlobalSize() const { return global_size; }
    const int GetLocalSize() const { return last_index - first_index; }

};

#endif // FASPXX_PARVEC_HXX
