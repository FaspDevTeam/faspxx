#include <gtest/gtest.h>
#include <iostream>

using namespace std;

bool Positive(int a)
{
    return a > 0;
}

class IsPositiveTest: public testing::TestWithParam<int>
{
public:
    IsPositiveTest() {
        num = GetParam();
    }

protected:
    int num;
};

TEST_P(IsPositiveTest, ParamTest)
{
    int n = GetParam();
    EXPECT_TRUE(Positive(n));
}
INSTANTIATE_TEST_CASE_P(ReturnTrue, IsPositiveTest, testing::Values(1,2,3,4,5,6));

TEST(ExplicitTest, Demo)
{
    ADD_FAILURE() << "Sorry";
    SUCCEED();
    cout << "------------------------- After FAIL()  stop. ------------------------\n\n" << endl;
}

int Add(int a, int b)
{
    return a + b;
}

TEST(AddTest, SimpleAdd)
{
    //ref: https://www.cnblogs.com/coderzh/archive/2009/04/06/1430364.html
    ASSERT_EQ(3, Add(1, 2)) << "In AddTest -> SimpleAdd error occur.";
    //EXPECT_EQ(4, Add(1, 2)) << "In AddTest -> SimpleAdd error occure.";
    cout << "------------------------- After ASSERT_EQ() stop. ------------------------\n\n" << endl;
}

int maxCommonFactor(int a, int b)
{
    // Find max common factor
    if (a == 0 || b == 0) throw "don't do that";
    int c = a % b;
    if (c == 0) return b;
    return maxCommonFactor(b, c);
}

TEST(maxCommonFactorTest, HandleNoneZeroInput)
{
    //ref: https://www.cnblogs.com/coderzh/archive/2009/03/31/1426758.html
    EXPECT_EQ(2, maxCommonFactor(4, 10));
    EXPECT_EQ(6, maxCommonFactor(30, 18));
}
