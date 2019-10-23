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

protected:

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
    const LOP& operator= (const LOP& lop);

    /// Default destructor
    ~LOP() = default;

    /// Get row space dimension
    INT GetRowSize() const;

    /// Get column space dimension
    INT GetColSize() const;

    /// Return VEC = *this * v
    virtual void Apply(const VEC& x, VEC& y) const = 0; //fff:把它变成纯虚函数,整个LOP都是一个抽象类,不能定义对象

};


// Identity linear operator
class IdentityLOP: public LOP
{
public:
    explicit IdentityLOP(INT n): LOP(n, n) {}

    virtual void Apply(const VEC& x, VEC& y) const { y = x; }
};



#endif /* end if for __LOP_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/