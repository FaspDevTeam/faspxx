/**
 * a header file for VEC auxiliary function
 */

#ifndef SRC_AUXVEC_HXX
#define SRC_AUXVEC_HXX

#include "vec.hxx"

//! Check whether sizes match
FaspErrorCode VECCheck_SizeMatch(VEC vec1,VEC vec2);

//! Check whether vector crossover
FaspErrorCode VECCheck_Get(VEC vec,INT position);

//! Check whether there is a zero in VEC object
FaspErrorCode VECCheck_TOL(VEC vec,DBL tol);

//! Check whether the size of VEC object is zero
FaspErrorCode VECCheck_SizeZero(VEC vec);

#endif //SRC_AUXVEC_HXX
