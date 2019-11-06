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
    const LOP *lop;
    VEC r,rt,p,v,ph,xhalf,s,sh,t,xmin,tmp;

    /// Print out final status of an iterative method
    void PrintFinal(const PRTLVL& prtlvl, const INT& iter, const INT& maxit,
                    const DBL& relres);
public:
    /// constructor by default
    BiCGStab():lop(nullptr),r(0),rt(0),p(0),v(0),ph(0),
    xhalf(0),s(0),sh(0),t(0),xmin(0),tmp(0){}

    /// check and allocate memory
    FaspRetCode Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param);

    /// build preconditioner operator
    void SetupPCD(const LOP* lop);

    /// solve by PCG
    FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x,IterParam& param);

    /// clean preconditioner operator
    void CleanPCD();

    /// Release temporary memory
    void Clean();

    /// destructor
    ~BiCGStab() {};

};

void fasp_itinfo(PRTLVL,INT,DBL,DBL,DBL);

#endif // BICGSTAB_HXX
