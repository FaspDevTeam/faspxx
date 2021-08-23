/*! \file    testWeightJacobi.cxx
 *  \brief   Test performance of Weighted Jacobi method
 *  \author  Kailei Zhang
 *  \date    Dec/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "Timing.hxx"
#include "ReadData.hxx"
#include "Param.hxx"
#include "LOP.hxx"
#include "Iter.hxx"

using std::cout;
using std::endl;
using std::vector;

int main(){
    // matrix 1
    INT row1=2;
    INT col1=2;
    INT nnz1=4;
    vector<DBL> value1(4);
    value1[0]=4;
    value1[1]=2;
    value1[2]=1;
    value1[3]=7;
    vector<INT> colInd1(4);
    colInd1[0]=0;
    colInd1[1]=1;
    colInd1[2]=0;
    colInd1[3]=1;
    vector<INT> rowPtr1(3);
    rowPtr1[0]=0;
    rowPtr1[1]=2;
    rowPtr1[2]=4;
    vector<INT> diagPtr1(2);
    diagPtr1[0]=0;
    diagPtr1[1]=3;

    MAT mat1(row1,col1,nnz1,value1,colInd1,rowPtr1,diagPtr1);

    //matrix 2
    INT row2=3;
    INT col2=3;
    INT nnz2=9;

    vector<DBL> value2(9);
    value2[0]=4;
    value2[1]=2;
    value2[2]=1;
    value2[3]=3;
    value2[4]=8;
    value2[5]=2;
    value2[6]=2;
    value2[7]=1;
    value2[8]=9;
    vector<INT> colInd2(9);
    colInd2[0]=0;
    colInd2[1]=1;
    colInd2[2]=2;
    colInd2[3]=0;
    colInd2[4]=1;
    colInd2[5]=2;
    colInd2[6]=0;
    colInd2[7]=1;
    colInd2[8]=2;
    vector<INT> rowPtr2(4);
    rowPtr2[0]=0;
    rowPtr2[1]=3;
    rowPtr2[2]=6;
    rowPtr2[3]=9;
    vector<INT> diagPtr2(3);
    diagPtr2[0]=0;
    diagPtr2[1]=4;
    diagPtr2[2]=8;

    MAT mat2(row2,col2,nnz2,value2,colInd2,rowPtr2,diagPtr2);

    vector<DBL> val1(2);
    val1[0]=1;
    val1[1]=1;

    VEC vec1(val1);

    vector<DBL> val2(3);
    val2[0]=1;
    val2[1]=1;
    val2[2]=1;

    VEC vec2(val2);

    VEC x1(2);
    VEC x2(3);

    DBL wei1=0.5;
    DBL wei2=0.5;

    class Jacobi j1;
    j1.SetOutput(Output::PRINT_SOME);
    j1.SetRelTol(1e-9);
    j1.SetWeight(wei1);
    j1.Setup(mat1);
    j1.Solve(vec1,x1);

    /*
     * true solutions (from octave): 0.19231 0.11538
     */
    cout << "x1 : " << x1[0] << ", " << x1[1] << endl;

    class Jacobi j2;
    j2.SetOutput(Output::PRINT_SOME);
    j2.SetRelTol(1e-9);
    j2.SetWeight(wei2);
    j2.Setup(mat2);
    j2.Solve(vec2,x2);

    /**
     * true solutions (from octave) : 0.221719 0.027149 0.058824
     */
    cout << "x2 : " << x2[0] << ", " << x2[1] << ", " << x2[2] <<endl;

    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/