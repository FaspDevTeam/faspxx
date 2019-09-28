
#include <gtest/gtest.h>
#include <vector>

#include "../src/MAT.hxx"


MAT mat0;

/*
 * \[
 * \begin{pmatrix}
 * 0 & 0 & 1 & 0 \\
 * 2 & 3 & 0 & 4 \\
 * 5 & 0 & 2 & 1 \\
 * 0 & 0 & 2 & 1 \\
 * \end{pmatrix}
 * \]
 * */
const std::vector<DBL> values1   = {0,1,2,3,4,5,2,1,2,1};
const std::vector<INT> colindex1 = {0,2,0,1,3,0,2,3,2,3};
const std::vector<INT> rowshift1 = {0,2,5,8,10};
const std::vector<INT> diag1 = {0,3,6,9};
const MAT mat1(4, 4, 10, values1, colindex1, rowshift1, diag1);

// Sparse structure
const MAT mat2(4, 4, 10, colindex1, rowshift1, diag1);

const std::vector<DBL> diag2 = {0, 1, 2, 3, 4, 5, 2, 1, 2, 1};
const MAT mat3(diag2);

const MAT mat4(mat3);

const MAT mat5(mat1);


TEST(MAT_SIZE, SIZE)
{
    EXPECT_EQ(mat0.GetRowSize(), 0);
    EXPECT_EQ(mat0.GetColSize(), 0);
    EXPECT_EQ(mat0.GetNNZ(), 0);

    EXPECT_EQ(mat1.GetRowSize(), 4);
    EXPECT_EQ(mat1.GetColSize(), 4);
    EXPECT_EQ(mat1.GetNNZ(), 10);

    EXPECT_EQ(mat2.GetRowSize(), 4);
    EXPECT_EQ(mat2.GetColSize(), 4);
    EXPECT_EQ(mat2.GetNNZ(), 10);
}


TEST(MAT_constructor, constructor)
{
    for (int i=0; i<mat3.GetColSize(); i++)
        for (int j=0; j<mat3.GetColSize(); j++)
        {
            if (i == j)
            {
                EXPECT_EQ(mat3.GetValue(i, j), diag2[i]);
                EXPECT_EQ(mat3.GetValue(i, j), mat4.GetValue(i, j));
            }
        }

    for (int i=0; i<mat5.GetRowSize(); i++)
        for (int j=0; j<mat5.GetColSize(); j++)
            EXPECT_EQ(mat5.GetValue(i, j), mat1.GetValue(i, j));
}


TEST(MAT_GetValue, GetValue)
{
    for (int i=0; i<mat1.GetRowSize(); i++)
    {
        for (int j=0; j<mat1.GetColSize(); j++)
        {
            if (i == 0 && j == 2)
                EXPECT_EQ(mat1.GetValue(i, j), 1);
            else if (i == 1 && j == 0)
                EXPECT_EQ(mat1.GetValue(i, j), 2);
            else if (i == 1 && j == 1)
                EXPECT_EQ(mat1.GetValue(i, j), 3);
            else if (i == 1 && j == 3)
                EXPECT_EQ(mat1.GetValue(i, j), 4);
            else if (i == 2 && j == 0)
                EXPECT_EQ(mat1.GetValue(i, j), 5);
            else if (i == 2 && j == 2)
                EXPECT_EQ(mat1.GetValue(i, j), 2);
            else if (i == 2 && j == 3)
                EXPECT_EQ(mat1.GetValue(i, j), 1);
            else if (i == 3 && j == 2)
                EXPECT_EQ(mat1.GetValue(i, j), 2);
            else if (i == 3 && j == 3)
                EXPECT_EQ(mat1.GetValue(i, j), 1);
            else
                EXPECT_EQ(mat1.GetValue(i, j), 0);
        }
    }

    for (int i=0; i<mat2.GetRowSize(); i++)
    {
        for (int j=0; j<mat2.GetColSize(); j++)
        {
            ASSERT_TRUE(mat2.GetValue(i, j) == 0 || mat2.GetValue(i, j) == 1);
        }
    }
}


TEST(MAT_GetRow, GetRow)
{
    std::vector<DBL> vec;
    mat1.GetRow(0, vec);
    for (int i=0; i<mat1.GetRowSize(); i++)
    {
        std::cout << mat1.GetValue(0, i) << ", " << vec[i] << std::endl;
        EXPECT_EQ(mat1.GetValue(0, i), vec[i]); // TODO: bug?
    }
}

