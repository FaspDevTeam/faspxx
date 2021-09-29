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

    USI fineSpaceSize; ///< fine-space dimension
    USI coarSpaceSize; ///< coarse-space dimension
    VEC b;             ///< right-hand side vector b at coarser level
    VEC x;             ///< solution vector x at coarser level
    VEC r;             ///< residual vector r at coarser level
};

/*! \class MG
 *  \brief Multigrid method in an abstract setting.
 */
template <class TTT>
class MG : public SOL
{
private:
    INT         level;           ///< currecnt level number
    USI         numLevelsMax;    ///< max number of levels
    USI         numLevelsCoarse; ///< number of coarse levels in use <= max_levels
    bool        useSymmOper;     ///< use symmetric operator
    vector<USI> numCycles;       ///< number of cycles for each coarse level
    VEC         r;               ///< work vector for current residual

public:
    vector<HL<TTT>> infoHL; ///< hierarichal info at all coarse levels

private:
    /// One multigrid V or W or variable cycle.
    void MGCycle(const VEC& b, VEC& x);

    /// One multigrid F cycle.
    // TODO: void FCycle(const VEC& b, VEC& x);

    /// One multigrid AMLI cycle.
    // TODO: void AMLICycle(const VEC& b, VEC& x);

    /// One multigrid K cycle.
    // TODO: void KCycle(const VEC& b, VEC& x);

public:
    /// Default constructor.
    MG()
        : level(-1)
        , numLevelsMax(20)
        , numLevelsCoarse(0)
        , useSymmOper(true)
    {
        SetNumCycles(1);
    };

    /// Default destructor.
    ~MG() = default;

    /// Set number of cycles for each coarse level.
    void SetNumCycles(USI ncycle);

    /// Setup the MG method using coefficient matrix A.
    FaspRetCode Setup(const TTT& A);

    /// Setup multilevel solver level by level.
    FaspRetCode SetupLevel(const TTT& A, const USI level, TTT* tranOpers,
                           SOL* smoothers, SOL* coarseSolvers);

    /// Setup multilevel solver by hand.
    FaspRetCode SetupALL(const TTT& A, const USI numLevels);

    /// Setup classical AMG.
    // TODO: FaspRetCode SetupCAMG(const MAT& A);

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
/*  Chensong Zhang      Sep/29/2021      Add hierarical info struct           */
/*----------------------------------------------------------------------------*/