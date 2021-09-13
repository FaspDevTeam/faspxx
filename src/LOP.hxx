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

#ifndef __LOP_HEADER__ /*-- allow multiple inclusions --*/
#define __LOP_HEADER__ /**< indicate LOP.hxx has been included before */

// FASPXX header files
#include "ErrorLog.hxx"
#include "VEC.hxx"

/*! \class LOP
 *  \brief Linear operator virtual class.
 */
class LOP
{

protected:
    INT nrow; ///< number of rows
    INT mcol; ///< number of columns

public:
    /// Default constructor.
    LOP()
        : nrow(0)
        , mcol(0){};

    /// Make an LOP from VEC(mcol) to VEC(nrow).
    LOP(const INT &nrow, const INT &mcol);

    /// Make an LOP from VEC(nrow) to VEC(nrow).
    explicit LOP(const INT &nrow);

    /// Make an LOP from another LOP.
    LOP(const LOP &lop);

    /// Overload the = operator.
    LOP &operator=(const LOP &lop);

    /// Default destructor.
    ~LOP() = default;

    /// Get row space dimension.
    INT GetRowSize() const;

    /// Get column space dimension.
    INT GetColSize() const;

    /// Action of the linear operator to a vector.
    virtual void Apply(const VEC &x, VEC &y) const
    {
        FASPXX_ABORT("Should be over-written!");
    };

    /// Compute residual of a linear operator equation.
    virtual void Residual(const VEC &b, const VEC &x, VEC &r) const
    {
        FASPXX_ABORT("Should be over-written!");
    };
};

/*! \class IdentityOper
 *  \brief Identity operator.
 */
class IdentityOper : public LOP
{

public:
    /// Default constructor
    IdentityOper() = default;

    /// Default destructor
    ~IdentityOper() = default;

    /// Iterator
    void Apply(const VEC &b, VEC &x) const override;
};

#endif /* end if for __LOP_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/