/*! \file    Iter.cxx
 *  \brief   Simple iterative methods definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Dec/02/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "Iter.hxx"

/// Does nothing in preconditioning
FaspRetCode Identity::Solve(const VEC &x, VEC &y) {
    y = x;
    return FaspRetCode::SUCCESS;
}

/// Standard constructor.
Jacobi::Jacobi(const MAT &A, const VEC &b) {

    /* A = C + D + U
     * lTri = C
     * uTri = U
     * diagInv = D^{-1}
     * Compute :
     * tmpMat = D^{-1} * ( C + U )
     * tmpVec = D^{-1} * b
     */

    MAT diagInv, lTri, uTri;

    A.GetDiagInv(diagInv); // get the diagonal elements' inverse matrix
    A.GetLowerTri(lTri); // get the lower triangular matrix
    A.GetUpperTri(uTri); // get the upper triangular matrix

    diagInv.Apply(b, tmpVec); // compute tmpVec = D^{-1} * b

    /* tmpMat = D^{-1} * ( C + U ) */
    tmpMat.Add(1.0, lTri, 1.0, uTri);
    tmpMat.MultRight(diagInv);

    this->A = &A;
    this->tmpb = &b;

    // Allocate memory for temporary variables
    try {
        tmp.SetValues(b.GetSize(), 0.0);
        rk.SetValues(b.GetSize(), 0.0);
        xTmp.SetValues(b.GetSize(), 0.0);
    } catch (std::bad_alloc &ex) {
        throw ("Insufficient memory!");
    }

}

/// Solve Ax=b using the Jacobi method.
FaspRetCode Jacobi::Solve(const VEC &x, VEC &y) {
    // Check whether vector space sizes
    if (x.GetSize() != tmpMat.GetColSize() ||
        tmpVec.GetSize() != tmpMat.GetRowSize() ||
        tmpMat.GetRowSize() != tmpMat.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    double norm2;
    double relRes, absRes, denRes;

    // Initialize iterative method
    numIter = 0;
    xTmp = x;

    // rk = b = A * x
    this->A->Apply(xTmp, tmp);
    rk.WAXPBY(1.0, *tmpb, 1.0, tmp);

    // compute absolute residual
    absRes = rk.Norm2();
    denRes = (1e-20 > absRes) ? 1e-20 : absRes;
    relRes = absRes / denRes;

    // If initial residual is very small, no need to iterate
    if ( relRes < relTol || absRes < absTol ) goto FINISHED;

    // Main Jacobian loop
    while (numIter < maxIter) {
        numIter++; // iteration count

        // y = -1 * D^{-1} * ( C + U ) * x + D^{-1} * b
        tmpMat.Apply(xTmp, tmp);
        y.WAXPBY(-1.0, tmp, 1.0, tmpVec);

        // r = b - A * x
        this->A->Apply(y, tmp);
        rk.WAXPBY(1.0, *tmpb, -1.0, tmp);

        absRes = rk.Norm2();
        relRes = absRes / denRes;

        if (absRes < absTol || relRes < relTol)
            goto FINISHED;

        xTmp = y; // save value for the next iteration
    }

    FINISHED:
    if (numIter >= maxIter) {
        PrintFinal(std::cout);
        return FaspRetCode::ERROR_SOLVER_MAXIT;
    }

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/