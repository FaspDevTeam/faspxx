/** \file    testReadData.cxx
 *  \brief   This example shows how to read data from disk files
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "MATUtil.hxx"
#include "ReadData.hxx"
#include "RetCode.hxx"
#include "VECUtil.hxx"

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main()
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw

    // Read a MTX matrix from file
    INT              row, col, nnz;
    std::vector<INT> rowInd; // row index i
    std::vector<INT> colInd; // col index j
    std::vector<DBL> values; // entry(i,j)

    // There are two test matrix nos7.mtx and nos7-.mtx. The difference is nos7-.mtx
    // is modified from nos7.mtx by set the first row as zero. So they should give
    // exactly same output.
    try {
        retCode = ReadMTX("../data/nos7.mtx", row, col, nnz, rowInd, colInd, values);
        if (retCode < 0) throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
    } catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Convert a MTX matrix to MAT
    MAT mat;
    try {
        retCode = MTXtoMAT(row, col, nnz, rowInd, colInd, values, mat);
        if (retCode < 0) throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
    } catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }
    std::cout << "  nrow = " << mat.GetRowSize() << ", ncol = " << mat.GetColSize()
              << std::endl;

    // Make a test VEC = (1.0, 1.0, ..., 1.0)'
    VEC v(col, 1.0), w(col);
    // Check a specific entry of VEC
    try {
        retCode = CheckVECSize(v, 700);
        if (retCode < 0)
            throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        else
            std::cout << "  v[700] = " << v.GetValue(700) << std::endl;
    } catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Check dimensional of MAT and VEC
    try {
        retCode = CheckMATVECSize(mat, v);
        if (retCode < 0)
            throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        else
            mat.Apply(v, w); // size matches, compute w = mat * v
    } catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    // Check a specific entry of VEC
    try {
        retCode = CheckVECSize(w, 700);
        if (retCode < 0)
            throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        else
            std::cout << "  w[700] = " << w.GetValue(700) << std::endl;
    } catch (FaspRunTime &ex) {
        ex.LogExcep();
        return ex.errorCode;
    }

    return retCode;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Oct/12/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/