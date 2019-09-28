/**
 * a demo file
 */

#include "PCG.hxx"
#include "ReadData.hxx"
#include "MATUtil.hxx"
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

int main() {

#if 0
    INT nrow=4,ncol=4;
    INT nnz=10;
    std::vector<INT> rowPtr{0,2,5,8,10};
    std::vector<INT> colInd{0,1,0,1,2,1,2,3,2,3};
    std::vector<DBL> values{4,1,1,4,1,1,4,1,1,4};
    std::vector<INT> diagPtr{0,3,6,9};

    MAT mat(nrow,ncol,nnz,values,colInd,rowPtr,diagPtr);

    std::vector<DBL> result{5,6,6,5};
    VEC b(result);

    std::vector<DBL> init(4,0.25);
    VEC x(init);

    CG(mat,b,x);

    for(INT j=0;j<4;j++)
        cout<<x[j]<<"  ";
#endif

    const INT L=200;

    INT row=L,col=L,nnz=3*L-2;
    std::vector<INT> rowPtr(row+1);
    rowPtr[0]=0;
    rowPtr[1]=2;
    for(INT j=1;j<row-1;j++)
        rowPtr[j+1]=rowPtr[j]+3;
    rowPtr[row]=rowPtr[row-1]+2;

    std::vector<INT> colInd(nnz);
    std::vector<DBL> values(nnz);

    INT count=0;
    colInd[count]=0;
    values[count]=4;
    count++;
    colInd[count]=1;
    values[count]=1;
    count++;
    for(INT j=1;j<row-1;j++){
        colInd[count]=j-1;
        values[count]=1;
        count++;
        colInd[count]=j;
        values[count]=4;
        count++;
        colInd[count]=j+1;
        values[count]=1;
        count++;
    }
    colInd[count]=col-2;
    values[count]=1;
    count++;
    colInd[count]=col-1;
    values[count]=4;
    count++;

    std::vector<INT> diagPtr(L);
    count=0;
    INT begin,end;
    for(INT j=0;j<row;j++){
        begin=rowPtr[j];
        end=rowPtr[j+1];
        for(INT k=begin;k<end;k++){
            if(colInd[k]==j){
                diagPtr[j]=k;
                count++;
            }
        }
    }
    if(count!=L){
        cout<<"It is not CSRx format\n";
        return 0;
    }

    MAT mat(row,col,nnz,values,colInd,rowPtr,diagPtr);

#define GENERATE 1
#if GENERATE
    fstream iotmp;
    VEC xtmp(col,0.0);
    iotmp.open("/home/kailei/data/ori_x");

    srand(unsigned(time(0)));

    for(INT j=0;j<col;j++){
        xtmp[j]=rand()/(double)RAND_MAX;
        iotmp<<xtmp[j]<<"\n";
    }
    iotmp.close();

    VEC btmp=mat.MultVec(xtmp);

    iotmp.open("/home/kailei/data/b");
    for(INT j=0;j<row;j++)
        iotmp<<btmp[j]<<"\n";
    iotmp.close();

#endif
# if 1
    fstream io;
    VEC b(row,0.0),control(col,0.0);

    io.open("/home/kailei/data/ori_x");
    for(INT j=0;j<col;j++)
        io>>control[j];
    io.close();

    io.open("/home/kailei/data/b");
    for(INT j=0;j<row;j++)
        io>>b[j];
    io.close();

    VEC x(col,0.0);

    CG(mat,b,x);

    io.open("/home/kailei/data/x");
    for(INT j=0;j<col;j++)
        io<<x[j]<<"\n";
    io.close();

    DBL Tol=0.0;
    for(INT j=0;j<col;j++){
        if(Tol<fabs(x[j]-control[j]))
            Tol=fabs(x[j]-control[j]);
    }
    cout<<"Tol : "<<Tol<<endl;
#endif
    return 0;
}
