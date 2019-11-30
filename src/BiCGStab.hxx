/**
 * BiCGStab algorithm
 */

#ifndef BICGSTAB_HXX
#define BICGSTAB_HXX

#include <cmath>
#include <cfloat>
#include "MAT.hxx"
#include "LOP.hxx"
#include "ErrorLog.hxx"
#include "SOL.hxx"

class BiCGStab:public SOL{
private:
    VEC r0star;
    VEC tmp;
    VEC apj;
    VEC asj;
    VEC pj;
    VEC rj;
    VEC sj;
    VEC ptmp;
    VEC stmp;
    VEC mp;
    VEC ms;

public:
    /// constructor by default
    BiCGStab():r0star(0),tmp(0),apj(0),asj(0),pj(0),
        rj(0),sj(0),ptmp(0),stmp(0),mp(0),ms(0){};

    /// check and allocate memory
    FaspRetCode Setup(const Mat& A);

    /// solve by BiCGStab
    FaspRetCode Solve(const VEC& b, VEC& x);

    /// destructor
    ~BiCGStab() {};

};

#endif // BICGSTAB_HXX