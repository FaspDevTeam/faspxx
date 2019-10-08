/** \file LOP.hxx
 *  \brief Linear operator class declaration
 *  \author Chensong Zhang
 *  \date Sep/27/2019
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
 */
class LOP {

private:

    INT nrow;   ///< number of rows
    INT ncol;   ///< number of columns

public:

    /// Default constructor
    LOP() : nrow(0), ncol(0) {};

    /// Assign nrow, ncol to *this
    LOP(const INT& nrow, const INT& ncol);

    /// Assign nrow, ncol=nrow to *this
    LOP(const INT& nrow);

    /// Assign LOP object to *this
    LOP(const LOP& lop);

    /// Overload = operator
    LOP& operator=(const LOP& lop);

    /// Default destructor
    ~LOP() = default;

    /// Get row space dimension
    INT GetRowSize() const;

    /// Get column space dimension
    INT GetColSize() const;

    /// Get (*this)[i][j]
     virtual DBL GetValue(const INT& row, const INT& col) const = 0;

    /// Get the whole row-th row in *this into VEC object
    virtual void GetRow(const INT& row, std::vector<DBL>& v) const = 0;

    /// Get the whole col-th column in *this into VEC object
    virtual void GetCol(const INT& col, std::vector<DBL>& v) const = 0;

    /// Get the whole diagonal entries in *this into VEC object
    virtual void GetDiag(std::vector<DBL>& v) const = 0;

    /// Copy *this to lop
    virtual void CopyTo(LOP& lop) const;

    /// Transpose *this // Todo: Check???
    virtual void Transpose();

    /// Return VEC = *this * v
    virtual VEC MultVec(const VEC& v) const;
};

#endif /* end if for __LOP_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/