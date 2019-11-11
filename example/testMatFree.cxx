/*! \file testMatFree.cxx
 *  \brief A demo for matrix-free FD solver for 2D Poisson's equation
 *             - \Delta u = f,  in \Omega = [0, 1]*[0, 1]
 *  \author Kailei Zhang, Chensong Zhang
 *  \date Nov/05/2019
 */

#include <iostream>
#include "LOP.hxx"
#include "PCG.hxx"
#include "Param.hxx"
#include "Timing.hxx"
#include "Poisson2D.hxx"

int dimen = 1024; // number of initial partition in X and Y directions

/// \brief Locate position of (x,y) in the global index
#define locate(row, column) (row - 1) * (dimen - 1) + column - 1


// Todo: Add detailed comments in this example!
/// \brief Matrix-free linear operator object
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
static void Rhs(int dimen, double *ptr) {

    const double h = 1.0 / dimen;

    for (int j = 0; j < (dimen - 1) * (dimen - 1); ++j) ptr[j] = 0.0;

    // interior points
    for (int k = 2; k <= dimen - 2; ++k) {
        for (int j = 2; j <= dimen - 2; ++j)
            ptr[locate(k, j)] = h * h * Load(k * h, j * h);
    }

    // left boundary
    for (int j = 2; j <= dimen - 2; ++j)
        ptr[locate(1, j)] = LeftBdyCond(0, j * h) + h * h * Load(1 * h, j * h);

    // right boundary
    for (int j = 2; j <= dimen - 2; ++j)
        ptr[locate(dimen - 1, j)] =
                RightBdyCond(1.0, j * h) + h * h * Load((dimen - 1) * h, j * h);

    // lower boundary
    for (int j = 2; j <= dimen - 2; ++j)
        ptr[locate(j, 1)] = LowerBdyCond(j * h, 0) + h * h * Load(j * h, 1 * h);

    // upper boundary
    for (int j = 2; j <= dimen - 2; ++j)
        ptr[locate(j, dimen - 1)] =
                UpperBdyCond(j * h, 1.0) + h * h * Load(j * h, (dimen - 1) * h);

    // left lower corner
    ptr[locate(1, 1)] =
            LeftBdyCond(0, h) + LowerBdyCond(h, 0) + h * h * Load(1 * h, 1 * h);
    // left upper corner
    ptr[locate(1, dimen - 1)] =
            LeftBdyCond(0, h * (dimen - 1)) + UpperBdyCond(h, 1.0) +
            h * h * Load(1 * h,
                         (dimen - 1) * h);
    // right lower corner
    ptr[locate(dimen - 1, 1)] =
            LowerBdyCond((dimen - 1) * h, 0) + RightBdyCond(1.0, h) + h * h * Load(
                    (dimen - 1) * h, 1 * h);
    // right upper corner
    ptr[locate(dimen - 1, dimen - 1)] = UpperBdyCond((dimen - 1) * h, 1.0)
                                        + RightBdyCond(1.0, (dimen - 1) * h) +
                                        h * h * Load((dimen - 1) * h,
                                                     (dimen - 1) * h);
}

// free-matrix 's operator : acting on a VEC object
void MatFree::Apply(const VEC &x, VEC &y) const {

    y.SetValues(x.GetSize(), 0.0);
    const int dimen1 = dimen - 1;
    int lower, left, center, right, upper;

    // interior points
    for (int k = 2; k <= nrow - 2; ++k) {
        for (int j = 2; j <= ncol - 2; ++j) {
            center = (k - 1) * dimen1 + j - 1;
            lower  = center - dimen1;
            left   = center - 1;
            right  = center + 1;
            upper  = center + dimen1;
            y[center] = -x[lower] - x[left] + 4.0 * x[center] - x[right] - x[upper];
        }
    }

    // lower boundary
    for (int j = 2; j <= ncol - 2; ++j) {
        center = j - 1;
        left   = center - 1;
        right  = center + 1;
        upper  = center + dimen1;
        y[center] = -x[left] + 4.0 * x[center] - x[right] - x[upper];
    }

    // upper boundary
    for (int j = 2; j <= ncol - 2; ++j) {
        center = (nrow - 2) * dimen1 + j - 1;
        lower  = center - dimen1;
        left   = center - 1;
        right  = center + 1;
        y[center] = -x[lower] - x[left] + 4.0 * x[center] - x[right];
    }

    // left boundary
    for (int k = 2; k <= nrow - 2; ++k) {
        center = (k - 1) * dimen1;
        lower  = center - dimen1;
        right  = center + 1;
        upper  = center + dimen1;
        y[center] = -x[lower] + 4.0 * x[center] - x[right] - x[upper];
    }

    // right boundary
    for (int k = 2; k <= nrow - 2; ++k) {
        center = (k - 1) * dimen1 + ncol - 2;
        lower  = center - dimen1;
        left   = center - 1;
        upper  = center + dimen1;
        y[center] = -x[lower] - x[left] + 4.0 * x[center] - x[upper];
    }

    // left lower corner
    center = locate(1, 1);
    right  = locate(1, 2);
    upper  = locate(2, 1);
    y[center] = 4.0 * x[center] - x[right] - x[upper];

    // left upper corner
    left   = locate(1, ncol - 2);
    center = locate(1, ncol - 1);
    right  = locate(2, ncol - 1);
    y[center] = -x[left] + 4.0 * x[center] - x[right];

    // right lower corner
    lower  = locate(nrow - 2, 1);
    center = locate(nrow - 1, 1);
    right  = locate(nrow - 1, 2);
    y[center] = -x[lower] + 4.0 * x[center] - x[right];

    // right upper corner
    lower  = locate(nrow - 2, ncol - 1);
    left   = locate(nrow - 1, ncol - 2);
    center = locate(nrow - 1, ncol - 1);
    y[center] = -x[lower] - x[left] + 4.0 * x[center];
}

int main(int argc, char *args[]) {

    // convergence parameter setting
    IterParam param;
    param.SetVerbose(PRINT_NONE);
    param.SetMaxIter(100);
    param.SetRelTol(1e-6);
    param.SetAbsTol(1e-10);
    param.SetRestart(20);
    param.Print();

    const int numTotalMesh = 2; // number of meshes in total
    int mesh = 1; // number of mesh refinement cycles
    double h = 0.0; // mesh size in X and Y directions
    GetWallTime timer;

    double *ptr = nullptr;
    bool markAllocDone = true;

    VEC b, x;

    while (mesh < numTotalMesh) {

        //dimen *= 2;
        h = 1.0 / dimen;
        std::cout << "(dimen-1)*(dimen-1) : " << (dimen - 1) * (dimen - 1)
                  << std::endl;

        // apply for new memory space and try to catch error
        if (ptr != nullptr) delete[] ptr;
        try {
            ptr = new double[(dimen - 1) * (dimen - 1)];
        } catch (std::bad_alloc &ex) {
            markAllocDone = false;
            std::cout << "bad allocation" << std::endl;
            break;
        }

        // compute rhs
        Rhs(dimen, ptr);

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
        std::cout << "Solving Ax=b costs "
                  << std::fixed << std::setprecision(3)
                  << timer.Stop() << "ms" << std::endl;

        pcg.Clean(); // clean preconditioner

        std::cout << std::scientific << std::setprecision(4)
                  << "NumIter : " << param.GetNumIter() << std::endl
                  << "Norm2   : " << param.GetNorm2() << std::endl
                  << "NormInf : " << param.GetNormInf() << std::endl;

        // l2-norm between numerical solution and continuous solution
        double norm2; // L2-norm of error
        double norm2Last; // L2-norm of error in previous step

        norm2 = 0.0;
        for (int j = 1; j <= dimen - 1; ++j) {
            for (int k = 1; k <= dimen - 1; ++k) {
                norm2 += pow(fabs(x[locate(j, k)] - ExactSolu(j * h, k * h)), 2.0);
            }
        }
        norm2 *= h * h; // use quadrature rule to compute L2-norm!

        std::cout << "L2 norm of error : "
                  << std::scientific << std::setprecision(4) << sqrt(norm2);
        if (mesh == 1) {
            std::cout << std::endl;
        } else {
            std::cout << ", Convergence rate : "
                      << std::fixed << std::setprecision(3)
                      << log(sqrt(norm2Last) / sqrt(norm2)) / log(2) << std::endl;
        }

        ++mesh;
        norm2Last = norm2; // store the previous error in L2-norm
    } // end while

    if (markAllocDone) delete[] ptr;

    return 0;
}
