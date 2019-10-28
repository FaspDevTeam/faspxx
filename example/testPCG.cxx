/*! \file testPCG.hxx
 *  \brief Test performance of PCG method
 *  \author Kailei Zhang, Chensong Zhang
 *  \date Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include "Timing.hxx"
#include "Param.hxx"
#include "PCG.hxx"
#include "LOP.hxx"
#include "ReadCommand.hxx"
#include "ReadData.hxx"

// Define a simple scaling preconditioner (Jacobi)
// Todo: Move preconditioners to separate files
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
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    GetWallTime timer;
    timer.Start();

    // Get command line options
    InitParam init;
    if ( (retCode = ReadParam(argc, args, init)) < 0 ) return retCode;

    // Print data files used by PCG
    init.data.Print();

    // Read matrix data file
    MAT mat;
    if ( (retCode = ReadMat(init.data.GetMatName(), mat)) < 0 ) return retCode;
    INT row = mat.GetRowSize();
    INT col = mat.GetColSize();
    INT nnz = mat.GetNNZ();

    // Read or generate right-hand side
    VEC b, x;
    if (init.data.GetRhsName() != nullptr)
        ReadVEC(init.data.GetRhsName(), b);
    else
        b.SetValues(row, 0.0);

    // Read or generate initial guess
    if (init.data.GetLhsName() != nullptr)
        ReadVEC(init.data.GetLhsName(), x);
    else
        x.SetValues(col, 1.0);

    // Print problem size information
    std::cout << "  nrow = " << row
              << ", ncol = " << col
              << ", nnz = " << nnz << std::endl;
    std::cout << "Reading Ax = b costs " << timer.Stop() << "ms" << std::endl;

    // Print parameters used by PCG
    init.param.Print();

    // Setup PCG class
    PCG pcg;
    pcg.Setup(&mat, b, x, init.param);

    // Setup preconditioner
#if 1
    IdentityLOP lop(row);
    pcg.SetupPCD(&lop);
#else // Todo: Add choices for preconditioner
    std::vector<double> vt;
    mat.GetDiag(vt);
    Scaling jac(vt);
    pcg.SetupPCD(jac);
#endif

    // PCG solve
    timer.Start();
    retCode = pcg.Solve(&mat, b, x, init.param);
    std::cout << "Solving Ax=b costs " << timer.Stop() << "ms" << std::endl;

    // Clean up preconditioner and solver data
    pcg.CleanPCD();
    pcg.Clean();

    return retCode;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/