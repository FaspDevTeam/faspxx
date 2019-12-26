/**
 *
 */

#include "Krylov.hxx"
#include "SOL.hxx"

FaspRetCode Krylov(LOP& A, VEC& b, VEC& x, SOL& pc, Params params){
    FaspRetCode retCode;
    SOL *sol;
    switch(params.type) {
        case SOLType::CG:
            std::cout<<"****   Apply CG to solve   ****"<<std::endl;
            sol=new class CG();
            sol->SetOutput(params.verbose);
            sol->SetMaxIter(params.maxIter);
            sol->SetMinIter(params.minIter);
            sol->SetRestart(params.restart);
            sol->SetRelTol(params.relTol);
            sol->SetAbsTol(params.absTol);
            sol->SetSafeIteration(params.safeIter);
            sol->Setup(A);
            sol->SetPC(&pc);
            retCode=sol->Solve(b, x);
            break;
        case SOLType::BICGSTAB:
            std::cout<<"****   Apply BiCGStab to solve   ****"<<std::endl;
            sol=new class BiCGStab();
            sol->SetOutput(params.verbose);
            sol->SetMaxIter(params.maxIter);
            sol->SetMinIter(params.minIter);
            sol->SetRestart(params.restart);
            sol->SetRelTol(params.relTol);
            sol->SetAbsTol(params.absTol);
            sol->SetSafeIteration(params.safeIter);
            sol->Setup(A);
            sol->SetPC(&pc);
            retCode=sol->Solve(b, x);
            break;
    }

    delete sol;
    return retCode;
}