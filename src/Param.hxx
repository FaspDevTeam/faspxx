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
#include <cstring>
#include <iostream>
#include <ostream>

enum PRTLVL {
    PRINT_NONE = 0,
    PRINT_MIN = 2,
    PRINT_SOME = 4,
    PRINT_MORE = 6,
    PRINT_MAX = 8,
};

class IterParam {

private:
    // Input parameters
    PRTLVL outLvl;    ///< Output level
    INT maxIter;   ///< Maximal number of iterations
    DBL relTol;    ///< Tolerance for relative residual
    DBL absTol;    ///< Tolerance for absolute residual
    INT restart;   ///< Tolerance for absolute residual

    // Output parameters
    INT numIter;   ///< Number of iteration used
    DBL norm2;     ///< Eucliean norm of residual
    DBL normInf;   ///< Infimum norm of residual

public:
    friend class PCG;

    IterParam() : outLvl(PRINT_NONE),
                  maxIter(100),
                  relTol(1e-6),
                  absTol(1e-10),
                  restart(20),
                  numIter(0),
                  norm2(1.0),
                  normInf(1.0) {};

    IterParam(IterParam &inParam) {
        this->outLvl = inParam.outLvl;
        this->maxIter = inParam.maxIter;
        this->relTol = inParam.relTol;
        this->absTol = inParam.absTol;
        this->restart = inParam.restart;
    }

    // Set input parameters
    __inline__ void SetOutLvl(const PRTLVL &outLvl) { this->outLvl = outLvl; };

    __inline__ void SetMaxIter(const INT &maxIter) { this->maxIter = maxIter; };

    __inline__ void SetRelTol(const DBL &relTol) { this->relTol = relTol; };

    __inline__ void SetAbsTol(const DBL &absTol) { this->absTol = absTol; };

    __inline__ void SetRestart(const INT &restart) { this->restart = restart; };

    // Get output parameters
    __inline__ INT GetNumIter() const { return numIter; };

    __inline__ DBL GetNorm2() const { return norm2; };

    __inline__ DBL GetNormInf() const { return normInf; };

    // Print parameters
    // TODO:给Print()加一个默认参数就可以将输出重定向到任何流,比如文件; 用\n替换std::endl可以避免重复刷新缓冲流
    void Print(std::ostream &out = std::cout) const {
            out << "\nParameters for iterative method: \n"
                << "-----------------------------------\n"
                << "  Output level:         " << outLvl  << '\n'
                << "  Max num of iteration: " << maxIter << '\n'
                << "  Relative tolerance:   " << relTol  << '\n'
                << "  Absolute tolerance:   " << absTol  << '\n'
                << "  Restart number:       " << restart << '\n'
                << "-----------------------------------\n";
    };

    ~IterParam() {};
};

// Matrix name, rhs name and lhs name
class MatRhsLhs {
private:
    char *matName;
    char *rhsName;
    char *lhsName;

public:
    // default construtor
    MatRhsLhs() : matName(nullptr), rhsName(nullptr), lhsName(nullptr) {};

    // constructor
    MatRhsLhs(const char *_matName, const char *_rhsName, const char *_lhsName) {
        matName = new char[strlen(_matName)];
        memcpy(matName, _matName, strlen(_matName));
        rhsName = new char[strlen(_rhsName)];
        memcpy(rhsName, _rhsName, strlen(_rhsName));
        lhsName = new char[strlen(_lhsName)];
        memcpy(lhsName, _lhsName, strlen(_lhsName));
    }

    // destructor
    ~MatRhsLhs() {
        if (matName != nullptr)
            delete[] matName;
        if (rhsName != nullptr)
            delete[] rhsName;
        if (lhsName != nullptr)
            delete[] lhsName;
    }

    // set matrices' name
    void SetMatName(const char *_matName) {
        matName = new char[strlen(_matName)];
        memcpy(matName, _matName, strlen(_matName));
    }

    // set rhs 's name
    void SetRhsName(const char *_rhsName) {
        rhsName = new char[strlen(_rhsName)];
        memcpy(rhsName, _rhsName, strlen(_rhsName));
    }

    // set lhs 's name
    void SetLhsName(const char *_lhsName) {
        lhsName = new char[strlen(_lhsName)];
        memcpy(lhsName, _lhsName, strlen(_lhsName));
    }

    char *GetMatName() const { return matName; }  // Get matrices' name
    char *GetRhsName() const { return rhsName; }  // Get rhs 's name
    char *GetLhsName() const { return lhsName; }  // Get lhs 's name

    void Print(std::ostream &out = std::cout) const {
        std::cout << "\nCommand line input: \n"
                  << "-----------------------------------\n";
        if ( matName != nullptr )
            std::cout << "Matrix data file: " << matName << std::endl;
        else
            std::cout << "### ERROR: No matrix data specified!\n";

        if ( rhsName != nullptr )
            std::cout << "RHS Data: " << rhsName << std::endl;
        else
            std::cout << "### WARNING: No RHS data specified!\n";

        if ( lhsName != nullptr )
            std::cout << "Initial guess: " << lhsName << std::endl;
        else
            std::cout << "### WARNING: No initial guess specified!\n";
        std::cout << "-----------------------------------\n";
    };
};

// Initialize parameters
struct InitParam {
    MatRhsLhs data;
    IterParam param;
};

#endif /* end if for __PARAM_HEADER__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/