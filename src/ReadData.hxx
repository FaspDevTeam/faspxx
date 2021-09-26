/** \file    ReadData.hxx
 *  \brief   Reading data from disk files
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Oct/11/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __READDATA__HEADER__ /*-- allow multiple inclusions --*/
#define __READDATA__HEADER__ /**< indicate ReadData.hxx has been included before */

// FASPXX header files
#include "Faspxx.hxx"
#include "MAT.hxx"

/*
 *  This class defines the basic MAT data structure and its basic operations. The
 *  CSRx data structure is an extension of the wellknown CSR sparse matrix format.
 *  The differences lie in the following two aspects:
 *      (1) Unlike the classical CSR format, the CSRx format requires the column
 *          indices in each row are in ascending order;
 *      (2) The CSRx format has a diagPtr array which stores the locations of the
 *          diagonal entries in each row.
 *
 *  We give two simple examples here:
 *
 *   Example 1.
 *  \begin{pmatrix}
 *      1  &  0  &  2 \\
 *      0  &  1  &  0 \\
 *      3  &  0  &  0 \\
 *  \end{pmatrix}
 *  and
 *      nnz = 5, row = 3, col = 3,
 *      values = { 1, 2, 1, 3, 0 },
 *      colInd = { 0, 2, 1, 0, 2 },
 *      rowPtr = { 0, 2, 3, 5 },
 *      diagPtr = { 0, 2, 4 }.
 *
 *  Example 2.
 *  \begin{pmatrix}
 *      1&  7&  2 \\
 *      0&  0&  0 \\
 *      3&  0&  4 \\
 *  \end{pmatrix}
 *  and
 *      nnz = 6, row = 3, col = 3,
 *      values = { 1, 7, 2, 0, 3, 4 },
 *      colInd = { 0, 1, 2, 1, 0, 2 },
 *      rowPtr = { 0, 3, 4, 6 },
 *      diagPtr = { 0, 3, 5 }.
 *
 *  Note that the CSRx format stores the diagonal entries even if they are zero.
 *  Furthermore, it is compatible with all CSR subroutines!
 */

// Definition of file format
const unsigned FILE_CSR = 1; ///< CSR file format
const unsigned FILE_MTX = 2; ///< MTX file format

/// Read a VEC data file and store it in dst
FaspRetCode ReadVEC(const char* filename, VEC& dst);

/// Read an MTX data file and store it in (rowInd, colInd, values)
FaspRetCode ReadMTX(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowInd, std::vector<INT>& colInd,
                    std::vector<DBL>& values);

/// Read a CSR data file and store it in (rowPtr, colInd, values)
FaspRetCode ReadCSR(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowPtr, std::vector<INT>& colInd,
                    std::vector<DBL>& values);

/// Read a MAT data file and store it in MAT
FaspRetCode ReadMat(const char* filename, MAT& dst);

#endif /* end if for __READDATA__HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Oct/11/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/