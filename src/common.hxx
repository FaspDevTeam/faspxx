/*! \file common.hxx
 *  \brief Header file for some FASP++ Matrix functions
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __COMMON_HEADER__      /*-- allow multiple inclusions --*/
#define __COMMON_HEADER__      /**< indicate mat.hxx has been included before */


#include "mat.hxx"

//! check whether the input data are CSRx
INT CheckCSRx(const INT row, const INT column, const INT nnz,
              const std::vector<DBL> values,
              const std::vector<INT> rowshift,
              const std::vector<INT> colindex,
              const std::vector<INT> diag);

//! check whether the input data are CSR
INT CheckCSR(const INT row, const INT column, const INT nnz,
             const std::vector<DBL> values,
             const std::vector<INT> rowshift,
             const std::vector<INT> colindex);

#endif /* end if for __COMMON_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/