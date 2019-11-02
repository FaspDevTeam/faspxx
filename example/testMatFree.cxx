/**
 * A demo for matrix-free implementation of 2D Poisson FD solver:
 * Solve the Poisson equation:
 *      - \Delta u = f,  in \Omega = [0, 1]*[0, 1]
 *      where f = -(x*x+y*y)sin(x*y)
 * The exact solution of this problem is u = sin(x*y)
 */

#include <iostream>
#include "Timing.hxx"
#include "LOP.hxx"
#include "Param.hxx"
#include "PCG.hxx"
#include "testMatFree.hxx"

class MatFree : public LOP {
    int locate(const int r, const int c) const {
        return (r - 1) * (nrow - 1) + c - 1;
    }

public:
    MatFree(const int row = 32, const int col = 32) {
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

int main(int argc, char *args[]) {

    const int dimen = 1024;
    double h = 1.0 / dimen;
    double *ptr;
    GetWallTime timer;

    Rhs(dimen, &ptr, h);

    VEC rhs((dimen - 1) * (dimen - 1), ptr);

    MatFree matfree(dimen, dimen);

    IterParam param;
    param.SetVerbose(PRINT_NONE);
    param.SetMaxIter(100);
    param.SetRelTol(1e-6);
    param.SetAbsTol(1e-10);
    param.SetRestart(25);
    param.Print();

    VEC x((dimen - 1) * (dimen - 1), 0.25);


    PCG pcg;
    pcg.Setup(&matfree, rhs, x, param);

    IdentityLOP lop((dimen - 1) * (dimen - 1));
    pcg.SetupPCD(&lop);

    timer.Start();
    pcg.Solve(&matfree, rhs, x, param);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

    pcg.Clean();

    std::cout << "NumIter : " << param.GetNumIter() << std::endl;
    std::cout << "Norm2   : " << param.GetNorm2()   << std::endl;
    std::cout << "NormInf : " << param.GetNormInf() << std::endl;

    double realErr = 0.0;
    for (int j = 1; j <= dimen - 1; j++) {
        for (int k = 1; k <= dimen - 1; k++) {
            if (realErr < fabs(x[locate(j, k, dimen)] - exact_solu(j * h, k * h)))
                realErr = fabs(x[locate(j, k, dimen)] - exact_solu(j * h, k * h));
        }
    }
    std::cout << "Error in inf-norm : " << realErr << std::endl;

    return 0;
}