/**
 * a header file for PCG
 */

#ifndef SRC_PCG_HXX
#define SRC_PCG_HXX

#include <cfloat>
#include "MAT.hxx"
#include "LOP.hxx"
#include <cmath>

enum StopType{
    STOP_REL_RES,
    STOP_REL_PRECRES,
    STOP_MOD_REL_RES
};

class PCG {
private:
    DBL rtol;
    INT maxIt;
    INT pc;
    MAT A;
    MAT P;
    VEC b;

    void ApplyPreconditioner(){};

public:
    PCG() : rtol(0), maxIt(0), pc(0) {};

    PCG(DBL &rtol_, DBL &maxIt_, INT &pc_) : rtol(rtol_), maxIt(maxIt),
                                             pc(pc) {};

    FaspRetCode SetUp(const MAT &A_, const VEC &b_, const DBL &rtol_, const INT
    &maxIt_);

    FaspRetCode SetUpPCD(const MAT &P);

    FaspRetCode Start(VEC &x, INT &MaxIter,StopType type);

    void CleanPCD();

    void Clean();

};

#endif //SRC_PCG_HXX
