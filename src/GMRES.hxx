
#ifndef GMRES_HXX
#define GMRES_HXX

#include <cmath>
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"
#include "VecCluster.hxx"

class GMRES: public SOL
{

private:
    VEC rk;
    VEC wk;
    VEC vk;
    VEC safe;
    VEC betae1;
    VEC tmp;
    VEC yk;
    std::vector<DBL> Hij;
    VecCluster cluster;

public:

    inline INT SetPosition(INT row,INT col);

    void Arnoldi(std::vector<DBL>& Hij,VecCluster& cluster);

    void HouseHolder(std::vector<DBL>& Hij,VEC& b);

    void SolveMin(std::vector<DBL> Hij, VEC b,VEC& x);

public:
    /// Default constructor.
    GMRES() : rk(0),  wk(0), vk(0), safe(0),
        Hij(0),cluster(0,0){};

    /// Default destructor.
    ~GMRES() = default;

    /// Setup the CG method.
    FaspRetCode Setup(const LOP& A) override;

    /// Solve Ax=b using the CG method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;
};





#endif //GMRES_HXX