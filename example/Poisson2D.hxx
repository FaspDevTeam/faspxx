/*! \file Poisson2D.hxx
 *  \brief Test problems for 2D Poisson's equation
 *  \author Kailei Zhang, Chensong Zhang
 *  \date Nov/05/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __POISSON2D_HXX__      /*-- allow multiple inclusions --*/
#define __POISSON2D_HXX__      /**< indicate Poisson2D.hxx has been included before */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

#define TEST_PROBLEM 3
//---------------------------------
// List of test problems:
// 1 :  u(x,y) = sin(x) * sin(y)
// 2 :  u(x,y) = sin(x*y)
// 3 :  u(x,y) = x**2 + y**2
//---------------------------------

/// \brief The exact solution
double ExactSolu(const double x, const double y) {
#if TEST_PROBLEM == 1
    return sin(M_PI*x)*sin(M_PI*y);
#endif
#if TEST_PROBLEM == 2
    return sin(x * y);
#endif
#if TEST_PROBLEM == 3
    return x * x + y * y;
#endif
}

/// \brief Right-hand side function
double Load(const double x, const double y) {
#if TEST_PROBLEM == 1
    return 2*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y);
#endif
#if TEST_PROBLEM == 2
    return (x * x + y * y) * sin(x * y);
#endif
#if TEST_PROBLEM == 3
    return -4;
#endif
}

/// \brief Left boundary condition
double LeftBdyCond(const double x, const double y) {
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return 0.0;
#endif
#if TEST_PROBLEM == 3
    return y * y;
#endif
}

/// \brief Right boundary condition
double RightBdyCond(const double x, const double y) {
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return sin(y);
#endif
#if TEST_PROBLEM == 3
    return 1 + y * y;
#endif
}

/// \brief Lower boundary condition
double LowerBdyCond(const double x, const  double y) {
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return 0.0;
#endif
#if TEST_PROBLEM == 3
    return x * x;
#endif
}

/// \brief Upper boundary condition
double UpperBdyCond(const double x, const double y) {
#if TEST_PROBLEM == 1
    return 0.0;
#endif
#if TEST_PROBLEM == 2
    return sin(x);
#endif
#if TEST_PROBLEM == 3
    return 1 + x * x;
#endif
}

#endif /* end if for __POISSON2D_HXX__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/