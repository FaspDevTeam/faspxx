/** \file testVecSpeed.cxx
 *  \brief Test speed of vector implementations
 *
 *  To compile this test, you may (after cmake) use
 *      $ make testVecSpeed
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "timing.hxx"
#include "vec.hxx"

const INT LENGTH = 10000000;
const int count = 100;

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main() {
    std::cout << "Problem size " << LENGTH << ", repeat " << count << " times:" << std::endl;
    if ( count < 1 ) return 0; // Do nothing at all!

    int j, k;
    double sum;
    CycleCount cycle;
    GetWallTime timer;

    srand((unsigned) time(nullptr));
    auto *test1 = new double[LENGTH];
    auto *test2 = new double[LENGTH];

    for ( j = 0; j < LENGTH; j++ ) {
        test1[j] = rand() / (double) RAND_MAX;
        test2[j] = rand() / (double) RAND_MAX;
    }

    /*------------------------------------------------------------*/
    std::cout << "\n------ allocation ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.start();
    cycle.start();
    auto *ptr1 = new double[LENGTH];
    auto *ptr2 = new double[LENGTH];
    std::cout << "pointer cycles : " << cycle.stop() << std::endl;
    std::cout << "pointer time   : " << timer.stop() << " seconds" << std::endl;

    timer.start();
    cycle.start();
    // Standard declaration of vector is slow (initialization cost):
    //     std::vector<double> vec1(LENGTH), vec2(LENGTH);
    // A much faster way is used as follows (do not initialize!):
    std::vector<double> vec1, vec2;
    vec1.reserve(LENGTH);
    vec2.reserve(LENGTH);
    std::cout << "vector cycles  : " << cycle.stop() << std::endl;
    std::cout << "vector time    : " << timer.stop() << " seconds" << std::endl;

    timer.start();
    cycle.start();
    VEC nvec1, nvec2;
    nvec1.Reserve(LENGTH);
    nvec2.Reserve(LENGTH);
    std::cout << "new VEC cycles : " << cycle.stop() << std::endl;
    std::cout << "new VEC time   : " << timer.stop() << " seconds" << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ assignment ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        memcpy(ptr1, test1, LENGTH * sizeof(double));
        memcpy(ptr2, test2, LENGTH * sizeof(double));
    }
    std::cout << "pointer cycles : " << cycle.stop() / count << std::endl;
    std::cout << "pointer time   : " << timer.stop() / count << " seconds" << std::endl;

    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        vec1.assign(test1, test1 + LENGTH);
        vec2.assign(test2, test2 + LENGTH);
    }
    std::cout << "vector cycles  : " << cycle.stop() / count << std::endl;
    std::cout << "vector time    : " << timer.stop() / count << " seconds" << std::endl;

    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        nvec1.SetValues(LENGTH, test1);
        nvec2.SetValues(LENGTH, test2);
    }
    std::cout << "new VEC cycles : " << cycle.stop() / count << std::endl;
    std::cout << "new VEC time   : " << timer.stop() / count << " seconds" << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ addition ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( j = 0; j < LENGTH; j++ ) ptr1[j] += ptr2[j];
    }
    std::cout << "pointer cycles : " << cycle.stop() / count << std::endl;
    std::cout << "pointer time   : " << timer.stop() / count << " seconds" << std::endl;

    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( j = 0; j < LENGTH; j++ ) vec1[j] += vec2[j];
    }
    std::cout << "vector cycles  : " << cycle.stop() / count << std::endl;
    std::cout << "vector time    : " << timer.stop() / count << " seconds" << std::endl;

    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) nvec1.Add(nvec2, 1.0, 1.0);
    std::cout << "new VEC cycles : " << cycle.stop() / count << std::endl;
    std::cout << "new VEC time   : " << timer.stop() / count << " seconds" << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ dot product ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += ptr1[j] * ptr2[j];
    }
    std::cout << "pointer dot    : " << sum << std::endl;
    std::cout << "pointer cycles : " << cycle.stop() / count << std::endl;
    std::cout << "pointer time   : " << timer.stop() / count << " seconds" << std::endl;

    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += vec1[j] * vec2[j];
    }
    std::cout << "vector dot     : " << sum << std::endl;
    std::cout << "vector cycles  : " << cycle.stop() / count << std::endl;
    std::cout << "vector time    : " << timer.stop() / count << " seconds" << std::endl;

    timer.start();
    cycle.start();
    for ( k = 0; k < count; k++ ) {
        // One can use the overloaded [ ] operator for VEC as well (but slower):
        //     for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += nvec1[j] * nvec2[j];
        sum = nvec1.Dot(nvec2);
    }
    std::cout << "new VEC dot    : " << sum << std::endl;
    std::cout << "new VEC cycles : " << cycle.stop() / count << std::endl;
    std::cout << "new VEC time   : " << timer.stop() / count << " seconds" << std::endl;

    delete[] ptr1;
    delete[] ptr2;
    delete[] test1;
    delete[] test2;

    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/
