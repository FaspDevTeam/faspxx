/**
 * a header file for PCG
 */

#ifndef SRC_PCG_HXX
#define SRC_PCG_HXX

#include "MAT.hxx"

class PCG {
private:
    DBL rtol;
    INT maxIt;
    INT pflag;
    MAT A;
    MAT P;
    VEC b;

public:
    PCG() : rtol(0), maxIt(0), pflag(0) {};

    PCG(DBL &rtol_, DBL &maxIt_, INT &pflag_) : rtol(rtol_), maxIt(maxIt),
                                             pflag(pflag_) {};

    FaspRetCode SetUp(const MAT &A_, const VEC &b_, const DBL &rtol_, const INT
    &maxIt_);

    FaspRetCode SetUpPCD(const MAT &P);

    FaspRetCode Start(VEC &x, INT &MaxIter);

    void CleanPCD();

    void Clean();

};

#endif //SRC_PCG_HXX
