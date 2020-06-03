
#ifndef GMRES_HXX
#define GMRES_HXX

#include <cmath>
#include <queue>
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

class RGMRES : public SOL {

private:
    VEC vk;
    VEC wk;
    VEC p0;
    VEC pk;
    VEC qk;
    VEC var;

    std::vector<std::vector<double>> hh;
    std::vector<double> sin;
    std::vector<double> cos;
    std::vector<double> norms;

    std::queue<VEC> queue_v;

public:
    /// Default constructor.
    RGMRES() : vk(0), wk(0), p0(0), pk(0), qk(0), hh(0), sin(0), cos(0), var(0),
               norms(0) {};

    /// Default destructor.
    ~RGMRES() = default;

    /// Setup the GMRES method.
    FaspRetCode Setup(const LOP &A) override;

    /// Clean up GMRES data allocated during Setup.
    void Clean() override;

    /// Solve Ax=b using the GMRES method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

};

#endif //GMRES_HXX