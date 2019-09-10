/**
 * a test file
 */

#include "vec.hxx"
#include <iostream>

using namespace std;

int main()
{

    //VEC():size(0), vec(0) {}
    /**
     * output :
     * v0.GetSize() : 0
     */
    VEC v0;
    cout<<"v0.GetSize() : "<<v0.GetSize()<<endl;

    //VEC(const INT size, const DBL value)
    //DBL &operator [] (const INT position)
    /**
     * output :
     * v1.GetSize() : 4
     * 2.312, 2.312, 2.312, 2.312
     */
    VEC v1(4,2.312);
    cout<<"v1.GetSize() : "<<v1.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v1[j]<<"  ";
    }
    cout<<endl;

    //VEC(const std::vector<DBL> vector_obj)
    //DBL &operator [] (const INT position)
    /**
     * output:
     * 1.23, 0.98, 234.67, 456
     * v2.GetSize() : 4
     * 1.23, 0.98, 234.67, 456
     */
    vector<DBL> vector1;
    vector1.resize(4);
    vector1[0]=1.23;
    vector1[1]=0.98;
    vector1[2]=234.67;
    vector1[3]=456;
    for(int j=0;j<4;j++){
        cout<<vector1[j]<<"  ";
    }
    cout<<endl;
    VEC v2(vector1);
    cout<<"v2.GetSize() : "<<v2.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v2[j]<<"  ";
    }
    cout<<endl;

    //VEC(const VEC &vec_obj)
    //DBL &operator [] (const INT position)
    /**
     * output:
     * v3.GetSize() : 4
     * 1.23, 0.98, 234.67, 456
     */
    VEC v3(v2);
    cout<<"v3.GetSize() : "<<v3.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v3[j]<<"  ";
    }
    cout<<endl;

    //VEC(const DBL begin_value, const DBL end_value, const INT N)
    //DBL &operator [] (const INT position)
    /**
     * output :
     * attention: random values aren't uncertain
     */
    VEC v4(21,34,6);
    cout<<"v4.GetSize() : "<<v4.GetSize()<<endl;
    for(int j=0;j<6;j++){
        cout<<v4[j]<<"  ";
    }
    cout<<endl;

    //VEC(const DBL *pointer, const INT size)
    //DBL &operator [] (const INT position)
    /**
     * output:
     * v5.GetSize() : 4
     * 452, 0.12354, 8964, 0.567
     */
    double *p=new double[4];
    p[0]=452;
    p[1]=0.12354;
    p[2]=8964;
    p[3]=0.567;
    VEC v5(p,4);
    cout<<"v5.GetSize() : "<<v5.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v5[j]<<"  ";
    }
    cout<<endl;

    //VEC &operator = (const VEC &vec_obj)
    //DBL &operator [] (const INT position)
    /**
     * output :
     * v6.GetSize() : 4
     * 452, 0.12354, 8964, 0.567
     */
    VEC v6=v5;
    cout<<"v6.GetSize() : "<<v6.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v6[j]<<"  ";
    }
    cout<<endl;

    //VEC &operator += (const VEC &vec_obj)
    /**
     * output :
     * v6.GetSize() : 4
     * 904, 0.24708, 17928, 1.134
     */
    v6+=v5;
    cout<<"v6.GetSize() : "<<v6.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v6[j]<<"  ";
    }
    cout<<endl;

    //VEC &operator -= (const VEC &vec_obj)
    /**
     * output :
     * 452, 0.12354, 8964, 0.567
     */
    v6-=v5;
    cout<<"v6.GetSize() : "<<v6.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v6[j]<<"  ";
    }
    cout<<endl;

    //void SetSize(const INT size)
    /**
     * output :
     * v6.GetSize() : 10
     */
    v6.SetSize(10);
    cout<<"v6.GetSize() : "<<v6.GetSize()<<endl;

    //void SetValues(const INT size, const DBL value)
    //void SetValues(const std::vector<DBL> vector_obj)
    //void SetValues(const DBL begin_value, const DBL end_value, const INT N)
    //void SetValues(const INT size, const DBL *array)
    /**
     * output :
     * v7.GetSize() : 6
     * 3.14, 3.14, 3.14, 3.14, 3.14, 3.14
     * v8.GetSize() : 4
     * 1.23, 0.98, 234.67, 456
     * v9.GetSize() : 6
     * random values
     * v10.GetSize() : 4
     * 452, 0.12354, 8964, 0.567
     */
    VEC v7,v8,v9,v10;
    v7.SetValues(6,3.14);
    cout<<"v7.GetSize() : "<<v7.GetSize()<<endl;
    for(int j=0;j<6;j++){
        cout<<v7[j]<<"  ";
    }
    cout<<endl;
    v8.SetValues(vector1);
    cout<<"v8.GetSize() : "<<v8.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v8[j]<<"  ";
    }
    cout<<endl;
    v9.SetValues(10,100,6);
    cout<<"v9.GetSize() : "<<v9.GetSize()<<endl;
    for(int j=0;j<6;j++){
        cout<<v9[j]<<"  ";
    }
    cout<<endl;
    v10.SetValues(4,p);
    cout<<"v10.GetSize() : "<<v10.GetSize()<<endl;
    for(int j=0;j<4;j++){
        cout<<v10[j]<<"  ";
    }
    cout<<endl;

    //void Get(const INT position, DBL &value) const
    /**
     * output :
     * 8964
     */
    DBL value;
    v10.Get(2,value);
    cout<<"v10.Get() : "<<value<<endl;

    //void GetArray(const INT &size, DBL **array) const
    /**
     * output :
     * 452, 0.12354, 8964
     */
    DBL *array;
    v10.GetArray(3,&array);
    cout<<"array : ";
    for(int j=0;j<3;j++){
        cout<<array[j]<<"  ";
    }
    cout<<endl;

    //INT GetSize() const
    //INT GetCapacity() const
    /**
     * output :
     * v10.GetSize() : 4
     * v10.Capacity() : 4
     */
    cout<<"v10.GetSize() : "<<v10.GetSize()<<endl;
    cout<<"v10.GetCapacity() : "<<v10.GetCapacity()<<endl;

    //void Scale(const DBL a)
    /**
     * output :
     * v10.Scale() : 1297.24, 0.35456, 25726.7, 1.62729
     */
    v10.Scale(2.87);
    cout<<"v10.Scale() : ";
    for(int j=0;j<v10.GetSize();j++){
        cout<<v10[j]<<"  ";
    }
    cout<<endl;

    //void Copy(VEC &vec_obj) const
    /**
     * output :
     * v10.Copy() : 1297.24, 0.35456, 25726.7, 1.62729
     */
    VEC v11;
    cout<<"v10.Copy() : ";
    v10.Copy(v11);
    for(int j=0;j<v10.GetSize();j++){
        cout<<v11[j]<<"  ";
    }
    cout<<endl;

    //void Max(DBL &max) const
    //void Min(DBL &min) const
    /**
     * output :
     * max : 25726.7
     * min : 0.35456
     */
    DBL max,min;
    v10.Max(max);
    v10.Min(min);
    cout<<"max : "<<max<<endl;
    cout<<"min : "<<min<<endl;

    //void Shift(DBL s)
    /**
     * output :
     * v10.Shift() : 2297.24  1000.35  26726.7  1001.63
     */
    v10.Shift(1000);
    cout<<"v10.Shift() : ";
    for(int j=0;j<v10.GetSize();j++){
        cout<<v10[j]<<"  ";
    }
    cout<<endl;

    //void Abs()
    /**
     * output :
     * v10.Abs() : 2297.24  1000.35  26726.7  1001.63
     */
    v10.Abs();
    cout<<"v10.Abs() : ";
    for(int j=0;j<v10.GetSize();j++){
        cout<<v10[j]<<"  ";
    }
    cout<<endl;

    //void Reciprocal(DBL tol)
    /**
     * output :
     * v10.GetSize() : 4
     * v10.Reciprocal() : 0.000435305, 0.000999646, 3.74158e-05, 0.000998375
     */
    cout<<"v10.GetSize() : "<<v10.GetSize()<<endl;
    cout<<"v10.Reciprocal() : ";
    v10.Reciprocal(10e-5);
    for(int j=0;j<v10.GetSize();j++){
        cout<<v10[j]<<"  ";
    }
    cout<<endl;

    //DBL Norm2() const
    //DBL NormInf() const
    /**
     * output :
     * Norm2 : 0.00147883
     * NormInf : 0.000999646
     */
    DBL norm2,norminf;
    norm2=v10.Norm2();
    norminf=v10.NormInf();
    cout<<"Norm2 : "<<norm2<<endl;
    cout<<"NormInf : "<<norminf<<endl;

    //FaspErrorType Add(const VEC &vec_obj, const DBL a, const DBL b)
    //FaspErrorType Add(const DBL a, const VEC &vec1_obj, const DBL b, const VEC &vec2_obj)
    /**
     * output :
     * v1.Add() : 1304.5, 7.61424, 25733.9, 8.88697
     * v12.Add() : 4498.41, 24.0187, 88782.5, 28.4097
     */
    cout<<"v1.Add() : ";
    v1.Add(v5,3.14,2.87);
    for(int j=0;j<4;j++){
        cout<<v1[j]<<"  ";
    }
    cout<<endl;
    VEC v12;
    cout<<"v12.Add() : ";
    v12.Add(3.14,v1,0.89,v5);
    for(int j=0;j<4;j++){
        cout<<v12[j]<<"  ";
    }
    cout<<endl;

    //FaspErrorType Dot(const VEC &vec_obj, DBL &dot) const
    /**
     * output :
     * v12.Dot() : 2.29059e+09
     */
    DBL dot;
    cout<<"v12.Dot() : ";
    v12.Dot(v1,dot);
    cout<<dot<<endl;

    //FaspErrorType PointWiseMult(const VEC &vec_obj)
    //FaspErrorType PointWiseDivide(const VEC &vec_obj,DBL tol)
    /**
     * output :
     * v12.PointWiseMult : 5.86817e+06, 182.884, 2.28472e+09, 252.476
     * v12.PointWiseDivide : 4498.41, 24.0187, 88782.5, 28.4097
     */
    cout<<"v12.PointWiseMult : ";
    v12.PointWiseMult(v1);
    for(int j=0;j<4;j++){
        cout<<v12[j]<<"  ";
    }
    cout<<endl;
    cout<<"v12.PointWiseDivide : ";
    v12.PointWiseDivide(v1,10e-4);
    for(int j=0;j<4;j++){
        cout<<v12[j]<<"  ";
    }
    cout<<endl;

    return 0;
}