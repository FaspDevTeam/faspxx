#include <cmath>
#include <vector>

#include "VEC.hxx"

#include "../catch.hxx"

TEST_CASE("VEC")
{
    DBL TOL = 1E-10;

    VEC v0;
    VEC v1(4, 2.312);

    std::vector<DBL> v2{1.23, 0.98, 234.67, 456};
    VEC              v3(v2);
    VEC              v4(v3);
    VEC              v5(v4);

    auto* p = new DBL[4]{452, 0.1234, 8964, 0.567};
    VEC   v6(4, p);

    auto* q = new DBL[100]{
        85, 89,  23, 39, 99, 41, 48, 68, 8,  16, 24, 28, 94, 93, 9,  49, 97, 40, 60, 27,
        92, 100, 95, 5,  81, 14, 21, 22, 7,  37, 46, 71, 34, 90, 33, 54, 29, 19, 31, 64,
        74, 12,  58, 59, 83, 88, 15, 52, 10, 18, 66, 38, 53, 57, 73, 1,  70, 67, 72, 78,
        36, 47,  51, 84, 76, 45, 26, 43, 65, 42, 80, 56, 91, 11, 30, 82, 62, 6,  87, 13,
        32, 3,   77, 4,  61, 35, 50, 69, 20, 98, 86, 96, 75, 25, 79, 17, 2,  55, 44, 63};
    VEC v7(100, q);

    SECTION("VEC: GetSize()")
    {
        std::cout << "TEST VEC::GetSize()" << std::endl;

        USI zero, four;
        zero = 0;
        four = 4;
        REQUIRE(v0.GetSize() == zero);
        REQUIRE(v1.GetSize() == four);
        REQUIRE(v5.GetSize() == four);
        REQUIRE(v6.GetSize() == four);
    }

    SECTION("VEC: copy constructor VEC()")
    {
        std::cout << "TEST VEC::VEC(const VEC& src)" << std::endl;

        for (USI i = 0; i < v3.GetSize(); i++) {
            REQUIRE(v3[i] == v2[i]);
            REQUIRE(v4[i] == v3[i]);
        }

        VEC v7 = v6;
        for (USI i = 0; i < v6.GetSize(); i++) {
            REQUIRE(v6[i] == p[i]);
            REQUIRE(std::abs(v6[i] - p[i]) < TOL);
            REQUIRE(v6[i] == v7[i]);
        }
    }

    SECTION("VEC: operator +=")
    {
        std::cout << "TEST VEC:operator+=()" << std::endl;

        VEC v8 = v5;
        v8 += v5;
        for (USI i = 0; i < v8.GetSize(); i++) {
            REQUIRE(std::abs(v8[i] - 2 * v5[i]) < TOL);
            REQUIRE(std::abs(v8[i] - 2 * v5[i]) < TOL);
        }
    }

    SECTION("VEC: operator -=")
    {
        std::cout << "TEST VEC::operator-=()" << std::endl;

        VEC v9 = v5;
        v9 -= v5;
        for (USI i = 0; i < v9.GetSize(); i++) {
            REQUIRE(std::abs(v9[i] - 0) < TOL);
        }
    }

    SECTION("VEC: SetValues()")
    {
        std::cout << "TEST VEC::SetValues()" << std::endl;

        VEC v10, v11, v12;

        v10.SetValues(6, 3.14);
        for (USI i = 0; i < v10.GetSize(); i++)
            REQUIRE(v10[i] == 3.14);

        v11.SetValues(v2);
        for (USI i = 0; i < v11.GetSize(); i++)
            REQUIRE(v11[i] == v2[i]);

        v12.SetValues(4, p);
        for (USI i = 0; i < v12.GetSize(); i++)
            REQUIRE(v12[i] == p[i]);
    }

    SECTION("VEC: GetValue()")
    {
        std::cout << "TEST VEC::GetValue()" << std::endl;

        for (USI i = 0; i < v6.GetSize(); i++)
            REQUIRE(v6.GetValue(i) == p[i]);
    }

    SECTION("VEC: Scale()")
    {
        std::cout << "TEST VEC::Scale()" << std::endl;

        DBL  scale    = 2.365;
        USI* indexPtr = new USI[v5.GetSize()];
        for (size_t j = 0; j < v5.GetSize(); j++) indexPtr[j] = j;
        DBL* array = new DBL[v5.GetSize()];
        v5.GetValues(v5.GetSize(), indexPtr, array);

        v5.Scale(scale);
        for (USI i = 0; i < v5.GetSize(); i++) {
            REQUIRE(v5.GetValue(i) == scale * array[i]);
        }

        delete[] array;
    }

    SECTION("VEC: CopyTo()")
    {
        std::cout << "TEST VEC::CopyTo()" << std::endl;

        VEC v;
        v6.CopyTo(v);
        for (USI i = 0; i < v6.GetSize(); i++)
            REQUIRE(v[i] == v6[i]);
    }

    SECTION("VEC: Min()")
    {
        std::cout << "TEST VEC::Min()" << std::endl;

        DBL min = v6.Min();
        REQUIRE(min != 0.1234 + TOL);
        REQUIRE(min == 0.1234);
    }

    SECTION("VEC: Max()")
    {
        std::cout << "TEST VEC::Max()" << std::endl;

        DBL max = v6.Max();
        REQUIRE(max != 8964 + TOL);
        REQUIRE(max == 8964);
    }

    SECTION("VEC: Shift()")
    {
        std::cout << "TEST VEC::Shift()" << std::endl;

        DBL shift = 3.14159;
        v6.Shift(shift);
        for (USI i = 0; i < v6.GetSize(); i++)
            REQUIRE(v6[i] == p[i] + shift);
    }

    SECTION("VEC: Abs()")
    {
        std::cout << "TEST VEC::Abs()" << std::endl;

        VEC v = v7;
        VEC vv(v);
        for (USI i = 0; i < v.GetSize(); i++) {
            v[i] *= pow(-1.0, i % 2);
        }

        v.Abs();
        for (USI i = 0; i < v.GetSize(); i++) {
            REQUIRE(v[i] == vv[i]);
        }
    }

    SECTION("VEC: Reciprocal()")
    {
        std::cout << "TEST VEC::Reciprocal()" << std::endl;

        VEC v = v7;
        VEC vv(v);
        for (USI i = 0; i < v.GetSize(); i++) {
            v[i] = 1.0 / v[i];
        }

        v.Reciprocal();
        for (USI i = 0; i < v.GetSize(); i++) {
            REQUIRE(std::abs(v[i] - vv[i]) < TOL);
        }
    }

    SECTION("VEC: Norm2()")
    {
        std::cout << "TEST VEC::Norm2()" << std::endl;

        VEC v     = v7;
        DBL norm2 = 0.0;
        for (USI i = 0; i < v.GetSize(); i++) {
            norm2 += v[i] * v[i];
        }
        norm2 = sqrt(norm2);

        REQUIRE(std::abs(norm2 - v.Norm2()) < TOL);
    }

    SECTION("VEC: Add()")
    {
        std::cout << "TEST VEC::Add()" << std::endl;

        VEC       v = v7;
        VEC       v1(v), v2(v), v3(v), v4(v.GetSize());
        const DBL a = 1.0;
        const DBL b = 3.14159;

        for (USI i = 0; i < v.GetSize(); i++) v[i] = a * v1[i] + b * v[i];

        v2.AXPBY(a, b, v1);

        v4.WAXPBY(a, v1, b, v3);

        for (USI i = 0; i < v.GetSize(); i++) {
            REQUIRE(std::abs(v[i] - v2[i]) < TOL);
            REQUIRE(std::abs(v2[i] - v4[i]) < TOL);
        }
    }

    SECTION("VEC: Dot()")
    {
        std::cout << "TEST VEC::Dot()" << std::endl;

        VEC v    = v7;
        DBL dot1 = 0.0;
        for (USI i = 0; i < v.GetSize(); i++) {
            dot1 += v[i] * v[i];
        }

        DBL dot2 = v.Dot(v);
        REQUIRE(dot1 == dot2);
        REQUIRE(std::abs(dot1 - dot2) < TOL);
    }

    SECTION("VEC: PointwiseMult()")
    {
        std::cout << "TEST VEC::PointwiseMult()" << std::endl;

        VEC v1 = v7;
        VEC v2 = v7;
        VEC v3(v1);
        for (USI i = 0; i < v1.GetSize(); i++) {
            v3[i] = v1[i] * v2[i];
        }

        v1.PointwiseMult(v2);
        for (USI i = 0; i < v1.GetSize(); i++) {
            REQUIRE(v1[i] == v3[i]);
        }
    }

    SECTION("VEC: PointwiseDivide()")
    {
        std::cout << "TEST VEC::PointwiseDivide()" << std::endl;

        VEC v1 = v7;
        VEC v2 = v7;
        VEC v3(v1);
        for (USI i = 0; i < v1.GetSize(); i++) {
            v3[i] = v1[i] / v2[i];
        }

        v1.PointwiseDivide(v2);
        for (USI i = 0; i < v1.GetSize(); i++) {
            REQUIRE(v1[i] == v3[i]);
        }
    }
}