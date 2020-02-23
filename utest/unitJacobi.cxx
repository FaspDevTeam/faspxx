/** \file    unitJacobi.cxx
 *  \brief   Unit test for Jacobi implementations
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Feb/22/2020
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include <vector>
#include "Param.hxx"
#include "LOP.hxx"
#include "Iter.hxx"

/*---------------------------------*/
/*--     Beginning of TEST       --*/
/*---------------------------------*/

TEST(WeightedJacobi, JacobiSolve2)
{
    const INT row = 2, col = 2, nnz = 4;

    // true solutions (from octave) : 0.19231 0.11538
    vector<DBL> xstar({0.19231, 0.11538});

    // coefficient matrix
    vector<DBL> value(4);
    value[0] = 4; value[1] = 2;
    value[2] = 1; value[3] = 7;

    vector<INT> colInd(4);
    colInd[0] = 0; colInd[1] = 1;
    colInd[2] = 0; colInd[3] = 1;

    vector<INT> rowPtr(3);
    rowPtr[0] = 0;
    rowPtr[1] = 2;
    rowPtr[2] = 4;

    vector<INT> diagPtr(2);
    diagPtr[0] = 0;
    diagPtr[1] = 3;

    MAT mat(row,col,nnz,value,colInd,rowPtr,diagPtr);

    // right-hand side
    vector<DBL> fval(2);
    fval[0] = 1;
    fval[1] = 1;
    VEC f(fval);

    VEC x(2, 0.0) ; // initialization

    class Jacobi Jsolve;
    Jsolve.SetRelTol(1e-9);

    // Jacobi iteration 1
    Jsolve.SetWeight(1.0);
    Jsolve.Setup(mat);
    Jsolve.Solve(f,x);

    // check solution
    for ( INT i = 0; i < row; i++ ) EXPECT_NEAR(x[i], xstar[i], 1e-5);

    // Jacobi iteration 2
    for ( INT i = 0; i < row; i++ ) x[i] = 0.0; // init to zero again
    Jsolve.SetWeight(0.5);
    Jsolve.Setup(mat);
    Jsolve.Solve(f,x);

    // check solution
    for ( INT i = 0; i < row; i++ ) EXPECT_NEAR(x[i], xstar[i], 1e-5);
}

TEST(WeightedJacobi, JacobiSolve3)
{
    const INT row = 3, col = 3, nnz = 9;

    // true solutions (from octave) : 0.221719 0.027149 0.058824
    vector<DBL> xstar({0.221719, 0.027149, 0.058824});

    // coefficient matrix
    vector<DBL> value(9);
    value[0] = 4; value[1] = 2; value[2] = 1;
    value[3] = 3; value[4] = 8; value[5] = 2;
    value[6] = 2; value[7] = 1; value[8] = 9;

    vector<INT> colInd(9);
    colInd[0] = 0; colInd[1] = 1; colInd[2] = 2;
    colInd[3] = 0; colInd[4] = 1; colInd[5] = 2;
    colInd[6] = 0; colInd[7] = 1; colInd[8] = 2;

    vector<INT> rowPtr(4);
    rowPtr[0] = 0;
    rowPtr[1] = 3;
    rowPtr[2] = 6;
    rowPtr[3] = 9;
    
    vector<INT> diagPtr(3);
    diagPtr[0] = 0;
    diagPtr[1] = 4;
    diagPtr[2] = 8;

    MAT mat(row, col, nnz, value, colInd, rowPtr, diagPtr);

    // right-hand side
    vector<DBL> fval(3);
    fval[0] = 1;
    fval[1] = 1;
    fval[2] = 1;

    VEC f(fval);
    
    VEC x(3, 0.0) ; // initialization

    class Jacobi Jsolve;
    Jsolve.SetRelTol(1e-9);

    // Jacobi iteration 1
    Jsolve.SetWeight(1.0);
    Jsolve.Setup(mat);
    Jsolve.Solve(f,x);

    for ( INT i = 0; i < row; i++ ) EXPECT_NEAR(x[i], xstar[i], 1e-5);

    // Jacobi iteration 2
    for ( INT i = 0; i < row; i++ ) x[i] = 0.0;
    Jsolve.SetWeight(0.5);
    Jsolve.Setup(mat);
    Jsolve.Solve(f,x);

    for ( INT i = 0; i < row; i++ ) EXPECT_NEAR(x[i], xstar[i], 1e-5);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/