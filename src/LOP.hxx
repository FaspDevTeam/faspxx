/** \file    LOP.hxx
 *  \brief   Linear operator class declaration
 *  \author  Chensong Zhang
 *  \date    Sep/27/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __LOP_HEADER__  /*-- allow multiple inclusions --*/
#define __LOP_HEADER__  /**< indicate LOP.hxx has been included before */

#include <vector>
#include "faspxx.hxx"
#include "VEC.hxx"

/*! \class LOP
 *  \brief Linear operator class from VEC(ncol) to VEC(nrow)
 *
 *  This is a virtual class for all linear operators, for example a MAT sparse
 *  matrix or a matrix-free operation.
 */
class LOP {

protected:

    INT nrow;   ///< number of rows
    INT ncol;   ///< number of columns

public:

    /// Default constructor
    LOP() : nrow(0), ncol(0) {};

    /// Assign nrow and ncol to *this
    LOP(const INT& nrow, const INT& ncol);

    /// Assign nrow and ncol=nrow to *this
    LOP(const INT& nrow);

    /// Assign LOP object to *this
    LOP(const LOP& lop);

    /// Overload = operator
    const LOP &operator=(const LOP& lop);

    /// Default destructor
    ~LOP() = default;

    /// Get row space dimension
    INT GetRowSize() const;

    /// Get column space dimension
    INT GetColSize() const;

    /// Action of the linear operator to a vector
    virtual void Apply(const VEC& x, VEC& y) const {throw("Action not defined!"); };
};

#endif /* end if for __LOP_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/