/**
 * BiCGStab algorithm
 */

#ifndef BICGSTAB_HXX
#define BICGSTAB_HXX

#include <cmath>
#include <cfloat>
#include "MAT.hxx"
#include "LOP.hxx"
#include "Param.hxx"
#include "ErrorLog.hxx"

class BiCGStab{
private:
    LOP *lop;
    VEC r0star;
    VEC pj;
    VEC rj;
    VEC sj;
    VEC tmp;
    VEC apj;
    VEC asj;

public:
    /// constructor by default
    BiCGStab():lop(nullptr),r0star(0),pj(0),rj(0),sj(0),
        tmp(0),apj(0),asj(0){}

    /// check and allocate memory
    FaspRetCode Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param);

    /// build preconditioner operator
    void SetupPCD(LOP *lop);

    /// solve by PCG
    FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x,IterParam& param);

    /// clean preconditioner operator
    void CleanPCD();

    /// Release temporary memory
    void Clean();

    /// destructor
    ~BiCGStab() {};

};

#endif // BICGSTAB_HXX
