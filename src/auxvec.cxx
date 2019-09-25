/**
 * source codes for VEC auxiliary functions
 */

#include "auxvec.hxx"
#include <cmath>

//! Check whether sizes match
FaspRetCode VECCheck_SizeMatch(VEC vec1, VEC vec2) {
    try {
        if ( vec1.GetSize() != vec2.GetSize()) {
            FaspRetCode errorCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw (FaspExcep(getRetCode(errorCode), __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspExcep &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

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
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

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
            std::cerr << " ### ERROR : " << ex.what() << std::endl;
            std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                      << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

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
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspRetCode::ERROR_VEC_SIZE;
    }

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/