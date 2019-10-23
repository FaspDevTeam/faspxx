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
    LOP& operator=(const LOP& lop);

    /// Default destructor
    ~LOP() = default;


public:
    //fff如果用户只给一个算子的作用方式,那么下面的大多数member function都不应该有,
    // 比如GetRow(),GetCol(),因为我们没法得到这些值,或者得到它们的代价比较高.
    // 而这些member function应该只存在于MAT之类的比较具体的LOP子类.
    // 还有一种方式是在这个LOP里面也定义这些member function, 但是都把它们定义成virtual(不能是pure virtual),类似下面的Transpose()

    /// Get row space dimension
    INT GetRowSize() const;

    /// Get column space dimension
    INT GetColSize() const;

    /// Get (*this)[i][j]
     virtual DBL GetValue(const INT& row, const INT& col) const;

    /// Get the whole row-th row in *this into VEC object
    virtual void GetRow(const INT& row, std::vector<DBL>& v) const;

    /// Get the whole col-th column in *this into VEC object
    virtual void GetCol(const INT& col, std::vector<DBL>& v) const;

    /// Get the whole diagonal entries in *this into VEC object
    virtual void GetDiag(std::vector<DBL>& v) const;

    /// Copy *this to lop
    virtual void CopyTo(LOP& lop) const;

    /// Transpose *this // Todo: Check???
    virtual void Transpose()
    { throw "Not supported!"; } //fff我们应该定义一个抛出异常或者debug信息的函数,比如fasp_error(),来替换这里的throw

    /// Return VEC = *this * v fff名字是用Apply好还是Mult
    virtual void Apply(const VEC& v,VEC &vt) const = 0; //fff:把它变成纯虚函数,整个LOP都是一个抽象类,不能定义对象
};


// Identity linear operator
class IdentityLOP: public LOP
{
public:
    explicit IdentityLOP(INT n): LOP(n, n) {}

    virtual void Apply(const VEC& x,VEC& y) const { y = x; }
};



#endif /* end if for __LOP_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/