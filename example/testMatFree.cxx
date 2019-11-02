/**
 * a demo about mat-free method
 */

#include "LOP.hxx"
#include <iostream>
#include "Param.hxx"
#include "PCG.hxx"
#include "Aux_testMatFree.hxx"

class MatFree : public LOP {
    int locate(const int r, const int c) const {
        return (r - 1) * (nrow - 1) + c - 1;
    }

public:
    MatFree(const int row = 20, const int col = 20) {
        nrow = row;
        ncol = col;
    }

    void Apply(const VEC &x, VEC &y) const;
};

void MatFree::Apply(const VEC &x, VEC &y) const {
    y.SetValues(x.GetSize(), 0.0);
    int first, second, third, fourth, fifth;

    // interior points
    for (int k = 2; k <= nrow - 2; k++) {
        for (int j = 2; j <= ncol - 2; j++) {
            first = locate(k - 1, j);
            second = locate(k, j - 1);
            third = locate(k, j);
            fourth = locate(k, j + 1);
            fifth = locate(k + 1, j);
            y[third] = -x[first] - x[second] + 4 * x[third] - x[fourth] - x[fifth];
        }
    }

    // left boundary
    for (int j = 2; j <= ncol - 2; j++) {
        second = locate(1, j - 1);
        third = locate(1, j);
        fourth = locate(1, j + 1);
        fifth = locate(2, j);
        y[third] = -x[second] + 4 * x[third] - x[fourth] - x[fifth];
    }

    // right boundary
    for (int j = 2; j<= ncol - 2; j++) {
        first = locate(nrow - 2, j);
        second = locate(nrow - 1, j - 1);
        third = locate(nrow - 1, j);
        fourth = locate(nrow - 1, j + 1);
        y[third] = -x[first] - x[second] + 4 * x[third] - x[fourth];
    }

    // lower boundary
    for (int k = 2; k <= nrow - 2; k++) {
        first = locate(k - 1, 1);
        third = locate(k, 1);
        fourth = locate(k, 2);
        fifth = locate(k + 1, 1);
        y[third] = -x[first] + 4 * x[third] - x[fourth] - x[fifth];
    }

    // upper boundary
    for (int k = 2; k <= nrow - 2; k++) {
        first = locate(k - 1, ncol - 1);
        second = locate(k, ncol - 2);
        third = locate(k, ncol - 1);
        fifth = locate(k + 1, ncol - 1);
        y[third] = -x[first] - x[second] + 4 * x[third] - x[fifth];
    }

    // left lower corner
    third = locate(1, 1);
    fourth = locate(1, 2);
    fifth = locate(2, 1);
    y[third] = 4 * x[third] - x[fourth] - x[fifth];

    // left upper corner
    second = locate(1, ncol - 2);
    third = locate(1, ncol - 1);
    fourth = locate(2, ncol - 1);
    y[third] = -x[second] + 4 * x[third] - x[fourth];

    // right lower corner
    first = locate(nrow - 2, 1);
    third = locate(nrow - 1, 1);
    fourth = locate(nrow - 1, 2);
    y[third] = -x[first] + 4 * x[third] - x[fourth];

    // right upper corner
    first = locate(nrow - 2, ncol - 1);
    second = locate(nrow - 1, ncol - 2);
    third = locate(nrow - 1, ncol - 1);
    y[third] = -x[first] - x[second] + 4 * x[third];

}

class Scaling:public LOP{
private:
    VEC diag;
public:
    Scaling(const std::vector<double>& diag) {
        this->diag.SetValues(diag);
        this->nrow = diag.size();
        this->ncol = diag.size();
    }
    void Apply(const VEC& x, VEC& y) const {
        y = x;
        y.PointwiseDivide(diag);
    }
};

int main(int argc, char *args[]) {

    const int dimen = 200;
    double h = 1.0 / dimen;
    double *ptr;

    Rhs(dimen, &ptr, h);

    VEC rhs((dimen - 1) * (dimen - 1), ptr);

    //VEC rhs((dimen-1)*(dimen-1),0.0);
    MatFree matfree(dimen, dimen);

    IterParam param;

    param.SetOutLvl(PRINT_MAX);
    param.SetMaxIter(1000);
    param.SetRelTol(1e-6);
    param.SetAbsTol(1e-10);
    param.SetRestart(25);

    VEC x((dimen - 1) * (dimen - 1), 0.25);

    PCG pcg;
    pcg.Setup(&matfree, rhs, x, param);
#if 0
    std::vector<DBL> diag((dimen-1)*(dimen-1),4);
    Scaling jac(diag);
    pcg.SetupPCD(&jac);
#else
    IdentityLOP lop((dimen - 1) * (dimen - 1));
    pcg.SetupPCD(&lop);
#endif

    pcg.Solve(&matfree, rhs, x, param);
    pcg.Clean();

    param.Print();

    std::cout << "Norm2 : " << param.GetNorm2() << std::endl;
    std::cout << "NormInf : " << param.GetNormInf() << std::endl;
    std::cout << "NumIter : " << param.GetNumIter() << std::endl;

    double realtol = 0.0;
    for (int j = 1; j <= dimen - 1; j++) {
        for (int k = 1; k <= dimen - 1; k++) {
            if (realtol < fabs(x[locate(j, k, dimen)] - exact_solu(j * h, k * h)))
                realtol = fabs(x[locate(j, k, dimen)] - exact_solu(j * h, k * h));
        }
    }
    std::cout << "Real tolerance : " << realtol << std::endl;

    return 0;
}