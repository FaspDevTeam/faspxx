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

// Number of parts divided in X or Y direction
int dimen = 8;

// locate position
static int locate(int row, int column) {
    return (row - 1) * (dimen - 1) + column - 1;
}

// Todo: Add detailed comments in this example!
// free-matrix objects
class MatFree : public LOP {
public:

    // constructor by default
    MatFree(const int row = 32, const int col = 32) {
        nrow = row;
        ncol = col;
    }

    // free-matrix 's operator : acting on a VEC object
    void Apply(const VEC &x, VEC &y) const;
};

// generate rhs
static void Rhs(int dimen, double *ptr, double h) {

    for (int j = 0; j < (dimen - 1) * (dimen - 1); j++) ptr[j] = 0.0;

    // interior points
    for (int k = 2; k <= dimen - 2; k++) {
        for (int j = 2; j <= dimen - 2; j++)
            ptr[locate(k, j)] = h * h * f(k * h, j * h);
    }

    // left boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(1, j)] = left(0, j * h) + h * h * f(1 * h, j * h);

    // right boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(dimen - 1, j)] =
                right(1.0, j * h) + h * h * f((dimen - 1) * h, j * h);

    // lower boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(j, 1)] = lower(j * h, 0) + h * h * f(j * h, 1 * h);

    // upper boundary
    for (int j = 2; j <= dimen - 2; j++)
        ptr[locate(j, dimen - 1)] =
                upper(j * h, 1.0) + h * h * f(j * h, (dimen - 1) * h);

    // left lower corner
    ptr[locate(1, 1)] = left(0, h) + lower(h, 0) + h * h * f(1 * h, 1 * h);
    // left upper corner
    ptr[locate(1, dimen - 1)] =
            left(0, h * (dimen - 1)) + upper(h, 1.0) + h * h * f(1 * h,
                                                                 (dimen - 1) * h);
    // right lower corner
    ptr[locate(dimen - 1, 1)] =
            lower((dimen - 1) * h, 0) + right(1.0, h) + h * h * f(
                    (dimen - 1) * h, 1 * h);
    // right upper corner
    ptr[locate(dimen - 1, dimen - 1)] = upper((dimen - 1) * h, 1.0)
                                        + right(1.0, (dimen - 1) * h) +
                                        h * h * f((dimen - 1) * h,
                                                  (dimen - 1) * h);
}

// free-matrix 's operator : acting on a VEC object
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

    // convergence parameter setting
    IterParam param;
    param.SetVerbose(PRINT_NONE);
    param.SetMaxIter(10000);
    param.SetRelTol(1e-16);
    param.SetAbsTol(1e-12);
    param.SetRestart(25);
    param.Print();

    int count = 1; // number of mark cycles
    double h = 0.0;
    double *ptr = nullptr;
    GetWallTime timer;
    double order; // convergence order
    double tmp;

    double tmpnorm2 = 0.0; // temporary l2-norm of residuals
    double norm2; // l2-norm of residuals
    int mark = 0;
    VEC b, x;

    while (count < 5) {

        dimen *= 2;
        h = 1.0 / dimen;

        // apply for new memory space and try to catch error
        if (ptr == nullptr) {
            try {
                ptr = new double[(dimen - 1) * (dimen - 1)];
            } catch (std::bad_alloc &ex) {
                mark = 1;
                std::cout << "bad allocation" << std::endl;
                break;
            }
        } else {
            // free up the memory
            delete[] ptr;
            try { // apply for new memory space and try to catch error
                ptr = new double[(dimen - 1) * (dimen - 1)];
            } catch (std::bad_alloc &ex) {
                mark = 1;
                std::cout << "bad allocation" << std::endl;
                break;
            }
        }

        // compute rhs
        Rhs(dimen, ptr, h);

        // create b and x and assign values to them
        b.SetValues((dimen - 1) * (dimen - 1), ptr);
        x.SetValues((dimen - 1) * (dimen - 1), 0.25);

        // create free-matrix object
        MatFree matfree(dimen, dimen);

        // create PCG object
        PCG pcg;
        pcg.Setup(matfree, b, x, param);

        // create identity preconditioner
        IdentityLOP lop((dimen - 1) * (dimen - 1));
        pcg.SetupPCD(&lop);

        // time
        timer.Start();
        pcg.Solve(matfree, b, x, param); // solve by free-matrix method
        std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

        pcg.Clean(); // clean preconditioner

        //std::cout << "NumIter : " << param.GetNumIter() << std::endl;
        std::cout << "Norm2   : " << param.GetNorm2() << std::endl;
        //std::cout << "NormInf : " << param.GetNormInf() << std::endl;

        // l2-norm between numerical solution and continuous solution
        norm2 = 0.0;
        for (int j = 1; j <= dimen - 1; j++) {
            for (int k = 1; k <= dimen - 1; k++) {
                tmp = fabs(x[locate(j, k)] - exact_solu(j * h, k * h));
                norm2 += tmp * tmp;
            }
        }

        if (tmpnorm2 == 0.0)
            tmpnorm2 = norm2;
        else {
            std::cout << "tmpnorm2 : " << tmpnorm2 << std::endl;
            std::cout << "norm2 : " << norm2 << std::endl;
            order = log(tmpnorm2 / norm2) / log(2); // compute the convergence order
            tmpnorm2 = norm2;
            std::cout << "the convergence order : " << order << std::endl;
        }

        count++;
    } // end while
    if (mark == 0)
        delete[] ptr;

    return 0;
}