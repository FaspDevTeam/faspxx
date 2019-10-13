/*! \file Param.hxx
 *  \brief Parameter classes declaration
 *  \author Chensong Zhang
 *  \date Oct/11/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __PARAM_HEADER__      /*-- allow multiple inclusions --*/
#define __PARAM_HEADER__      /**< indicate Param.hxx has been included before */

#include "faspxx.hxx"

enum PRTLVL {
    PRINT_NONE = 0,
    PRINT_MIN  = 2,
    PRINT_SOME = 4,
    PRINT_MORE = 6,
    PRINT_MAX  = 8,
};

class IterParam {

private:
    // Input parameters
    PRTLVL outLvl;    ///< Output level
    INT    maxIter;   ///< Maximal number of iterations
    DBL    relTol;    ///< Tolerance for relative residual
    DBL    absTol;    ///< Tolerance for absolute residual
    INT    restart;   ///< Tolerance for absolute residual

    // Output parameters
    INT    numIter;   ///< Number of iteration used
    DBL    norm2;     ///< Eucliean norm of residual
    DBL    normInf;   ///< Infimum norm of residual

public:
    friend class PCG;

    IterParam() : outLvl(PRINT_NONE),
                  maxIter(100),
                  relTol(1e-6),
                  absTol(1e-10),
                  restart(25),
                  numIter(0),
                  norm2(1.0),
                  normInf(1.0)
                  {};

    IterParam(IterParam &inParam) {
        this->outLvl  = inParam.outLvl;
        this->maxIter = inParam.maxIter;
        this->relTol  = inParam.relTol;
        this->absTol  = inParam.absTol;
        this->restart = inParam.restart;
    }

    // Set input parameters
    __inline__ void SetOutLvl (const PRTLVL& outLvl) { this->outLvl = outLvl; };
    __inline__ void SetMaxIter(const INT& maxIter)   { this->maxIter = maxIter; };
    __inline__ void SetRelTol (const DBL& relTol)    { this->relTol = relTol; };
    __inline__ void SetAbsTol (const DBL& absTol)    { this->absTol = absTol; };
    __inline__ void SetRestart(const INT& restart)   { this->restart = restart; };

    // Get output parameters
    __inline__ INT GetNumIter() const { return numIter; };
    __inline__ DBL GetNorm2()   const { return norm2; };
    __inline__ DBL GetNormInf() const { return normInf; };

    // Print parameters
    void Print() const {
        std::cout << "Parameters for iterative method:   " << std::endl
                  << "-----------------------------------" << std::endl
                  << "  Output level:         " << outLvl  << std::endl
                  << "  Max num of iteration: " << maxIter << std::endl
                  << "  Relative tolerance:   " << relTol  << std::endl
                  << "  Absolute tolerance:   " << absTol  << std::endl
                  << "  Restart number:       " << restart << std::endl
                  << "-----------------------------------" << std::endl;
    };

    ~IterParam() {};
};

#endif /* end if for __PARAM_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/