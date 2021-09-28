/*! \file    AMG.hxx
 *  \brief   Algebraic MG class declaration
 *  \author  Chensong Zhang
 *  \date    Sep/12/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __AMG_HEADER__ /*-- allow multiple inclusions --*/
#define __AMG_HEADER__ /**< indicate MG.hxx has been included before */

// FASPXX header files
#include "Iter.hxx"
#include "MG.hxx"

using std::vector;

/*! \class AMG
 *  \brief Algebraic multigrid method.
 */
class AMG : public MG
{
private:
    bool            useDefaultSolver; ///< use default smoothers and coarsest solver
    vector<HL<MAT>> infoHL;           ///< hierarichal info at all coarse levels

private:
    FaspRetCode SetupOneLevel(const MAT& A);     ///< default setup for one-level AMG
    vector<IdentityMatrix> defaultTrans;         ///< default transfer operators
    vector<class Jacobi>   defaultSolvers;       ///< default smoothers at coarse levels
    vector<class Jacobi>   defaultCoarseSolvers; ///< default solver at coarse levels

public:
    /// Default constructor.
    AMG()
        : useDefaultSolver(true){};

    /// Default destructor.
    ~AMG() = default;

    /// Setup the AMG method using coefficient matrix A.
    FaspRetCode Setup(const MAT& A);

    /// Solve Ax=b using the AMG method.
    // FaspRetCode Solve(const VEC &b, VEC &x) override;

    /// Clean up AMG data allocated during Setup.
    void Clean() override;
};

#endif /* end if for __AMG_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/12/2021      Create file                          */
/*----------------------------------------------------------------------------*/