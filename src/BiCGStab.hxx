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
    VEC r0;
    VEC rj;
    VEC pj;
    VEC gj;
    VEC sj;
    VEC qj;
    VEC uj;
    VEC yj;
    VEC xj;
    VEC tmp;

public:
    /// constructor by default
    BiCGStab():r0(0),rj(0),pj(0),gj(0),sj(0),qj(0),
        uj(0),yj(0),xj(0),tmp(0){}

    /// check and allocate memory
    FaspRetCode Setup(const Mat& A,const VEC& b,VEC& x);

    /// build preconditioner operator
    void SetPC(LOP *lop);

    /// solve by BiCGStab
    FaspRetCode Solve(const VEC& b, VEC& x);

    /// clean preconditioner operator
    void CleanPCD();

    /// Release temporary memory
    void Clean();

    /// destructor
    ~BiCGStab() {};

};

#endif // BICGSTAB_HXX