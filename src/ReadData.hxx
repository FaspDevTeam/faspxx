/** \file ReadData.hxx
 *  \brief Reading data from disk files
 *  \author Chensong Zhang, Kailei Zhang
 *  \date Oct/11/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __READ_DATA_HXX__
#define __READ_DATA_HXX__

#include "faspxx.hxx"
#include "MAT.hxx"

/// \brief Read a VEC data file
FaspRetCode ReadVEC(const char* filename, VEC& rhs);

/// \brief Read an MTX data file and store it in (rowInd, colInd, values)
FaspRetCode ReadMTX(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowInd, std::vector<INT>& colInd,
                    std::vector<DBL>& values);

/// \brief Read a CSR data file and store it in (rowPtr, colInd, values)
FaspRetCode ReadCSR(const char* filename, INT& row, INT& col, INT& nnz,
                    std::vector<INT>& rowPtr, std::vector<INT>& colInd,
                    std::vector<DBL>& values);

/// \brief Read an matrix data file and store it in MAT
FaspRetCode ReadMat(const char *filename, MAT& mat);

#endif // __READ_DATA_HXX__

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/