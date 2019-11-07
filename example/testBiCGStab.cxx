/**
 * a test file
 */

#include "BiCGStab.hxx"
#include <iostream>
#include "ReadData.hxx"
#include "Param.hxx"

using namespace std;

int main(int argc, char *argv[]) {

    MAT A;

    ReadMat("../data/fdm_10X10.csr", A);

    INT row = A.GetRowSize(), col = A.GetColSize();

    BiCGStab Bi;
    IterParam param;

    param.SetAbsTol(1e-4);
    param.SetVerbose(PRINT_MORE);
    param.SetRelTol(1e-8);
    param.SetMaxIter(400);
    param.Print();

    VEC b(row,0.0),x(col,1.0);

    Bi.Setup(A,b,x,param);
    Bi.Solve(A,b,x,param);
    Bi.Clean();

    VEC tmp(row);
    A.Apply(x,tmp);

    double realError=0;
    double tmpError;
    for(int j=0;j<row;j++){
       tmpError=fabs(b[j]-tmp[j]);
       if(realError<tmpError)
           realError=tmpError;
    }
    cout<<"realError : "<<realError<<endl;

    cout<<"NumIter : "<<param.GetNumIter()<<endl;
    cout<<"NormInf : "<<param.GetNormInf()<<endl;
    cout<<"Norm2 : "<<param.GetNorm2()<<endl;

    return 0;
}