/**
 * SOL header file
 */

#ifndef SOL_HXX
#define SOL_HXX

#include "Param.hxx"
#include "VEC.hxx"
#include "LOP.hxx"
#include "RetCode.hxx"

class SOL{
protected:
    // preconditioner
    LOP *lop;

public:
    // default constructor
    SOL():lop(nullptr){};

    /// check and allocate memory
    virtual FaspRetCode Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param) = 0;

    /// build preconditioner operator
    virtual void SetupPCD(const LOP* lop) = 0;

    /// solve by SOL
    virtual FaspRetCode Solve(const LOP& A, const VEC& b, VEC& x,IterParam& param) = 0;

    /// clean preconditioner operator
    virtual void CleanPCD() = 0;

    /// Release temporary memory
    virtual void Clean() = 0;

    // destructor
    ~SOL(){};
};

#endif // SOL_HXX
