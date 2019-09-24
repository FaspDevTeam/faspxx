/**
 * source codes for VEC auxiliary functions
 */

#include "auxvec.hxx"
#include <cmath>

//! Check whether sizes match
FaspErrorCode VECCheck_SizeMatch(VEC vec1, VEC vec2) {
    try {
        if ( vec1.GetSize() != vec2.GetSize()) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_NONMATCH_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_NONMATCH_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check whether vector crossover
FaspErrorCode VECCheck_Get(VEC vec, INT position) {
    try {
        if ( position >= vec.GetSize() || position < 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_INPUT_PAR;
    }

    return FaspErrorCode::SUCCESS;
}

//! Check whether there is a zero in VEC object
FaspErrorCode VECCheck_TOL(VEC vec, DBL tol) {
    for ( INT j = 0; j < vec.GetSize(); j++ ) {
        try {
            if ( fabs(vec[j]) <= tol ) {
                FaspErrorCode errorCode = FaspErrorCode::ERROR_DIVIDE_ZERO;
                throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                     __LINE__));
            }
        } catch ( FaspException &ex ) {
            std::cerr << " ### ERROR : " << ex.what() << std::endl;
            std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                      << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

            return FaspErrorCode::ERROR_DIVIDE_ZERO;
        }

        return FaspErrorCode::SUCCESS;
    }
}

//! Check whether the size of VEC object is zero
FaspErrorCode VECCheck_SizeZero(VEC vec) {
    try {
        if ( vec.GetSize() == 0 ) {
            FaspErrorCode errorCode = FaspErrorCode::ERROR_VEC_SIZE;
            throw (FaspException(getErrorCode(errorCode), __FILE__, __FUNCTION__,
                                 __LINE__));
        }
    } catch ( FaspException &ex ) {
        std::cerr << " ### ERROR : " << ex.what() << std::endl;
        std::cerr << " ### ERROR : Check " << ex.getFile() << " in Function : "
                  << ex.getFunc() << " at Line " << ex.getLine() << std::endl;

        return FaspErrorCode::ERROR_VEC_SIZE;
    }

    return FaspErrorCode::SUCCESS;
}
