/**
 * A demo for matrix-free implementation of 2D Poisson FD solver:
 * Solve the Poisson equation:
 *      - \Delta u = f,  in \Omega = [0, 1]*[0, 1]
 *      where f = M_PI*M_PI*(x*x+y*y)sin(M_PI*x*y)
 * The exact solution of this problem is u = sin(M_PI*x*y)
 */
// Todo: How to apply boundary condition?

#include <iostream>
#include "Timing.hxx"
#include "LOP.hxx"
#include "Param.hxx"
#include "PCG.hxx"
#include "Poisson2D.hxx"

// Todo: Add detailed comments in this example!
class MatFree : public LOP {
private:
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

/*
 * locate position
 */
// Todo: Why use two locate??? It should be same in the example.
static int locate(int row, int column, int dimen) {
    return (row - 1) * (dimen - 1) + column - 1;
}

static void Rhs(int dimen, double *ptr, double h) {

    for (int j = 0; j < (dimen - 1) * (dimen - 1); j++) ptr[j] = 0.0;

    for (int k = 2; k <= dimen - 2; k++) {
        for (int j = 2; j <= dimen - 2; j++)
            ptr[locate(k, j, dimen)] = h * h * f(k * h, j * h);
    }

    // left boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(1, j, dimen)] = left(0, j * h) + h * h * f(1 * h, j * h);

    // right boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(dimen - 1, j, dimen)] =
                right(1.0, j * h) + h * h * f((dimen - 1) * h, j * h);

    // lower boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(j, 1, dimen)] = lower(j * h, 0) + h * h * f(j * h, 1 * h);

    // upper boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(j, dimen - 1, dimen)] =
                upper(j * h, 1.0) + h * h * f(j * h, (dimen - 1) * h);

    // left lower corner
    ptr[locate(1, 1, dimen)] = left(0, h) + lower(h, 0) + h * h * f(1 * h, 1 * h);
    // left upper corner
    ptr[locate(1, dimen - 1, dimen)] =
            left(0, h * (dimen - 1)) + upper(h, 1.0) + h * h * f(1 * h,
                                                                 (dimen - 1) * h);
    // right lower corner
    ptr[locate(dimen - 1, 1, dimen)] =
            lower((dimen - 1) * h, 0) + right(1.0, h) + h * h * f(
                    (dimen - 1) * h, 1 * h);
    // right upper corner
    ptr[locate(dimen - 1, dimen - 1, dimen)] = upper((dimen - 1) * h, 1.0)
                                                  + right(1.0, (dimen - 1) * h) +
                                                  h * h * f((dimen - 1) * h,
                                                            (dimen - 1) * h);
}

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
            y[third] = -x[first] - x[second] + 4.0 * x[third] - x[fourth] - x[fifth];
        }
    }

    // left boundary
    for (int j = 2; j <= ncol - 2; j++) {
        second = locate(1, j - 1);
        third = locate(1, j);
        fourth = locate(1, j + 1);
        fifth = locate(2, j);
        y[third] = -x[second] + 4.0 * x[third] - x[fourth] - x[fifth];
    }

    // right boundary
    for (int j = 2; j <= ncol - 2; j++) {
        first = locate(nrow - 2, j);
        second = locate(nrow - 1, j - 1);
        third = locate(nrow - 1, j);
        fourth = locate(nrow - 1, j + 1);
        y[third] = -x[first] - x[second] + 4.0 * x[third] - x[fourth];
    }

    // lower boundary
    for (int k = 2; k <= nrow - 2; k++) {
        first = locate(k - 1, 1);
        third = locate(k, 1);
        fourth = locate(k, 2);
        fifth = locate(k + 1, 1);
        y[third] = -x[first] + 4.0 * x[third] - x[fourth] - x[fifth];
    }

    // upper boundary
    for (int k = 2; k <= nrow - 2; k++) {
        first = locate(k - 1, ncol - 1);
        second = locate(k, ncol - 2);
        third = locate(k, ncol - 1);
        fifth = locate(k + 1, ncol - 1);
        y[third] = -x[first] - x[second] + 4.0 * x[third] - x[fifth];
    }

    // left lower corner
    third = locate(1, 1);
    fourth = locate(1, 2);
    fifth = locate(2, 1);
    y[third] = 4.0 * x[third] - x[fourth] - x[fifth];

    // left upper corner
    second = locate(1, ncol - 2);
    third = locate(1, ncol - 1);
    fourth = locate(2, ncol - 1);
    y[third] = -x[second] + 4.0 * x[third] - x[fourth];

    // right lower corner
    first = locate(nrow - 2, 1);
    third = locate(nrow - 1, 1);
    fourth = locate(nrow - 1, 2);
    y[third] = -x[first] + 4.0 * x[third] - x[fourth];

    // right upper corner
    first = locate(nrow - 2, ncol - 1);
    second = locate(nrow - 1, ncol - 2);
    third = locate(nrow - 1, ncol - 1);
    y[third] = -x[first] - x[second] + 4.0 * x[third];
}

int main(int argc, char *args[]) {

    const int dimen = 32;
    const double h = 1.0 / dimen;
    double *ptr;
    GetWallTime timer;

    ptr = new double[(dimen - 1) * (dimen - 1)];
    Rhs(dimen, ptr, h); // Todo: ptr is never released?

    VEC b((dimen - 1) * (dimen - 1), ptr);

    VEC x((dimen - 1) * (dimen - 1), 0.25);

    delete[] ptr;

    MatFree matfree(dimen, dimen);

    IterParam param;
    param.SetVerbose(PRINT_NONE);
    param.SetMaxIter(5000);
    param.SetRelTol(1e-6);
    param.SetAbsTol(1e-10);
    param.SetRestart(25);
    param.Print();

    PCG pcg;
    pcg.Setup(matfree, b, x, param);

    IdentityLOP lop((dimen - 1) * (dimen - 1));
    pcg.SetupPCD(&lop);

    timer.Start();
    pcg.Solve(matfree, b, x, param);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

    pcg.Clean();

    std::cout << "NumIter : " << param.GetNumIter() << std::endl;
    std::cout << "Norm2   : " << param.GetNorm2() << std::endl;
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