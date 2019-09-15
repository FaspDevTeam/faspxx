/** \file unitVEC.cxx
 *  \brief Unit test for VEC class implementations
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "vec.hxx"

double TOL = 1E-10;

VEC v0;
VEC v1(4, 2.312);

std::vector<double> v2{1.23, 0.98, 234.67, 456};
VEC v3(v2);
VEC v4(v3);

VEC v5(21, 34, 6);

auto *p = new double[4]{452, 0.1234, 8964, 0.567};
VEC v6(4, p);

/*---------------------------------*/
/*--     Beginning of TEST       --*/
/*---------------------------------*/

TEST(VEC_size, Size)
{
    EXPECT_EQ(v0.GetSize(), 0);
    EXPECT_EQ(v1.GetSize(), 4);
    EXPECT_EQ(v5.GetSize(), 21);
    EXPECT_EQ(v6.GetSize(), 4);
}

TEST(VEC_copy_constructor, CopyConstructor)
{
    for (int i=0; i<v3.GetSize(); i++) {
        EXPECT_EQ(v3[i], v2[i]);
        EXPECT_EQ(v4[i], v3[i]);
    }

    VEC v7 = v6;
    for (int i=0; i<v6.GetSize(); i++) {
        EXPECT_EQ(v6[i], p[i]);
        EXPECT_NE(v6[i], p[i] + TOL);

        EXPECT_EQ(v6[i], v7[i]);
    }
}

TEST(VEC_operator_plus, Plus)
{
    VEC v8 = v5; //fff VEC v8; v8 += v5 最好要发出警告或者默认初始化v8
    v8 += v5;
    for (int i=0; i<v8.GetSize(); i++) {
        EXPECT_EQ(v8[i], 2*v5[i]);
        EXPECT_NE(v8[i], 2*v5[i]+TOL);
    }
}

TEST(VEC_operator_minus, Minus)
{
    VEC v9 = v5;
    v9 -= v5;
    for (int i=0; i<v9.GetSize(); i++) {
        EXPECT_EQ(v9[i], 0);
        EXPECT_NE(v9[i], TOL);
    }
}

TEST(VEC_SetValue, SetValue)
{
    VEC v10, v11, v12;

    v10.SetValues(6, 3.14);
    for (int i=0; i<v10.GetSize(); i++)
        EXPECT_EQ(v10[i], 3.14);

    v11.SetValues(v2);
    for (int i=0; i<v11.GetSize(); i++)
        EXPECT_EQ(v11[i], v2[i]);

    v12.SetValues(4, p);
    for (int i=0; i<v12.GetSize(); i++)
        EXPECT_EQ(v12[i], p[i]);

}

TEST(VEC_Get, Get)
{
    for (int i=0; i<v6.GetSize(); i++)
        EXPECT_EQ(v6.Get(i), p[i]);
}

TEST(VEC_Scale, Scale)
{
    double scale = 1.02365;
    DBL *array;
    v5.GetArray(v5.GetSize(), &array);
    v5.Scale(scale);
    for (int i=0; i<v5.GetSize(); i++)
        EXPECT_EQ(v5.Get(i), scale*array[i]);
}

TEST(VEC_Copy, Copy)
{
    VEC v;
    v6.Copy(v);
    for (int i=0; i<v6.GetSize(); i++)
        EXPECT_EQ(v[i], v6[i]);
}

TEST(VEC_Min, Min)
{
    DBL min;
    v6.Min(min); //fff最好是Min(), 而不是Min(DBL&)
    EXPECT_NE(min, 0.1234+TOL);
    EXPECT_EQ(min, 0.1234);
}

TEST(VEC_Max, Max)
{
    DBL max;
    v6.Max(max); //fff最好是Max(), 而不是Max(DBL&)
    EXPECT_NE(max, 8964+TOL);
    EXPECT_EQ(max, 8964);
}

TEST(VEC_Shift, Shift)
{
    DBL shift = 3.14159;
    v6.Shift(shift);
    for (int i=0; i<v6.GetSize(); i++)
        EXPECT_EQ(v6[i], p[i]+shift);
}

TEST(VEC_Abs, Abs)
{
    VEC v(100, 1, 100);
    VEC vv(v);
    for (int i=0; i<v.GetSize(); i++) {
        v[i] *= pow(-1.0, i%2);
    }

    v.Abs();
    for (int i=0; i<v.GetSize(); i++) {
        EXPECT_EQ(v[i], vv[i]);
    }
}

TEST(VEC_Reciprocal, Reciprocal)
{
    VEC v(100, 1, 100);
    VEC vv(v);
    for (int i=0; i<v.GetSize(); i++) {
        v[i] = 1.0 / v[i];
    }

    v.Reciprocal(TOL);
    for (int i=0; i<v.GetSize(); i++) {
        EXPECT_DOUBLE_EQ(v[i], vv[i]);
        EXPECT_NEAR(v[i], vv[i], TOL);
    }
}

TEST(VEC_Norm2, Norm2)
{
    VEC v(100, 1, 100);
    double norm2 = 0.0;
    for (int i=0; i<v.GetSize(); i++) {
        norm2 += v[i]*v[i];
    }
    norm2 = sqrt(norm2);

    EXPECT_DOUBLE_EQ(norm2, v.Norm2());
}

TEST(VEC_Add, Add)
{
    VEC v(100, 1, 100);
    VEC v1(v), v2(v), v3(v), v4;
    double a = 3.14159;
    double b = 3.14159;
    for (int i=0; i<v.GetSize(); i++) {
        v[i] = a*v1[i] + b*v[i];
    }

    v2.Add(v1, b, a);
    v4.Add(a, v1, b, v3);
    for (int i=0; i<v.GetSize(); i++) {
        EXPECT_EQ(v[i], v2[i]);
        EXPECT_NE(v[i], v2[i]+TOL);
        EXPECT_EQ(v2[i], v4[i]);
        EXPECT_NE(v2[i], v4[i]+TOL);
    }
}

TEST(VEC_Dot, Dot) {
    VEC v(100, 1, 100);
    DBL dot1 = 0.0;
    for (int i = 0; i < v.GetSize(); i++) {
        dot1 += v[i] * v[i];
    }

    DBL dot2;
    v.Dot(v, dot2);
    EXPECT_EQ(dot1, dot2);
    EXPECT_NE(dot1, dot2+TOL);
}

TEST(VEC_PointwiseMult, PointwiseMult)
{
    VEC v1(100, 1, 100);
    VEC v2(100, 1, 100);
    VEC v3(v1);
    for (int i=0; i<v1.GetSize(); i++) {
        v3[i] = v1[i] * v2[i];
    }

    v1.PointwiseMult(v2);
    for (int i=0; i<v1.GetSize(); i++) {
        EXPECT_EQ(v1[i], v3[i]);
    }
}

TEST(VEC_PointwiseDivide, PointwiseDivide)
{
    VEC v1(100, 1, 100);
    VEC v2(100, 1, 100); // v1==v2, 没有正真的随机, 随机数生成器要修改
    VEC v3(v1);
    for (int i=0; i<v1.GetSize(); i++) {
        v3[i] = v1[i] / v2[i];
    }

    v1.PointwiseDivide(v2, TOL);
    for (int i=0; i<v1.GetSize(); i++) {
        EXPECT_EQ(v1[i], v3[i]);
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/