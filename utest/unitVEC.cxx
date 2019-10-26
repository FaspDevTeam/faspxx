/** \file unitVEC.cxx
 *  \brief Unit test for VEC class implementations
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "../src/VEC.hxx"

DBL TOL = 1E-10;

VEC v0;
VEC v1(4, 2.312);

std::vector<DBL> v2{1.23, 0.98, 234.67, 456};
VEC v3(v2);
VEC v4(v3);
VEC v5(v4);

auto *p = new DBL[4]{452, 0.1234, 8964, 0.567};
VEC v6(4, p);

auto *q = new DBL[100] {85, 89 , 23, 39, 99 ,41 ,48 ,68 ,8  ,16, 24,
                        28 , 94, 93, 9  ,49 ,97 ,40 ,60 ,27,92, 100,
                        95, 5 , 81 ,14, 21, 22, 7  ,37,46, 71 , 34,
                        90, 33 ,54 ,29, 19, 31, 64,74, 12 , 58, 59,
                        83, 88, 15, 52, 10, 18, 66, 38 , 53, 57, 73,
                        1, 70, 67, 72, 78, 36, 47 , 51, 84, 76 ,45,
                        26, 43, 65, 42, 80, 56 , 91, 11, 30 ,82 ,62,
                        6, 87 ,13,32, 3 , 77, 4 , 61 ,35 ,50 ,69,
                        20,98, 86, 96, 75, 25, 79 ,17 ,2  ,55 ,44,63};
VEC v7(100, q);

/*---------------------------------*/
/*--     Beginning of TEST       --*/
/*---------------------------------*/

TEST(VEC_size, Size)
{
    INT zero, four;
    zero = 0;
    four = 4;
    EXPECT_EQ(v0.GetSize(), zero);
    EXPECT_EQ(v1.GetSize(), four);
    EXPECT_EQ(v5.GetSize(), four);
    EXPECT_EQ(v6.GetSize(), four);
}

TEST(VEC_copy_constructor, CopyConstructor)
{
    for (INT i=0; i<v3.GetSize(); i++) {
        EXPECT_EQ(v3[i], v2[i]);
        EXPECT_EQ(v4[i], v3[i]);
    }

    VEC v7 = v6;
    for (INT i=0; i<v6.GetSize(); i++) {
        EXPECT_EQ(v6[i], p[i]);
        EXPECT_NE(v6[i], p[i] + TOL);
        EXPECT_EQ(v6[i], v7[i]);
    }
}

TEST(VEC_operator_plus, Plus)
{
    VEC v8 = v5; // TODO: VEC v8; v8 += v5 should do default initialization
    v8 += v5;
    for (INT i=0; i<v8.GetSize(); i++) {
        EXPECT_EQ(v8[i], 2*v5[i]);
        EXPECT_NE(v8[i], 2*v5[i]+TOL);
    }
}

TEST(VEC_operator_minus, Minus)
{
    VEC v9 = v5;
    v9 -= v5;
    for (INT i=0; i<v9.GetSize(); i++) {
        EXPECT_EQ(v9[i], 0);
        EXPECT_NE(v9[i], TOL);
    }
}

TEST(VEC_SetValue, SetValue)
{
    VEC v10, v11, v12;

    v10.SetValues(6, 3.14);
    for (INT i=0; i<v10.GetSize(); i++)
        EXPECT_EQ(v10[i], 3.14);

    v11.SetValues(v2);
    for (INT i=0; i<v11.GetSize(); i++)
        EXPECT_EQ(v11[i], v2[i]);

    v12.SetValues(4, p);
    for (INT i=0; i<v12.GetSize(); i++)
        EXPECT_EQ(v12[i], p[i]);
}

TEST(VEC_Get, GetValue)
{
    for (INT i=0; i<v6.GetSize(); i++)
        EXPECT_EQ(v6.GetValue(i), p[i]);
}

TEST(VEC_Scale, Scale)
{
    DBL scale = 2.365;
    INT *indexPtr=new INT[v5.GetSize()];
    for(size_t j=0;j<v5.GetSize();j++)
	    indexPtr[j]=j;
    DBL *array = new DBL[v5.GetSize()];
    v5.GetValues(v5.GetSize(),indexPtr, array);

    v5.Scale(scale);
    std::cout << v5.GetSize() << std::endl;
    for ( INT i=0; i<v5.GetSize(); i++ ) {
        EXPECT_EQ(v5.GetValue(i), scale*array[i]);
    }

    delete[] array;
}

TEST(VEC_Copy, Copy)
{
    VEC v;
    v6.CopyTo(v);
    for (INT i=0; i<v6.GetSize(); i++)
        EXPECT_EQ(v[i], v6[i]);
}

TEST(VEC_Min, Min)
{
    DBL min = v6.Min();
    EXPECT_NE(min, 0.1234+TOL);
    EXPECT_EQ(min, 0.1234);
}

TEST(VEC_Max, Max)
{
    DBL max = v6.Max();
    EXPECT_NE(max, 8964+TOL);
    EXPECT_EQ(max, 8964);
}

TEST(VEC_Shift, Shift)
{
    DBL shift = 3.14159;
    v6.Shift(shift);
    for (INT i=0; i<v6.GetSize(); i++)
        EXPECT_EQ(v6[i], p[i]+shift);
}

TEST(VEC_Abs, Abs)
{
    VEC v = v7;
    VEC vv(v);
    for (INT i=0; i<v.GetSize(); i++) {
        v[i] *= pow(-1.0, i%2);
    }

    v.Abs();
    for (INT i=0; i<v.GetSize(); i++) {
        EXPECT_EQ(v[i], vv[i]);
    }
}

TEST(VEC_Reciprocal, Reciprocal)
{
    VEC v = v7;
    VEC vv(v);
    for (INT i=0; i<v.GetSize(); i++) {
        v[i] = 1.0 / v[i];
    }

    v.Reciprocal();
    for (INT i=0; i<v.GetSize(); i++) {
        EXPECT_DOUBLE_EQ(v[i], vv[i]);
        EXPECT_NEAR(v[i], vv[i], TOL);
    }
}

TEST(VEC_Norm2, Norm2)
{
    VEC v=v7;
    DBL norm2 = 0.0;
    for (INT i=0; i<v.GetSize(); i++) {
        norm2 += v[i]*v[i];
    }
    norm2 = sqrt(norm2);

    EXPECT_DOUBLE_EQ(norm2, v.Norm2());
}

TEST(VEC_Add, Add)
{
    VEC v=v7;
    VEC v1(v), v2(v), v3(v), v4(v.GetSize());
    const DBL a = 1.0;
    const DBL b = 3.14159;

    for (INT i=0; i<v.GetSize(); i++) v[i] = a*v1[i] + b*v[i];

    v2.AXPBY(a, b, v1);

    v4.WAXPBY(a, v1, b, v3);

    for (INT i=0; i<v.GetSize(); i++) {
        EXPECT_EQ(v[i], v2[i]);
        EXPECT_NE(v[i], v2[i]+TOL);
        EXPECT_EQ(v2[i], v4[i]);
        EXPECT_NE(v2[i], v4[i]+TOL);
    }
}

TEST(VEC_Dot, Dot) {
    VEC v=v7;
    DBL dot1 = 0.0;
    for (INT i = 0; i < v.GetSize(); i++) {
        dot1 += v[i] * v[i];
    }

    DBL dot2 = v.Dot(v);
    EXPECT_EQ(dot1, dot2);
    EXPECT_NE(dot1, dot2+TOL);
}

TEST(VEC_PoINTwiseMult, PoINTwiseMult)
{
    VEC v1 = v7;
    VEC v2 = v7;
    VEC v3(v1);
    for (INT i=0; i<v1.GetSize(); i++) {
        v3[i] = v1[i] * v2[i];
    }

    v1.PointwiseMult(v2);
    for (INT i=0; i<v1.GetSize(); i++) {
        EXPECT_EQ(v1[i], v3[i]);
    }
}

TEST(VEC_PoINTwiseDivide, PoINTwiseDivide)
{
    VEC v1 = v7;
    VEC v2 = v7;
    VEC v3(v1);
    for (INT i=0; i<v1.GetSize(); i++) {
        v3[i] = v1[i] / v2[i];
    }

    v1.PointwiseDivide(v2);
    for (INT i=0; i<v1.GetSize(); i++) {
        EXPECT_EQ(v1[i], v3[i]);
    }
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
