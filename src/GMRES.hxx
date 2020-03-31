
#ifndef GMRES_HXX
#define GMRES_HXX

#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"
#include "VecCluster.hxx"

class GMRES{
    friend class LGMRES;
    friend class RGMRES;
private:
    void SolveMin(std::vector<DBL> H,VEC betae1,VEC qk);
};

class LGMRES: public SOL{
private:
    VEC p0;
    VEC pk;
    VEC r0;
    VEC tmp;
    VecCluster V;
    VEC vk;
    VEC wk;
    VEC betae1;
    std::vector<DBL> H;
public:
    // default construction
    LGMRES():p0(0),pk(0),r0(0),tmp(0),V(0,0),
        vk(0),wk(0),betae1(0),H(0){};

    // destruction
    ~LGMRES(){};

    /// Setup the LGMRES method.
    FaspRetCode Setup(const LOP& A) override;

    /// Clean up LGMRES data allocated during Setup.
    void Clean() override;

    /// Solve Ax=b using the LGMRES method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;
};
#endif //GMRES_HXX