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

// left boundary
double left(double x, double y) {
    return 0.0;
}

// right boundary
double right(double x, double y) {
    return sin(y);
}

// lower boundary
double lower(double x, double y) {
    return 0.0;
}

// upper boundary
double upper(double x, double y){
    return sin(x);
}

// \Delta u = -f
double f(double x, double y){
    return (x*x+y*y)*sin(x*y);
}

// exact solution
double exact_solu(double x, double y) {
    return sin(x*y);
}
