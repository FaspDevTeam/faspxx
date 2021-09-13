/*! \file    MG.hxx
 *  \brief   Abstract MG class declaration
 *  \author  Chensong Zhang
 *  \date    Oct/12/2021
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
#include "LOP.hxx"
#include "MAT.hxx"
#include "SOL.hxx"

using std::vector;

/*! \class MG
 *  \brief Multigrid method in an abstract setting.
 */
class MG : public SOL {
private:
    unsigned numLevelsMax;     ///< max number of levels
    unsigned numLevelsUse;     ///< number of levels in use <= max_levels
    bool     useSymmRoperator; ///< use symmetric restriction operator
    unsigned len;              ///< dimension of the solution vector

    vector<LOP>   coeffMatrices; ///< coefficient matrices at all levels
    vector<LOP *> prolongations; ///< pointers to prolongations at all levels
    vector<LOP *> restrictions;  ///< pointers to restrictions at all levels

public:
    vector<unsigned> sizes;    ///< problem sizes at all levels
    vector<unsigned> cycles;   ///< number of cycles at all levels
    vector<VEC>      bVectors; ///< right-hand side vectors b at all levels
    vector<VEC>      xVectors; ///< solution vectors x at all levels
    vector<VEC>      wVectors; ///< work vectors w at all levels

    vector<SOL *> preSolvers;     ///< pointers to presmoothers at all levels
    vector<SOL *> postSolvers;    ///< pointers to presmoothers at all levels
    SOL *         coarsestSolver; ///< pointer to solver at coarsest level

private:
    void oneCycleMultigrid(const VEC &b, VEC &x); ///< one multigrid cycle

public:
    /// Default constructor.
    MG()
        : numLevelsMax(20)
        , numLevelsUse(1)
        , useSymmRoperator(true){};

    /// Default destructor.
    ~MG() = default;

    /// Solve Ax=b using the MG method.
    FaspRetCode Solve(const VEC &b, VEC &x) override;

    /// Clean up MG data allocated during Setup.
    void Clean() override;
};

#endif /* end if for __MG_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/