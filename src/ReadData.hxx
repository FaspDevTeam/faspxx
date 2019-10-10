/** \file ReadData.hxx
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

FaspRetCode ReadMTX(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowInd, std::vector<INT>& colInd,
                    std::vector<DBL>& values);

FaspRetCode ReadCSR(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowPtr, std::vector<INT>& colInd,
                    std::vector<DBL>& values);

#endif // __READ_FILE_HXX__

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/