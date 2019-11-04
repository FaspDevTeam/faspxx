/**
 * Five-point stencil of solving the Poisson's equation
 */

#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>

// Todo: Give detailed comments for the most important example!
// Todo: Use appropriate spaces in "if", "for", function calls, etc.
// Todo: Use 1.0 for double, 1 for integer.

#define SINX_SINY 1
#define SIN_XY 0
#define POLY 0

// left boundary
double left(double x, double y) {
#if SIN_XY
    return 0.0;
#endif
#if POLY
    return y * y;
#endif
#if SINX_SINY
    return 0.0;
#endif
}

// right boundary
double right(double x, double y) {
#if SIN_XY
    return sin(y);
#endif
#if POLY
    return 1 + y * y;
#endif
#if SINX_SINY
    return 0.0;
#endif
}

// lower boundary
double lower(double x, double y) {
#if SIN_XY
    return 0.0;
#endif
#if POLY
    return x * x;
#endif
#if SINX_SINY
    return 0.0;
#endif
}

// upper boundary
double upper(double x, double y) {
#if SIN_XY
    return sin(x);
#endif
#if POLY
    return 1 + x * x;
#endif
#if SINX_SINY
    return 0.0;
#endif
}

// -\Delta u = f
double f(double x, double y) {
#if SIN_XY
    return (x * x + y * y) * sin(x * y);
#endif
#if POLY
    return -4;
#endif
#if SINX_SINY
    return 2*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y);
#endif
}

// exact solution
double exact_solu(double x, double y) {
#if SIN_XY
    return sin(x * y);
#endif
#if POLY
    return x * x + y * y;
#endif
#if SINX_SINY
    return sin(M_PI*x)*sin(M_PI*y);
#endif
}