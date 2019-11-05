/**
 * BiCGStab algorithm
 */

#ifndef BICGSTAB_HXX
#define BICGSTAB_HXX

class BiCGStab{
private:
    const LOP *lop;

public:
    /// constructor by default
    BiCGStab():lop(nullptr){}

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
    ~PCG() {};

};

#endif // BICGSTAB_HXX
