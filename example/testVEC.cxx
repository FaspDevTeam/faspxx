/** \file testVEC.cxx
 *  \brief Test VEC class implementations
 *
 *  To compile this test, you may (after cmake) use
 *      $ make testVEC
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include "VEC.hxx"

using namespace std;

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main()
{
    /**
     * output :
     * v0.GetSize() : 0
     */
    VEC v0;
    cout << "v0.GetSize() : " << v0.GetSize() << endl;

    /**
     * output :
     * v1.GetSize() : 4
     * 2.312, 2.312, 2.312, 2.312
     */
    VEC v1(4,2.312);
    cout << "v1.GetSize() : " << v1.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v1[j] << "  ";
    cout<<endl;

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
    for ( int j=0; j<4; j++ ) cout << vector1[j] << "  ";
    cout << endl;

    VEC v2(vector1);
    cout << "v2.GetSize() : " << v2.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v2[j] << "  ";
    cout << endl;

    /**
     * output:
     * v3.GetSize() : 4
     * 1.23, 0.98, 234.67, 456
     */
    VEC v3(v2);
    cout << "v3.GetSize() : " << v3.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v3[j] << "  ";
    cout << endl;

    /**
     * output:
     * v5.GetSize() : 4
     * 452, 0.12354, 8964, 0.567
     */
    auto *p = new double[4];
    p[0]=452;
    p[1]=0.12354;
    p[2]=8964;
    p[3]=0.567;
    VEC v5(4,p);
    cout << "v5.GetSize() : " << v5.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v5[j] << "  ";
    cout << endl;

    /**
     * output :
     * v6.GetSize() : 4
     * 452, 0.12354, 8964, 0.567
     */
    VEC v6=v5;
    cout << "v6.GetSize() : " << v6.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v6[j] << "  ";
    cout << endl;

    /**
     * output :
     * v6.GetSize() : 4
     * 904, 0.24708, 17928, 1.134
     */
    v6+=v5;
    cout << "v6.GetSize() : " << v6.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v6[j] << "  ";
    cout << endl;

    /**
     * output :
     * 452, 0.12354, 8964, 0.567
     */
    v6-=v5;
    cout << "v6.GetSize() : " << v6.GetSize() << endl;
    for ( int j=0; j<4; j++ ) cout << v6[j] << "  ";
    cout << endl;

    /**
     * output :
     * v6.GetSize() : 10
     */
    v6.SetSize(10);
    cout << "v6.GetSize() : " << v6.GetSize() << endl;

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
    cout << "v7.GetSize() : " << v7.GetSize() << endl;
    for (int j=0; j<6; j++ ) cout << v7[j] << "  ";
    cout << endl;

    v8.SetValues(vector1);
    cout << "v8.GetSize() : " << v8.GetSize() << endl;
    for (int j=0; j<6; j++ ) cout << v8[j] << "  ";
    cout << endl;

    v10.SetValues(4,p);
    cout << "v10.GetSize() : " << v10.GetSize() << endl;
    for (int j=0; j<6; j++ ) cout << v10[j] << "  ";
    cout << endl;

    /**
     * output :
     * 8964
     */
    DBL value;
    value = v10.GetValue(2);
    cout << "v10.GetValuec() : " << value << endl;

    /**
     * output :
     * 452, 0.12354, 8964
     */
    DBL *array;
    v10.GetArray(3,&array);
    cout << "array : ";
    for ( int j=0; j<3; j++ ) cout << array[j] << "  ";
    cout << endl;

    //INT GetSize() const
    //INT GetCapacity() const
    /**
     * output :
     * v10.GetSize() : 4
     * v10.Capacity() : 4
     */
    cout << "v10.GetSize() : " << v10.GetSize() << endl;
    cout << "v10.GetCapacity() : " << v10.GetCapacity() << endl;

    /**
     * output :
     * v10.Scale() : 1297.24, 0.35456, 25726.7, 1.62729
     */
    v10.Scale(2.87);
    cout << "v10.Scale() : ";
    for ( int j=0; j<v10.GetSize(); j++ ) cout << v10[j] << "  ";
    cout<<endl;

    /**
     * output :
     * v10.CopyTo() : 1297.24, 0.35456, 25726.7, 1.62729
     */
    VEC v11;
    cout << "v10.CopyTo() : ";
    v10.CopyTo(v11);
    for (int j=0; j<v10.GetSize(); j++) cout << v11[j] << "  ";
    cout << endl;

    /**
     * output :
     * max : 25726.7
     * min : 0.35456
     */
    DBL max = v10.Max(), min = v10.Min();
    cout << "max : " << max << endl;
    cout << "min : " << min << endl;

    /**
     * output :
     * v10.Shift() : 2297.24  1000.35  26726.7  1001.63
     */
    v10.Shift(1000);
    cout << "v10.Shift() : ";
    for ( int j=0; j<v10.GetSize(); j++ ) cout << v10[j] << "  ";
    cout << endl;

    /**
     * output :
     * v10.Abs() : 2297.24  1000.35  26726.7  1001.63
     */
    v10.Abs();
    cout << "v10.Abs() : ";
    for ( int j=0; j<v10.GetSize(); j++ ) cout << v10[j] << "  ";
    cout << endl;

    //void Reciprocal(DBL tol)
    /**
     * output :
     * v10.GetSize() : 4
     * v10.Reciprocal() : 0.000435305, 0.000999646, 3.74158e-05, 0.000998375
     */
    cout << "v10.GetSize() : " << v10.GetSize() << endl;
    cout << "v10.Reciprocal() : ";
    v10.Reciprocal();
    for ( int j=0; j<v10.GetSize(); j++ ) cout << v10[j] << "  ";
    cout << endl;

    /**
     * output :
     * Norm2 : 0.00147883
     * NormInf : 0.000999646
     */
    cout << "Norm2 : " << v10.Norm2() << endl;
    cout << "NormInf : " << v10.NormInf() <<endl;

    /**
     * output :
     * v1.Add() : 1304.5, 7.61424, 25733.9, 8.88697
     * v12.Add() : 4498.41, 24.0187, 88782.5, 28.4097
     */
    cout << "v1.Add() : ";
    v1.Add(v5,3.14,2.87);
    for ( int j=0; j<4; j++ ) cout << v1[j] << "  ";
    cout << endl;

    VEC v12;
    cout << "v12.Add() : ";
    v12.Add(3.14,v1,0.89,v5);
    for ( int j=0; j<4; j++ ) cout << v12[j] << "  ";
    cout << endl;

    /**
     * output :
     * v12.Dot() : 2.29059e+09
     */
    DBL dot = v12.Dot(v1);
    cout << "v12.Dot() : ";
    cout << dot << endl;

    /**
     * output :
     * v12.PointWiseMult : 5.86817e+06, 182.884, 2.28472e+09, 252.476
     * v12.PointWiseDivide : 4498.41, 24.0187, 88782.5, 28.4097
     */
    cout << "v12.PointwiseMult : ";
    v12.PointwiseMult(v1);
    for ( int j=0; j<4; j++ ) cout<<v12[j]<<"  ";
    cout << endl;

    cout << "v12.PointwiseDivide : ";
    v12.PointwiseDivide(v1);
    for ( int j=0; j<4; j++ ) cout<<v12[j]<<"  ";
    cout << endl;

    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/