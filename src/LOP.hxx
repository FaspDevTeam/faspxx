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
    USI nrow; ///< dimension of image space
    USI mcol; ///< dimension of origin space

public:
    /// Default constructor.
    explicit LOP()
        : nrow(0)
        , mcol(0){};

    /// Make an LOP from VEC(mcol) to VEC(nrow).
    explicit LOP(const USI& nrow, const USI& mcol);

    /// Make an LOP from VEC(nrow) to VEC(nrow).
    explicit LOP(const USI& nrow);

    /// Clone from another LOP.
    LOP(const LOP& lop);

    /// Overload the = operator.
    LOP& operator=(const LOP& lop);

    /// Default destructor.
    ~LOP() = default;

    /// Get row space dimension.
    USI GetRowSize() const;

    /// Get column space dimension.
    USI GetColSize() const;

    /// Action of the linear operator to a vector.
    virtual void Apply(const VEC& x, VEC& y) const
    {
        FASPXX_ABORT("Should be over-written!");
    };

    /// Compute residual of a linear operator equation.
    virtual void Residual(const VEC& b, const VEC& x, VEC& r) const
    {
        FASPXX_ABORT("Should be over-written!");
    };
};

/*! \class IdentityOp
 *  \brief Identity operator.
 */
class IdentityOp : public LOP
{

public:
    /// Default constructor
    IdentityOp() = default;

    /// Default destructor
    ~IdentityOp() = default;

    /// Iterator
    void Apply(const VEC& b, VEC& x) const override;
};

#endif /* end if for __LOP_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/27/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/