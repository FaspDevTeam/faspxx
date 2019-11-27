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
    VEC rj;
    VEC r0;
    VEC pj;
    VEC sj;
    VEC ph;
    VEC xh;
    VEC qj;
    VEC sh;
    VEC yj;
    VEC xmin;
    VEC tmp;

public:
    /// constructor by default
    BiCGStab():rj(0),r0(0),pj(0),sj(0),ph(0),xh(0),
        qj(0),sh(0),yj(0),xmin(0),tmp(0){}

    /// check and allocate memory
    FaspRetCode Setup(const Mat& A,const VEC& b,VEC& x);

    /// solve by BiCGStab
    FaspRetCode Solve(const VEC& b, VEC& x);

    /// destructor
    ~BiCGStab() {};

};

#endif // BICGSTAB_HXX