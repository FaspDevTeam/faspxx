/*! \file    MG.hxx
 *  \brief   Abstract MG class declaration
 *  \author  Chensong Zhang
 *  \date    Sep/12/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __MG_HEADER__ /*-- allow multiple inclusions --*/
#define __MG_HEADER__ /**< indicate MG.hxx has been included before */

// FASPXX header files
#include "ErrorLog.hxx"
#include "Iter.hxx"
#include "LOP.hxx"
#include "SOL.hxx"

using std::vector;

/*! \struct HL
 *  \brief  Hierarchical information for one level.
 */
template <class TTT>
struct HL {
    TTT* restriction;  ///< restriction to a coarser level
    TTT* prolongation; ///< prolongation from a coarser level

    SOL* preSolver;    ///< pre-smoother before CGC
    SOL* coarseSolver; ///< coarse solver for CGC
    SOL* postSolver;   ///< post-smoother after CGC

    INT fineSpaceSize; ///< fine-space dimension
    INT coarSpaceSize; ///< coarse-space dimension
    VEC b;             ///< right-hand side vector b at coarser level
    VEC x;             ///< solution vector x at coarser level
    VEC work;          ///< solution vector x at coarser level
};

/*! \class MG
 *  \brief Multigrid method in an abstract setting.
 */
template <class TTT>
class MG : public SOL
{
private:
    int              level;           ///< currecnt level number
    unsigned         numLevelsMax;    ///< max number of levels
    unsigned         numLevelsCoarse; ///< number of coarse levels in use <= max_levels
    bool             useSymmOper;     ///< use symmetric operator
    vector<unsigned> numCycles;       ///< number of cycles for each coarse level
    VEC              work;            ///< work vector

public:
    vector<HL<TTT>> infoHL; ///< hierarichal info at all coarse levels

private:
    void        MGCycle(const VEC& b, VEC& x); ///< one multigrid cycle
    FaspRetCode SetupSimple(const TTT& A);     ///< simple test setup

public:
    /// Default constructor.
    MG()
        : level(-1)
        , numLevelsMax(20)
        , numLevelsCoarse(0)
        , useSymmOper(true){};

    /// Default destructor.
    ~MG() = default;

    /// Setup the MG method using coefficient matrix A.
    FaspRetCode Setup(const TTT& A);

    /// Solve Ax=b using the MG method.
    FaspRetCode Solve(const VEC& b, VEC& x) override;

    /// Clean up MG data allocated during Setup.
    void Clean() override;
};

#endif /* end if for __MG_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/12/2021      Create file                          */
/*  Chensong Zhang      Sep/27/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/