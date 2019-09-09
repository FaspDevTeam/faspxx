/**
 * test vec.hxx and vec.cxx
 */

#include "vec.hxx"
#include <iostream>

using namespace std;

int main()
{
    VEC a(4,1.0);
    VEC b(a);
    VEC c;

    DBL dot,add,value,max,min;
    INT length;

    a.Get(1,value);
    cout<<"a[1] : "<<value<<endl;

    a.Len(length);
    cout<<"length : "<<length<<endl;

    a.Scale(0.23);
    a.Get(1,value);
    cout<<"a[1] : "<<value<<endl;

    a.Copy(c);
    c.Get(1,value);
    cout<<"c[1] : "<<value<<endl;
    a.Min(min);
    a.Max(max);
    cout<<"max : "<<max<<endl;
    cout<<"min : "<<min<<endl;

    a.Shift(12.89);
    a.Get(1,value);
    cout<<"shift : "<<value<<endl;

    a.Reciprocal();
    a.Get(1,value);
    cout<<"reciprocal : "<<value<<endl;

    a.Norm2(value);
    cout<<"norm2 : "<<value<<endl;

    a.NormInf(value);
    cout<<"normInf : "<<value<<endl;

    a.Add(b,3.14,2.87);
    a.Get(1,value);
    cout<<"add : "<<value<<endl;

    a.Dot(b,dot);
    cout<<"dot : "<<dot<<endl;

    a.PointWiseMult(b);
    a.Get(1,value);
    cout<<"mult : "<<value<<endl;

    a.PointWiseDivide(b);
    a.Get(1,value);
    cout<<"divide : "<<value<<endl;

    a.PointWiseDivided(b);
    a.Get(1,value);
    cout<<"divided : "<<value<<endl;

    a.operator=(b);
    a.Get(1,value);
    cout<<"a= : "<<value<<endl;
    b.Get(1,value);
    cout<<"b= : "<<value<<endl;

    return 0;
}