/**\file ReadData.hxx
 *  \brief Reading data from disk files
 *  \author Kailei Zhang
 *  \date Sep/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __READ_FILE_HXX__
#define __READ_FILE_HXX__

#include "faspxx.hxx"
#include "MAT.hxx"

FaspRetCode ReadMtx(const char *filename, INT &row, INT &col, INT &nnz,
                    std::vector<INT> &rowInd, std::vector<INT> &colInd,
                    std::vector<DBL> &values);

FaspRetCode MtxToMAT(const INT row, const INT col, const INT nnz,
                     const std::vector<INT> rowInd, const std::vector<INT> colInd,
                     const std::vector<DBL> values, MAT &mat);

#endif // __READ_FILE_HXX__

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/