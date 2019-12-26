/**
 *
 */

#include "Krylov.hxx"

FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, Parameter params){
    switch(params.type) {
        case SOLType::CG:
            class CG cg;
            cg.SetOutput(params.verbose);
            cg.SetMaxIter(params.maxIter);
            cg.SetMinIter(params.minIter);
            cg.SetRestart(params.restart);
            cg.SetRelTol(params.relTol);
            cg.SetAbsTol(params.absTol);
            cg.SetSafeIter(params.safeIter);
            cg.Setup(A);
            return cg.Solve(b, x);
        case SOLType::BICGSTAB:
            class BiCGStab bicg;
            bicg.SetOutput(params.verbose);
            bicg.SetMaxIter(params.maxIter);
            bicg.SetMinIter(params.minIter);
            bicg.SetRestart(params.restart);
            bicg.SetRelTol(params.relTol);
            bicg.SetAbsTol(params.absTol);
            bicg.SetSafeIter(params.safeIter);
            bicg.Setup(A);
            return bicg.Solve(b, x);
    }

}

FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, SOL& pc, Parameter params){
    switch(params.type) {
        case SOLType::CG:
            class CG cg;
            cg.SetOutput(params.verbose);
            cg.SetMaxIter(params.maxIter);
            cg.SetMinIter(params.minIter);
            cg.SetRestart(params.restart);
            cg.SetRelTol(params.relTol);
            cg.SetAbsTol(params.absTol);
            cg.SetSafeIter(params.safeIter);
            cg.Setup(A);
            cg.SetPC(&pc);
            return cg.Solve(b, x);
        case SOLType::BICGSTAB:
            class BiCGStab bicg;
            bicg.SetOutput(params.verbose);
            bicg.SetMaxIter(params.maxIter);
            bicg.SetMinIter(params.minIter);
            bicg.SetRestart(params.restart);
            bicg.SetRelTol(params.relTol);
            bicg.SetAbsTol(params.absTol);
            bicg.SetSafeIter(params.safeIter);
            bicg.Setup(A);
            bicg.SetPC(&pc);
            return bicg.Solve(b, x);
    }
}