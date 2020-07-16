
#ifndef FGMRES_HXX
#define FGMRES_HXX

#include <cmath>
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

class FGMRES : public SOL {

private:
    VEC wk;
    VEC safe;
    VEC tmp;

    std::vector<std::vector<double>> hh;
    std::vector<double> hsin;
    std::vector<double> hcos;
    std::vector<double> var;

    std::vector<VEC> V;
    std::vector<VEC> Z;

    int maxRestart;
    int minRestart;
    int restart;
    int len;

public:
    /// Default constructor.
    FGMRES() : wk(0), safe(0), tmp(0), hh(0), hsin(0), hcos(0), var(0),
              maxRestart{20}, minRestart(5) {};

    /// Default destructor.
    ~FGMRES() = default;

    /// Set the maximum and minmum restart numbers for variable FGMRES.
    void SetMaxMinRestart(const int maxRestart, const int minRestart);

    /// Setup the FGMRES method.
    FaspRetCode Setup(const LOP &A) override;

    /// Clean up FGMRES data allocated during setup.
    void Clean() override;

    /// Solve Ax=b using the FGMRES method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

};

#endif // FGMRES_HXX