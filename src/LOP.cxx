/** \file    LOP.cxx
 *  \brief   Linear operator class definition
 *  \author  Chensong Zhang, Kailei Zhang
 *  \date    Oct/27/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// FASPXX header files
#include "LOP.hxx"

/// Assign nrow, mcol to *this.
LOP::LOP(const USI& nrow, const USI& mcol)
{
    this->nrow = nrow;
    this->mcol = mcol;
}

/// Assign nrow, mcol=nrow to *this.
LOP::LOP(const USI& nrow)
{
    this->nrow = nrow;
    this->mcol = nrow;
}

/// Assign LOP object to *this.
LOP::LOP(const LOP& lop)
{
    this->nrow = lop.nrow;
    this->mcol = lop.mcol;
}

/// Assignment for the LOP object.
LOP& LOP::operator=(const LOP& lop)
{
    if (this == &lop) return *this; // self-assignment
    this->nrow = lop.nrow;
    this->mcol = lop.mcol;
    return *this;
}

/// Dimension of the row space of LOP.
USI LOP::GetRowSize() const { return this->nrow; }

/// Dimension of the column space of LOP.
USI LOP::GetColSize() const { return this->mcol; }

/// Identity operator.
void IdentityOp::Apply(const VEC& b, VEC& x) const { x = b; }

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Oct/27/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/