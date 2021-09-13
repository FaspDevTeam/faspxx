/*! \file    testMatFree.cxx
 *  \brief   Test performance of matrix-free 5P2D solver for the Poisson's equation
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/05/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "CG.hxx"
#include "Iter.hxx"
#include "LOP.hxx"
#include "Param.hxx"
#include "Poisson2D.hxx"
#include "Timing.hxx"

int dim = 16; // number of partitions in X and Y directions: 16x16 grid

/// Find the 1-dim global index of (x,y)
#define locate(row, column) (((row)-1) * (dim - 1) + (column)-1)

/// Matrix-free linear operator object.
class MatFree : public LOP
{

public:
    /// Default constructor.
    MatFree(const INT row, const INT col)
    {
        nrow = row;
        mcol = col;
    }

    /// Matrix-free matrix-vector multiplication.
    void Apply(const VEC &x, VEC &y) const override;
};

/// Assemble the right-hand side.
static void AssembleRHS(int dim, DBL *ptr)
{
    const DBL h    = 1.0 / dim; // mesh size
    const DBL h2   = h * h;     // mesh size squared
    const int dim1 = dim - 1;   // DOF in each direction

    for (INT j = 0; j < dim1 * dim1; ++j) ptr[j] = 0.0;

    // interior points
    for (int k = 2; k < dim1; ++k)
        for (int j = 2; j < dim1; ++j) ptr[locate(k, j)] = h2 * Load(k * h, j * h);

    // left boundary
    for (int j = 2; j < dim1; ++j)
        ptr[locate(1, j)] = LeftBdyCond(0, j * h) + h2 * Load(h, j * h);

    // right boundary
    for (int j = 2; j < dim1; ++j)
        ptr[locate(dim - 1, j)] = RightBdyCond(1.0, j * h) + h2 * Load(dim1 * h, j * h);

    // lower boundary
    for (int j = 2; j < dim1; ++j)
        ptr[locate(j, 1)] = LowerBdyCond(j * h, 0) + h2 * Load(j * h, h);

    // upper boundary
    for (int j = 2; j < dim1; ++j)
        ptr[locate(j, dim1)] = UpperBdyCond(j * h, 1.0) + h2 * Load(j * h, dim1 * h);

    // left lower corner
    ptr[locate(1, 1)] = LeftBdyCond(0, h) + LowerBdyCond(h, 0) + h2 * Load(h, h);

    // left upper corner
    ptr[locate(1, dim1)] =
        LeftBdyCond(0, h * dim1) + UpperBdyCond(h, 1.0) + h2 * Load(h, dim1 * h);

    // right lower corner
    ptr[locate(dim1, 1)] =
        LowerBdyCond(dim1 * h, 0) + RightBdyCond(1.0, h) + h2 * Load(dim1 * h, h);

    // right upper corner
    ptr[locate(dim1, dim1)] = UpperBdyCond(dim1 * h, 1.0) +
                              RightBdyCond(1.0, dim1 * h) +
                              h2 * Load(dim1 * h, dim1 * h);
}

/// Matrix-free action on a VEC object.
void MatFree::Apply(const VEC &x, VEC &y) const
{
    const DBL *x_val;
    x.GetArray(&x_val);

    y.SetValues(x.GetSize(), 0.0);
    DBL *y_val;
    y.GetArray(&y_val);

    const int dim1 = dim - 1;
    int       lower, left, center, right, upper;

    // interior points
    for (int k = 2; k < dim - 1; ++k) {
        for (int j = 2; j < dim - 1; ++j) {
            center        = (k - 1) * dim1 + j - 1;
            lower         = center - dim1;
            left          = center - 1;
            right         = center + 1;
            upper         = center + dim1;
            y_val[center] = -x_val[lower] - x_val[left] + 4.0 * x_val[center] -
                            x_val[right] - x_val[upper];
        }
    }

    // lower boundary
    for (int j = 2; j < dim - 1; ++j) {
        center = j - 1;
        left   = center - 1;
        right  = center + 1;
        upper  = center + dim1;
        y_val[center] =
            -x_val[left] + 4.0 * x_val[center] - x_val[right] - x_val[upper];
    }

    // upper boundary
    for (int j = 2; j < dim - 1; ++j) {
        center = (dim - 2) * dim1 + j - 1;
        lower  = center - dim1;
        left   = center - 1;
        right  = center + 1;
        y_val[center] =
            -x_val[lower] - x_val[left] + 4.0 * x_val[center] - x_val[right];
    }

    // left boundary
    for (int k = 2; k < dim - 1; ++k) {
        center = (k - 1) * dim1;
        lower  = center - dim1;
        right  = center + 1;
        upper  = center + dim1;
        y_val[center] =
            -x_val[lower] + 4.0 * x_val[center] - x_val[right] - x_val[upper];
    }

    // right boundary
    for (int k = 2; k < dim - 1; ++k) {
        center = (k - 1) * dim1 + dim - 2;
        lower  = center - dim1;
        left   = center - 1;
        upper  = center + dim1;
        y_val[center] =
            -x_val[lower] - x_val[left] + 4.0 * x_val[center] - x_val[upper];
    }

    // left lower corner
    center        = locate(1, 1);
    right         = locate(1, 2);
    upper         = locate(2, 1);
    y_val[center] = 4.0 * x_val[center] - x_val[right] - x_val[upper];

    // left upper corner
    left          = locate(1, dim - 2);
    center        = locate(1, dim - 1);
    right         = locate(2, dim - 1);
    y_val[center] = -x_val[left] + 4.0 * x_val[center] - x_val[right];

    // right lower corner
    lower         = locate(dim - 2, 1);
    center        = locate(dim - 1, 1);
    right         = locate(dim - 1, 2);
    y_val[center] = -x_val[lower] + 4.0 * x_val[center] - x_val[right];

    // right upper corner
    lower         = locate(dim - 2, dim - 1);
    left          = locate(dim - 1, dim - 2);
    center        = locate(dim - 1, dim - 1);
    y_val[center] = -x_val[lower] - x_val[left] + 4.0 * x_val[center];
}

int main(int argc, char *args[])
{
    const int   numTotalMesh = 7; // number of meshes in total
    GetWallTime timer;
    DBL *       ptr           = nullptr;
    bool        markAllocDone = true;
    VEC         b, x;
    DBL         norm2, norm2Old = 1.0;

    for (int mesh = 1; mesh < numTotalMesh; mesh++) {
        std::cout << "dim = " << dim << ", "
                  << "(dim-1)X(dim-1) : " << (dim - 1) * (dim - 1) << std::endl;

        DBL h = 1.0 / dim; // mesh size in X and Y directions

        // Allocate for temp memory space
        delete[] ptr;
        try {
            ptr = new DBL[(dim - 1) * (dim - 1)];
        } catch (std::bad_alloc &ex) {
            markAllocDone = false;
            std::cout << "Bad allocation" << std::endl;
            break;
        }

        // Assemble RHS
        AssembleRHS(dim, ptr);

        // Create b and x and assign values to them
        b.SetValues((dim - 1) * (dim - 1), ptr);
        x.SetValues((dim - 1) * (dim - 1), 0.25);

        // Create free-matrix object
        MatFree matfree((dim - 1) * (dim - 1), (dim - 1) * (dim - 1));

        // Setup preconditioner parameters
        Identity pc; // pc = identity, no preconditioning used

        // Setup solver parameters
        class CG cg;
        cg.SetOutput(PRINT_NONE);
        cg.SetMaxIter(10000);
        cg.SetRestart(20);
        cg.SetAbsTol(1e-12);
        cg.SetRelTol(1e-8);
        cg.SetPC(pc);
        cg.Setup(matfree);

        // Call CG method to solve Ax=b
        timer.Start();
        cg.Solve(b, x);
        cg.PrintTime(timer.Stop());

        std::cout << std::scientific << std::setprecision(4)
                  << "NumIter : " << cg.GetIterations() << std::endl
                  << "Norm2   : " << cg.GetNorm2() << std::endl
                  << "NormInf : " << cg.GetInfNorm() << std::endl;

        // Compute l2-norm between numerical solution and continuous solution
        norm2 = 0.0;
        for (int j = 1; j <= dim - 1; ++j) {
            for (int k = 1; k <= dim - 1; ++k) {
                norm2 += pow(fabs(x[locate(j, k)] - ExactSolu(j * h, k * h)), 2.0);
            }
        }
        norm2 *= h * h; // Apply simple quadrature rule to compute L2-norm!

        std::cout << "Error in L2-norm : " << std::scientific << std::setprecision(4)
                  << sqrt(norm2) << std::endl;
        if (mesh > 1)
            std::cout << "Convergence rate : " << std::fixed << std::setprecision(3)
                      << log(sqrt(norm2Old) / sqrt(norm2)) / log(2) << std::endl;
        std::cout << std::endl;

        // Refine the mesh and continue
        norm2Old = norm2; // store the previous error in L2-norm
        dim *= 2;         // refine the grid
    }

    if (markAllocDone) delete[] ptr;

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/