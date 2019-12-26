//
// Created by zkl on 12/24/19.
//

#ifndef FASPXX_KRYLOV_HXX
#define FASPXX_KRYLOV_HXX

#include "SOL.hxx"
#include "CG.hxx"
#include "BiCGStab.hxx"
#include "Iter.hxx"
#include "RetCode.hxx"

FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, Parameter params);

FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, SOL& pc, Parameter params);

#endif //FASPXX_KRYLOV_HXX
