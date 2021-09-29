/*! \file    VECUtil.cxx
 *  \brief   Tools definition for checking and manipulating VEC
 *  \author  Kailei Zhang
 *  \date    Sep/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Standard header files
#include <cmath>

// FASPXX header files
#include "VECUtil.hxx"

/// Check whether the size of VEC object is zero.
FaspRetCode CheckVECSize(const VEC& v)
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    try {
        if (v.GetSize() == 0) {
            retCode = FaspRetCode::ERROR_VEC_SIZE;
            throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        }
    } catch (FaspRunTime& ex) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return retCode;
}

/// Check whether two VEC sizes match.
FaspRetCode CheckVECSize(const VEC& v1, const VEC& v2)
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    try {
        if (v1.GetSize() != v2.GetSize()) {
            retCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        }
    } catch (FaspRunTime& ex) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return retCode;
}

/// Check whether vector crossover.
FaspRetCode CheckVECSize(const VEC& v, const USI& position)
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    try {
        if (position >= v.GetSize() || position < 0) {
            retCode = FaspRetCode::ERROR_VEC_SIZE;
            throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        }
    } catch (FaspRunTime& ex) {
        ex.LogExcep();
        return ex.errorCode;
    }
    return retCode;
}

/// Check whether there is a zero entry in VEC object.
FaspRetCode CheckVECZero(const VEC& v, const DBL tol)
{
    FaspRetCode retCode = FaspRetCode::SUCCESS; // Return success if no-throw
    for (USI j = 0; j < v.GetSize(); ++j) {
        try {
            if (fabs(v[j]) <= tol) {
                retCode = FaspRetCode::ERROR_DIVIDE_ZERO;
                throw(FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
            }
        } catch (FaspRunTime& ex) {
            ex.LogExcep();
            return ex.errorCode;
        }
    }
    return retCode;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Sep/25/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/
