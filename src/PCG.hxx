/**
 * a header file for PCG
 */

#ifndef SRC_PCG_HXX
#define SRC_PCG_HXX

#include "MAT.hxx"

FaspRetCode CG(const MAT &A,const VEC &b,VEC &x);

#endif //SRC_PCG_HXX
