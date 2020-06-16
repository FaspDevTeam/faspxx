
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
    VEC wk;
    VEC safe;
    VEC tmp;

    std::vector<std::vector<double>> hh;
    std::vector<double> sin;
    std::vector<double> cos;
    std::vector<double> var;
    std::vector<double> norms;

    std::vector<VEC> V;

    int maxRestart;
    int minRestart;

public:
    /// Default constructor.
    RGMRES() : wk(0), safe(0), tmp(0), hh(0), sin(0), cos(0),
               var(0), norms(0), maxRestart{20}, minRestart(5) {};

    /// Default destructor.
    ~RGMRES() = default;

    /// set the maximum and minmum restart
    void SetMaxMinRestart(int maxRestart, int minRestart);

    /// Setup the GMRES method.
    FaspRetCode Setup(const LOP &A) override;

    /// Clean up GMRES data allocated during Setup.
    //void Clean() override;

    /// Solve Ax=b using the GMRES method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

};

#endif //GMRES_HXX