/**
 * the *.cpp file intends to exam all the header files' functions
 */

#include<iostream>
#include"dVector.h"
using namespace std;

int main()
{
    Vector v;
    double *arr;
    int j,tol=2;
    int row;
    double value;

    // test SetSameValue, GetRow, GetValue, GetValues
    v.SetSameValue(2,1.5);
    v.GetRow(&row);
    cout<<row<<endl;
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }
    v.GetValues(&arr);
    for(j=0;j<tol;j++){
        cout<<"arr["<<j<<"] : "<<arr[j]<<endl;
    }

    // test SetValues
    cout<<"test SetValues"<<endl;
    arr[0]=1.23;
    arr[1]=10;
    v.SetValues(2,arr);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    cout<<"test Scale"<<endl;
    v.Scale(0.314);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    cout<<"test Dot"<<endl;
    Vector b;
    b.SetSameValue(2,2.3);
    double dot;
    v.Dot(b,&dot);
    cout<<"dot : "<<dot<<endl;

    cout<<"test Add"<<endl;
    v.Add(b);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    double par=0.98;
    cout<<"test AddAmultVec"<<endl;
    v.AddAmultVec(par,b);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    cout<<"test AmultAddVec"<<endl;
    v.AmultAddVec(par,b);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    v.Scale(-1);
    cout<<"test Abs"<<endl;
    v.Abs();
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }
    
    cout<<"test PointWiseMult"<<endl;
    v.PointWiseMult(b);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    cout<<"test Max"<<endl;
    double max;
    v.Max(&max);
    cout<<"max : "<<max<<endl;

    cout<<"test Min"<<endl;
    double min;
    v.Min(&min);
    cout<<"min : "<<min<<endl;

    cout<<"test Sum"<<endl;
    double sum;
    v.Sum(&sum);
    cout<<"sum : "<<endl;

    cout<<"test Reciprocal"<<endl;
    v.Reciprocial();
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    cout<<"test Shift"<<endl;
    v.Shift(12.3);
    for(j=0;j<tol;j++){
        v.GetValue(j,&value);
        cout<<"the "<<j<<"th : "<<value<<endl;
    }

    cout<<"test Norm1"<<endl;
    double norm1;
    v.Norm1(&norm1);
    cout<<"norm1 : "<<norm1<<endl;

    cout<<"test Norm2"<<endl;
    double norm2;
    v.Norm2(&norm2);
    cout<<"norm2 : "<<norm2<<endl;

    cout<<"test NormInfinity"<<endl;
    double norminfinity;
    v.NormInfinity(&norminfinity);
    cout<<"norminifinity : "<<norminfinity<<endl;

    cout<<"test Normp"<<endl;
    double normp;
    v.NormP(&normp,3);
    cout<<"normp : "<<normp<<endl;
    return 0;
}