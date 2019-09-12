/** \file testVecSpeed.cxx
 *  \brief Test speed of vector implementations
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <sys/time.h>
#include <cstring>
#include "cyclecount.hxx"

const int LENGTH = 32000000;
const int count = 10;

static double get_wall_time() {
    struct timeval time;
    if (gettimeofday(&time, NULL)) return 0;
    return (double) time.tv_sec + (double) time.tv_usec * 0.000001;
}

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main() {
    int j, k;
    double sum, add;
    double time_start, time_end;
    CycleCount cycle;

    srand((unsigned) time(NULL));
    double *test_1 = new double[LENGTH];
    double *test_2 = new double[LENGTH];

    for (j = 0; j < LENGTH; j++) {
        test_1[j] = rand() / (double) RAND_MAX;
        test_2[j] = rand() / (double) RAND_MAX;
    }

    /*------------------------------------------------------------*/
    std::cout << "\n------ allocation ------" << std::endl;

    time_start = get_wall_time();
    cycle.start();
    double *p_1 = new double[LENGTH];
    double *p_2 = new double[LENGTH];
    std::cout << "pointer cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "pointer time   : " << time_end - time_start << std::endl;

    time_start = get_wall_time();
    cycle.start();
    std::vector<double> vec_1(LENGTH), vec_2(LENGTH);
    std::cout << "vector cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "vector time   : " << time_end - time_start << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ assignment ------" << std::endl;

    time_start = get_wall_time();
    cycle.start();
    for (k = 0; k < count; k++) {
        memcpy(p_1, test_1, LENGTH*sizeof(double));
        memcpy(p_2, test_2, LENGTH*sizeof(double));
    }
    std::cout << "pointer cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "pointer time   : " << (time_end - time_start)/count << std::endl;

    time_start = get_wall_time();
    cycle.start();
    for (k = 0; k < count; k++) {
        vec_1.assign(test_1, test_1 + LENGTH);
        vec_2.assign(test_2, test_2 + LENGTH);
    }
    std::cout << "vector cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "vector time   : " << (time_end - time_start)/count << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ addition ------" << std::endl;

    add = 0.0;
    time_start = get_wall_time();
    cycle.start();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            add = add + p_1[j] + p_2[j];
        }
    }
    std::cout << "pointer add    : " << add << std::endl;
    std::cout << "pointer cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "pointer time   : " << (time_end - time_start)/count << std::endl;

    add = 0.0;
    time_start = get_wall_time();
    cycle.start();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            add = add + vec_1[j] + vec_2[j];
        }
    }
    std::cout << "vector add    : " << add << std::endl;
    std::cout << "vector cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "vector time   : " << (time_end - time_start)/count << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\n------ dot product ------" << std::endl;

    sum = 0.0;
    time_start = get_wall_time();
    cycle.start();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            sum += p_1[j] * p_2[j];
        }
    }
    std::cout << "pointer dot    : " << sum << std::endl;
    std::cout << "pointer cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "pointer time   : " << (time_end - time_start)/count << std::endl;
    
    sum = 0.0;
    time_start = get_wall_time();
    cycle.start();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            sum += vec_1[j] * vec_2[j];
        }
    }
    std::cout << "vector dot    : " << sum << std::endl;
    std::cout << "vector cycles : " << cycle.stop() << std::endl;
    time_end = get_wall_time();
    std::cout << "vector time   : " << (time_end - time_start)/count << std::endl;
 
    delete[] p_1;
    delete[] p_2;
    delete[] test_1;
    delete[] test_2;

    return 0;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/