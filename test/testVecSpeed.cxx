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
#include "cyclecount.hxx"
#include "../src/vec.hxx"

const int LENGTH = 32000000;
const int count = 10;

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main() {
    int j, k;
    double sum, add;
    CycleCount cycle;

    srand((unsigned) time(nullptr));
    auto *test_1 = new double[LENGTH];
    auto *test_2 = new double[LENGTH];

    for ( j = 0; j < LENGTH; j++ ) {
        test_1[j] = rand() / (double) RAND_MAX;
        test_2[j] = rand() / (double) RAND_MAX;
    }

    /*------------------------------------------------------------*/
    std::cout << "\n------ allocation ------" << std::endl;

    cycle.start();
    auto *ptr_1 = new double[LENGTH];
    auto *ptr_2 = new double[LENGTH];
    std::cout << "pointer cycles : " << cycle.stop() << std::endl;

    cycle.start();
    // Standard declaration of vector is slow (initialization cost):
    //     std::vector<double> vec_1(LENGTH), vec_2(LENGTH);
    // A much faster way is used as follows (do not initialize!):
    std::vector<double> vec_1, vec_2;
    vec_1.reserve(LENGTH);
    vec_2.reserve(LENGTH);
    std::cout << "vector cycles  : " << cycle.stop() << std::endl;

    cycle.start();
    VEC v_1, v_2;
    v_1.SetSize(LENGTH);
    v_2.SetSize(LENGTH);
    std::cout << "VEC cycles : " << cycle.stop() << std::endl;


    /*------------------------------------------------------------*/
    std::cout << "\n------ assignment ------" << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        memcpy(ptr_1, test_1, LENGTH * sizeof(double));
        memcpy(ptr_2, test_2, LENGTH * sizeof(double));
    }
    std::cout << "pointer cycles : " << cycle.stop() / count << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        vec_1.assign(test_1, test_1 + LENGTH);
        vec_2.assign(test_2, test_2 + LENGTH);
    }
    std::cout << "vector cycles  : " << cycle.stop() / count << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        v_1.SetValues(LENGTH, test_1);
        v_2.SetValues(LENGTH, test_2);
    }
    std::cout << "VEC cycles : " << cycle.stop() / count << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ addition ------" << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( add = 0.0, j = 0; j < LENGTH; j++ ) add += ptr_1[j] + ptr_2[j];
    }
    std::cout << "pointer add    : " << add << std::endl;
    std::cout << "pointer cycles : " << cycle.stop() / count << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( add = 0.0, j = 0; j < LENGTH; j++ ) add += vec_1[j] + vec_2[j];
    }
    std::cout << "vector add     : " << add << std::endl;
    std::cout << "vector cycles  : " << cycle.stop() / count << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( add = 0.0, j = 0; j < LENGTH; j++ ) add += v_1[j] + v_2[j];
    }
    std::cout << "VEC add        : " << add << std::endl;
    std::cout << "VEC cycles     : " << cycle.stop() / count << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ dot product ------" << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += ptr_1[j] * ptr_2[j];
    }
    std::cout << "pointer dot    : " << sum << std::endl;
    std::cout << "pointer cycles : " << cycle.stop() / count << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += vec_1[j] * vec_2[j];
    }
    std::cout << "vector dot     : " << sum << std::endl;
    std::cout << "vector cycles  : " << cycle.stop() / count << std::endl;

    cycle.start();
    for ( k = 0; k < count; k++ ) {
        for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += v_1[j] * v_2[j];
    }
    std::cout << "VEC dot        : " << sum << std::endl;
    std::cout << "VEC cycles     : " << cycle.stop() / count << std::endl;

    delete[] ptr_1;
    delete[] ptr_2;
    delete[] test_1;
    delete[] test_2;

    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/