/*! \file VECUtil.cxx
 *  \brief Some auxiliary functions for VEC
 *  \author Kailei Zhang
 *  \date Sep/25/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <cmath>

#include "VECUtil.hxx"

//! Check whether sizes match
FaspRetCode VECCheck_SizeMatch(VEC vec1, VEC vec2) {
    try {
        if ( vec1.GetSize() != vec2.GetSize()) {
            FaspRetCode errorCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw (FaspExcep(getRetCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspExcep &ex ) {
        ex.LogExcep();
        return FaspRetCode::ERROR_NONMATCH_SIZE;
    }

    return FaspRetCode::SUCCESS;
}

//! Check whether vector crossover
FaspRetCode VECCheck_Get(VEC vec, INT position) {
    try {
        if ( position >= vec.GetSize() || position < 0 ) {
            FaspRetCode errorCode = FaspRetCode::ERROR_VEC_SIZE;
            throw (FaspExcep(getRetCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspExcep &ex ) {
        ex.LogExcep();
        return FaspRetCode::ERROR_INPUT_PAR;
    }

    return FaspRetCode::SUCCESS;
}

//! Check whether there is a zero in VEC object
FaspRetCode VECCheck_TOL(VEC vec, DBL tol) {
    for ( INT j = 0; j < vec.GetSize(); j++ ) {
        try {
            if ( fabs(vec[j]) <= tol ) {
                FaspRetCode errorCode = FaspRetCode::ERROR_DIVIDE_ZERO;
                throw (FaspExcep(getRetCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
            }
        } catch ( FaspExcep &ex ) {
            ex.LogExcep();
            return FaspRetCode::ERROR_DIVIDE_ZERO;
        }
    }
    return FaspRetCode::SUCCESS;
}

//! Check whether the size of VEC object is zero
FaspRetCode VECCheck_SizeZero(VEC vec) {
    try {
        if ( vec.GetSize() == 0 ) {
            FaspRetCode errorCode = FaspRetCode::ERROR_VEC_SIZE;
            throw (FaspExcep(getRetCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspExcep &ex ) {
        ex.LogExcep();
        return FaspRetCode::ERROR_VEC_SIZE;
    }

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/