/**
 * a test file
 */

#include <iostream>
#include "PCG.hxx"
#include <fstream>
#include "MATUtil.hxx"
#include "ReadData.hxx"
#include "Timing.hxx"

using namespace std;

int main() {
    INT row, col, nnz;
    std::vector<INT> colInd, rowPtr;
    std::vector<DBL> values;
    ReadMTX("/home/kailei/Project/faspsolver/data/sherman1.mtx", row, col,
            nnz, rowPtr, colInd, values);

    MAT mat;
    MTXtoMAT(row, col, nnz, rowPtr, colInd, values, mat);

    INT len = row;
    VEC init(len);

    ifstream in;
    in.open("/home/kailei/Data/init");
    INT tmp;
    in >> tmp >> tmp;

    for (int j = 0; j < len; j++)
        in >> init[j];

    VEC b = mat.MultVec(init);

    VEC x(row, 0.0);
    PCG pcg;
    PCGInputParam iparam;
    PCGOutputParam oparam;
    iparam.printLevel = PRINT_NONE;
    iparam.maxIter = len;
    iparam.relTol = 1e-6;
    iparam.absTol = 1e-9;
    iparam.restart = 20;
    LOP lop(row, col);

    GetWallTime timer;
    timer.start();

    pcg.SetUp(iparam);
    pcg.SetUpPCD(lop);
    pcg.Start(mat, b, x, STOP_REL_RES, oparam);
    cout << "PCG Time : " << timer.stop() << " seconds" << endl;

    DBL max=0;
    for(int j=0;j<len;j++){
        if(max<fabs(x[j]-init[j]))
            max=fabs(x[j]-init[j]);
    }

    cout<<"relTol : "<<iparam.relTol<<endl;
    cout<<"Max error : "<<max<<endl;
    cout<<"Iteration : "<<oparam.iter<<endl;
    cout<<"Norm2 : "<<oparam.norm2<<endl;
    cout<<"NormInf : "<<oparam.normInf<<endl;

    return 0;
}
