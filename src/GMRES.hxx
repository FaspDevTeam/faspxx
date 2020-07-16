
#ifndef GMRES_HXX
#define GMRES_HXX

#include <cmath>
#include "ErrorLog.hxx"
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

class GMRES : public SOL {

private:
    VEC wk;
    VEC safe;
    VEC tmp;

    std::vector<std::vector<double>> hh;
    std::vector<double> hsin;
    std::vector<double> hcos;
    std::vector<double> var;

    std::vector<VEC> V;

    int maxRestart;
    int minRestart;
    int restart;
    int len;

public:
    /// Default constructor.
    GMRES() : wk(0), safe(0), tmp(0), hh(0), hsin(0), hcos(0), var(0),
              maxRestart{20}, minRestart(5) {};

    /// Default destructor.
    ~GMRES() = default;

    /// Set the maximum and minmum restart numbers for variable GMRES.
    void SetMaxMinRestart(const int maxRestart, const int minRestart);

    /// Setup the GMRES method.
    FaspRetCode Setup(const LOP &A) override;

    /// Clean up GMRES data allocated during setup.
    void Clean() override;

private:
    /// Solve Ax=b using the GMRES method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

public:
    /// Right-preconditioned GMRES solver.
    FaspRetCode RSolve(const VEC &b, VEC &x);

    /// Left-preconditioned GMRES slover.
    FaspRetCode LSolve(const VEC &b, VEC &x);

};

#endif // GMRES_HXX