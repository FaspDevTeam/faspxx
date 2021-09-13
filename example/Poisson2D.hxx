/*! \file    Poisson2D.hxx
 *  \brief   Test problems for 2D Poisson's equation
 *  \author  Kailei Zhang, Chensong Zhang
 *  \date    Nov/05/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __POISSON2D_HXX__ /*-- allow multiple inclusions --*/
#define __POISSON2D_HXX__ /**< indicate Poisson2D.hxx has been included before */

// Standard header files
#include <cmath>

//---------------------------------
// List of test problems:
// 1 :  u(x,y) = sin(x) * sin(y)
// 2 :  u(x,y) = sin(x*y)
// 4 :  u(x,y) = exp(x*y)
// 3 :  u(x,y) = x**2 + y**2
// 5 :  u(x,y) = x**20 + y**20
//---------------------------------
#define TEST_PROBLEM 1

/// The exact solution.
double ExactSolu(const double x, const double y)
{
#if TEST_PROBLEM == 1
    return sin(M_PI * x) * sin(M_PI * y);
#endif
#if TEST_PROBLEM == 2
    return sin(x * y);
#endif
#if TEST_PROBLEM == 3
    return exp(x * y);
#endif
#if TEST_PROBLEM == 4
    return x * x + y * y;
#endif
#if TEST_PROBLEM == 5
    return pow(x, 20) + pow(y, 20);
#endif
}

/// Right-hand side function.
double Load(const double x, const double y)
{
#if TEST_PROBLEM == 1
    return 2 * M_PI * M_PI * sin(M_PI * x) * sin(M_PI * y);
#endif
#if TEST_PROBLEM == 2
    return (x * x + y * y) * sin(x * y);
#endif
#if TEST_PROBLEM == 3
    return -(x * x + y * y) * exp(x * y);
#endif
#if TEST_PROBLEM == 4
    return -4.0;
#endif
#if TEST_PROBLEM == 5
    return -380 * pow(x, 18) - 380 * pow(y, 18);
#endif
}

/// Left boundary condition.
double LeftBdyCond(const double x, const double y)
{
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return 0.0;
#endif
#if TEST_PROBLEM == 3
    return 1.0;
#endif
#if TEST_PROBLEM == 4
    return y * y;
#endif
#if TEST_PROBLEM == 5
    return pow(y, 20);
#endif
}

/// Right boundary condition.
double RightBdyCond(const double x, const double y)
{
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return sin(y);
#endif
#if TEST_PROBLEM == 3
    return exp(y);
#endif
#if TEST_PROBLEM == 4
    return 1.0 + y * y;
#endif
#if TEST_PROBLEM == 5
    return 1.0 + pow(y, 20);
#endif
}

/// Lower boundary condition.
double LowerBdyCond(const double x, const double y)
{
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return 0.0;
#endif
#if TEST_PROBLEM == 3
    return 1.0;
#endif
#if TEST_PROBLEM == 4
    return x * x;
#endif
#if TEST_PROBLEM == 5
    return pow(x, 20);
#endif
}

/// Upper boundary condition.
double UpperBdyCond(const double x, const double y)
{
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return sin(x);
#endif
#if TEST_PROBLEM == 3
    return exp(x);
#endif
#if TEST_PROBLEM == 4
    return 1.0 + x * x;
#endif
#if TEST_PROBLEM == 5
    return 1.0 + pow(x, 20);
#endif
}

#endif /* end if for __POISSON2D_HXX__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/