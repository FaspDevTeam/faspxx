/** \file    testVecSpeed.cxx
 *  \brief   Test speed of vector implementations
 *  \author  Kailei Zhang
 *  \date    Oct/12/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

// Standard header files
#include <cstring>

// FASPXX header files
#include "Timing.hxx"
#include "VEC.hxx"

const int LENGTH = 10000000;
const int count  = 200;

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/

int main()
{
    if (count < 1) return 0; // Do nothing at all!

    std::cout << "Problem size " << LENGTH << ", repeat " << count
              << " times:" << std::endl;

    int         j, k;
    double      sum;
    GetCycleNum cycle;
    GetWallTime timer;

    srand((unsigned)time(nullptr));
    auto* test1 = new double[LENGTH];
    auto* test2 = new double[LENGTH];

    for (j = 0; j < LENGTH; j++) {
        test1[j] = rand() / (double)RAND_MAX;
        test2[j] = rand() / (double)RAND_MAX;
    }

    /*------------------------------------------------------------*/
    std::cout << "\n------ allocation ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.Start();
    cycle.Start();
    auto* ptr1 = new double[LENGTH];
    auto* ptr2 = new double[LENGTH];
    std::cout << "pointer cycles : " << cycle.Stop() << std::endl;
    std::cout << "pointer time   : " << timer.Stop() << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    // Standard declaration of vector is slow (initialization cost):
    //     std::vector<double> vec1(LENGTH), vec2(LENGTH);
    // A much faster way is used as follows (do not initialize!):
    std::vector<double> vec1, vec2;
    vec1.reserve(LENGTH);
    vec2.reserve(LENGTH);
    std::cout << "vector cycles  : " << cycle.Stop() << std::endl;
    std::cout << "vector time    : " << timer.Stop() << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    VEC nvec1, nvec2;
    nvec1.Reserve(LENGTH);
    nvec2.Reserve(LENGTH);
    std::cout << "new VEC cycles : " << cycle.Stop() << std::endl;
    std::cout << "new VEC time   : " << timer.Stop() << "ms" << std::endl;

    //! Timing pointer : 19.0365ms, vector : 19.2257ms, new VEC : 18.9041ms
    /*------------------------------------------------------------*/
    std::cout << "\n------ assignment ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
        memcpy(ptr1, test1, LENGTH * sizeof(double));
        memcpy(ptr2, test2, LENGTH * sizeof(double));
    }
    std::cout << "pointer cycles : " << cycle.Stop() / count << std::endl;
    std::cout << "pointer time   : " << timer.Stop() / count << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
        vec1.assign(test1, test1 + LENGTH);
        vec2.assign(test2, test2 + LENGTH);
    }
    std::cout << "vector cycles  : " << cycle.Stop() / count << std::endl;
    std::cout << "vector time    : " << timer.Stop() / count << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
        nvec1.SetValues(LENGTH, test1);
        nvec2.SetValues(LENGTH, test2);
    }
    std::cout << "new VEC cycles : " << cycle.Stop() / count << std::endl;
    std::cout << "new VEC time   : " << timer.Stop() / count << "ms" << std::endl;

    //! Timing pointer : 10.9632ms, vector : 11.0955ms, new VEC : 11.2195ms
    /*------------------------------------------------------------*/
    std::cout << "\n------ addition ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
#pragma omp parallel private(j) shared(ptr1, ptr2)
#pragma omp for
        for (j = 0; j < LENGTH; j++) ptr1[j] += ptr2[j];
    }
    std::cout << "pointer cycles : " << cycle.Stop() / count << std::endl;
    std::cout << "pointer time   : " << timer.Stop() / count << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
#pragma omp parallel private(j) shared(vec1, vec2)
#pragma omp for
        for (j = 0; j < LENGTH; j++) vec1[j] += vec2[j];
    }
    std::cout << "vector cycles  : " << cycle.Stop() / count << std::endl;
    std::cout << "vector time    : " << timer.Stop() / count << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) nvec1.AXPBY(1.0, 1.0, nvec2);
    std::cout << "new VEC cycles : " << cycle.Stop() / count << std::endl;
    std::cout << "new VEC time   : " << timer.Stop() / count << "ms" << std::endl;

    //! Timing pointer : 23.7068ms, vector : 23.0305ms, new VEC : 9.61151ms
    /*------------------------------------------------------------*/
    std::cout << "\n------ dot product ------" << std::endl;
    /*------------------------------------------------------------*/
    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
        sum = 0.0;
#pragma omp parallel shared(ptr1, ptr2) private(j)
#pragma omp for reduction(+ : sum)
        for (j = 0; j < LENGTH; j++) sum += ptr1[j] * ptr2[j];
    }
    std::cout << "pointer dot    : " << sum << std::endl;
    std::cout << "pointer cycles : " << cycle.Stop() / count << std::endl;
    std::cout << "pointer time   : " << timer.Stop() / count << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
        sum = 0.0;
#pragma omp parallel shared(vec1, vec2) private(j)
#pragma omp for reduction(+ : sum)
        for (j = 0; j < LENGTH; j++) sum += vec1[j] * vec2[j];
    }
    std::cout << "vector dot     : " << sum << std::endl;
    std::cout << "vector cycles  : " << cycle.Stop() / count << std::endl;
    std::cout << "vector time    : " << timer.Stop() / count << "ms" << std::endl;

    timer.Start();
    cycle.Start();
    for (k = 0; k < count; k++) {
        // One can use the overloaded [ ] operator for VEC as well (but slower):
        //     for ( sum = 0.0, j = 0; j < LENGTH; j++ ) sum += nvec1[j] * nvec2[j];
        sum = nvec1.Dot(nvec2);
    }
    std::cout << "new VEC dot    : " << sum << std::endl;
    std::cout << "new VEC cycles : " << cycle.Stop() / count << std::endl;
    std::cout << "new VEC time   : " << timer.Stop() / count << "ms" << std::endl;

    delete[] ptr1;
    delete[] ptr2;
    delete[] test1;
    delete[] test2;

    return 0;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Oct/12/2019      Create file                          */
/*  Chensong Zhang      Sep/17/2021      Test performance of new version      */
/*  Chensong Zhang      Jan/25/2022      Test performance of OpenMP           */
/*----------------------------------------------------------------------------*/