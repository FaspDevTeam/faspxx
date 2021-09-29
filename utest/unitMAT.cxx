/** \file    unitMAT.cxx
 *  \brief   Unit test for MAT class implementations
 *  \author  Ronghong Fan
 *  \date    Oct/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Standard header files
#include <gtest/gtest.h>
#include <vector>

// FASPXX header files
#include "MAT.hxx"
#include "VEC.hxx"

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
 * \[
 * \begin{pmatrix}
 * 0.00000 & -0.00000 & 0.20000 & -0.20000 \\
 * 2.66667 & 0.33333 & -0.13333 & -1.20000 \\
 * 1.00000 & 0.00000 & 0.00000 & 0.00000 \\
 * -2.00000 & 0.00000 & 0.00000 & 1.00000 \\
 * \end{pmatrix}
 * \]
 * */
const std::vector<DBL> values1  = {0, 1, 2, 3, 4, 5, 2, 1, 2, 1};
const std::vector<USI> colInd1  = {0, 2, 0, 1, 3, 0, 2, 3, 2, 3};
const std::vector<USI> rowPtr1  = {0, 2, 5, 8, 10};
const std::vector<USI> diagPtr1 = {0, 3, 6, 9};
const MAT              mat1(4, 4, 10, values1, colInd1, rowPtr1, diagPtr1);

const MAT mat2(4, 4, 10, colInd1, rowPtr1, diagPtr1);

const std::vector<DBL> diag2 = {0, 1, 2, 3, 4, 5, 2, 1, 2, 1};
const MAT              mat3(diag2);

const MAT mat4(mat3);
const MAT mat5(mat1);

const VEC vec1(diag2);
const MAT mat6(vec1);

const std::vector<DBL> values2 = {0,     2.87, 5.74, 8.61, 11.48,
                                  14.35, 5.74, 2.87, 5.74, 2.87};
const MAT              mat7(4, 4, 10, values2, colInd1, rowPtr1,
               diagPtr1); // scale by mat1 with 2.87

const VEC vec2(4, 0.314);

const double* p1 = new double[4]{0.314, 2.826, 2.512, 0.942};
const VEC     vec3(4, p1); // mat1 * vec2 == vec3

const std::vector<DBL> valuesInv1 = {2.0, 2.0};
const std::vector<DBL> valuesInv2 = {0.5, 0.5};

const std::vector<USI> colIndInv  = {0, 1};
const std::vector<USI> rowPtrInv  = {0, 1, 2};
const std::vector<USI> diagPtrInv = {0, 1, 2};
const MAT              invmat1(2, 2, 2, valuesInv1, colIndInv, rowPtrInv, diagPtrInv);
const MAT              invmat2(2, 2, 2, valuesInv2, colIndInv, rowPtrInv, diagPtrInv);

/*---------------------------------*/
/*--     Beginning of TEST       --*/
/*---------------------------------*/

TEST(MAT_SIZE, SIZE)
{
    USI zero, four, ten;
    zero = 0;
    four = 4;
    ten  = 10;
    EXPECT_EQ(mat0.GetRowSize(), zero);
    EXPECT_EQ(mat0.GetColSize(), zero);
    EXPECT_EQ(mat0.GetNNZ(), zero);

    EXPECT_EQ(mat1.GetRowSize(), four);
    EXPECT_EQ(mat1.GetColSize(), four);
    EXPECT_EQ(mat1.GetNNZ(), ten);

    EXPECT_EQ(mat2.GetRowSize(), four);
    EXPECT_EQ(mat2.GetColSize(), four);
    EXPECT_EQ(mat2.GetNNZ(), ten);
}

TEST(MAT_constructor, constructor)
{
    for (USI i = 0; i < mat3.GetColSize(); i++)
        for (USI j = 0; j < mat3.GetColSize(); j++) {
            if (i == j) {
                EXPECT_EQ(mat3.GetValue(i, j), diag2[i]);
                EXPECT_EQ(mat3.GetValue(i, j), mat4.GetValue(i, j));
            }
        }

    for (USI i = 0; i < mat5.GetRowSize(); i++)
        for (USI j = 0; j < mat5.GetColSize(); j++)
            EXPECT_EQ(mat5.GetValue(i, j), mat1.GetValue(i, j));
}

TEST(MAT_GetValue, GetValue)
{
    for (USI i = 0; i < mat1.GetRowSize(); i++) {
        for (USI j = 0; j < mat1.GetColSize(); j++) {
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

    for (USI i = 0; i < mat2.GetRowSize(); i++) {
        for (USI j = 0; j < mat2.GetColSize(); j++) {
            ASSERT_TRUE(mat2.GetValue(i, j) == 0 || mat2.GetValue(i, j) == 1);
        }
    }
}

TEST(MAT_MultVec, MultVec)
{
    VEC r(vec2.GetSize());
    mat1.Apply(vec2, r);
    for (USI i = 0; i < r.GetSize(); i++) {
        EXPECT_DOUBLE_EQ(r[i], vec3[i]);
    }
}

TEST(MAT_operator_equal, operator_equal)
{
    MAT mat;
    mat = mat5;
    for (USI i = 0; i < mat.GetRowSize(); i++)
        for (USI j = 0; j < mat.GetColSize(); j++)
            EXPECT_EQ(mat.GetValue(i, j), mat5.GetValue(i, j));

    mat = mat6;
    for (USI i = 0; i < mat.GetRowSize(); i++)
        for (USI j = 0; j < mat.GetColSize(); j++)
            EXPECT_EQ(mat.GetValue(i, j), mat6.GetValue(i, j));
}

TEST(MAT_Zero, Zero)
{
    MAT mat(mat1);
    mat.Zero();
    for (USI i = 0; i < mat.GetRowSize(); i++)
        for (USI j = 0; j < mat.GetColSize(); j++) EXPECT_EQ(mat.GetValue(i, j), 0);
}

TEST(MAT_CopyTo, CopyTo)
{
    MAT mat;
    mat1.CopyTo(mat);
    for (USI i = 0; i < mat.GetRowSize(); i++)
        for (USI j = 0; j < mat.GetColSize(); j++)
            EXPECT_EQ(mat.GetValue(i, j), mat1.GetValue(i, j));
}

TEST(MAT_Scale, Scale)
{
    MAT mat(mat1);
    mat.Scale(2.87);
    for (USI i = 0; i < mat.GetRowSize(); i++)
        for (USI j = 0; j < mat.GetColSize(); j++)
            EXPECT_DOUBLE_EQ(mat.GetValue(i, j), mat7.GetValue(i, j));
}

TEST(MAT_Inverse, Inverse)
{
    MAT invmat;
    invmat1.Inverse(invmat);
    for (USI i = 0; i < invmat.GetRowSize(); i++)
        for (USI j = 0; j < invmat.GetColSize(); j++)
            EXPECT_NEAR(invmat.GetValue(i, j), invmat2.GetValue(i, j), 1e-5);
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
