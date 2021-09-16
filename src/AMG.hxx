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
    unsigned numLevelsMax;     ///< max number of levels
    unsigned numLevelsUse;     ///< number of levels in use <= max_levels
    bool     useSymmRoperator; ///< use symmetric restriction operator
    bool     useDefaultSolver; ///< use default smoothers and coarsest solver
    unsigned len;              ///< dimension of the solution vector

    vector<MAT>   coeffMatrices; ///< coefficient matrices at all levels
    vector<MAT *> prolongations; ///< pointers to prolongations at all levels
    vector<MAT *> restrictions;  ///< pointers to restrictions at all levels

private:
    FaspRetCode SetupOneLevel(const MAT &A);      ///< default setup for one-level AMG
    vector<IdentityMatrix> defaultTrans;          ///< default transfer operators
    vector<class Jacobi>   defaultSolvers;        ///< default smoothers at all levels
    class Jacobi           dafaultCoarsestSolver; ///< default solver at coarsest level

public:
    /// Default constructor.
    AMG()
        : numLevelsMax(20)
        , numLevelsUse(1)
        , useSymmRoperator(true)
        , useDefaultSolver(true){};

    /// Default destructor.
    ~AMG() = default;

    /// Setup the AMG method using coefficient matrix A.
    FaspRetCode Setup(const MAT &A);

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