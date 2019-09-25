/**
 * a header file for VEC auxiliary function
 */

#ifndef SRC_AUXVEC_HXX
#define SRC_AUXVEC_HXX

#include "VEC.hxx"

//! Check whether sizes match
FaspRetCode VECCheck_SizeMatch(VEC vec1,VEC vec2);

//! Check whether vector crossover
FaspRetCode VECCheck_Get(VEC vec,INT position);

//! Check whether there is a zero in VEC object
FaspRetCode VECCheck_TOL(VEC vec,DBL tol);

//! Check whether the size of VEC object is zero
FaspRetCode VECCheck_SizeZero(VEC vec);

#endif // SRC_AUXVEC_HXX

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/