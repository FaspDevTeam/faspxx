/*! \file    testInverse.cxx
 *  \brief   Test inversion of a matrix
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Standard header files
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

// FASPXX header files
#include "MAT.hxx"

int main()
{
    INT         row1 = 4, col1 = 4, nnz1 = 10;
    vector<INT> colInd1(10), rowPtr1(5), diag1(4);
    vector<DBL> values1(10);

    values1[0] = 0;
    values1[1] = 1;
    values1[2] = 2;
    values1[3] = 3;
    values1[4] = 4;
    values1[5] = 5;
    values1[6] = 2;
    values1[7] = 1;
    values1[8] = 2;
    values1[9] = 1;

    colInd1[0] = 0;
    colInd1[1] = 2;
    colInd1[2] = 0;
    colInd1[3] = 1;
    colInd1[4] = 3;
    colInd1[5] = 0;
    colInd1[6] = 2;
    colInd1[7] = 3;
    colInd1[8] = 2;
    colInd1[9] = 3;

    rowPtr1[0] = 0;
    rowPtr1[1] = 2;
    rowPtr1[2] = 5;
    rowPtr1[3] = 8;
    rowPtr1[4] = 10;

    diag1[0] = 0;
    diag1[1] = 3;
    diag1[2] = 6;
    diag1[3] = 9;

    MAT A(row1, col1, nnz1, values1, colInd1, rowPtr1, diag1);
    MAT B;

    A.Inverse(B);
    cout << "initial matrix : A" << endl;
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) cout << A.GetValue(j, k) << "  ";
        cout << endl;
    }
    cout << "the inverse of A :" << endl;
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) cout << B.GetValue(j, k) << "  ";
        cout << endl;
    }

    vector<DBL> valuesInv1 = {2.0, 2.0};

    vector<INT> colIndInv  = {0, 1};
    vector<INT> rowPtrInv  = {0, 1, 2};
    vector<INT> diagPtrInv = {0, 1, 2};

    MAT C(2, 2, 2, valuesInv1, colIndInv, rowPtrInv, diagPtrInv);
    MAT D;

    C.Inverse(D);
    cout << "initial matrix : C" << endl;
    for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) cout << C.GetValue(j, k) << "  ";
        cout << endl;
    }

    cout << "the inverse of C :" << endl;
    for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) cout << D.GetValue(j, k) << "  ";
        cout << endl;
    }

    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/