/** \file    MATUtil.hxx
 *  \brief   Tools for checking and manipulating MAT
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Sep/26/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __MATUTIL_HXX__ /*-- allow multiple inclusions --*/
#define __MATUTIL_HXX__ /**< indicate MATUtil.hxx has been included before */

// FASPXX header files
#include "MAT.hxx"

/// Check whether two matrices have same sizes for addition
FaspRetCode CheckMATAddSize(const MAT& mat1, const MAT& mat2);

/// Check MAT-MAT multiplication sizes
FaspRetCode CheckMATMultSize(const MAT& mat1, const MAT& mat2);

/// Check whether (row,col) is out of bound
FaspRetCode CheckMATSize(const MAT& mat, const USI& row, const USI& col);

/// Check whether (row,:) is out of bound
FaspRetCode CheckMATRowSize(const MAT& mat, const USI& row);

/// Check whether (:,col) is out of bound
FaspRetCode CheckMATColSize(const MAT& mat, const USI& col);

/// Check MAT-VEC multiplication sizes
FaspRetCode CheckMATVECSize(const MAT& mat, const VEC& vec);

/// Check whether the data is good for CSR
FaspRetCode CheckCSR(const USI& row, const USI& col, const USI& nnz,
                     const std::vector<DBL>& values, const std::vector<USI>& colInd,
                     const std::vector<USI>& rowPtr);

/// Check whether the data is good for CSRx
FaspRetCode CheckCSRx(const USI& row, const USI& col, const USI& nnz,
                      const std::vector<DBL>& values, const std::vector<USI>& colInd,
                      const std::vector<USI>& rowPtr, const std::vector<USI>& diagPtr);

/// Convert a CSR matrix to MAT (private)
FaspRetCode CSRtoMAT(const USI& row, const USI& col, const USI& nnz,
                     const std::vector<DBL>& values, const std::vector<USI>& colInd,
                     const std::vector<USI>& rowPtr, MAT& mat);

/// Convert MTX data to MAT data structure
FaspRetCode MTXtoMAT(const USI& row, const USI& col, const USI& nnz,
                     const std::vector<USI>& rowInd, const std::vector<USI>& colInd,
                     const std::vector<DBL>& values, MAT& mat);

/// Sort "colInd" of each row in ascending order and rearrange "values" accordingly
FaspRetCode SortCSRRow(const USI& row, const USI& col, const USI& nnz,
                       const std::vector<USI>& rowPtr, std::vector<USI>& colInd,
                       std::vector<DBL>& values);

#endif /* end if for __MATUTIL_HXX__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Sep/26/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/