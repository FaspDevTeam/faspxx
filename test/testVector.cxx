/**
 * seek for the fastest operations among pointer, array and vector
 */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <sys/time.h>
#include <cstring>

const int LENGTH = 16000000;
const int count = 10;

double get_wall_time() {
    struct timeval time;
    if (gettimeofday(&time, NULL)) return 0;
    return (double) time.tv_sec + (double) time.tv_usec * 0.000001;
}

int main() {
    double time_start, time_end;
    double sum, add;
    int j, k;
    double average;

    srand((unsigned) time(NULL));
    double *test_1 = new double[LENGTH];
    double *test_2 = new double[LENGTH];

    for (j = 0; j < LENGTH; j++) {
        test_1[j] = rand() / (double) RAND_MAX;
        test_2[j] = rand() / (double) RAND_MAX;
    }

    /*------------------------------------------------------------*/
    std::cout << "\nallocation time : " << std::endl;

    time_start = get_wall_time();
    double *p_1 = new double[LENGTH];
    double *p_2 = new double[LENGTH];
    time_end = get_wall_time();
    std::cout << "pointer : " << time_end - time_start << std::endl;

    time_start = get_wall_time();
    std::vector<double> vec_1(LENGTH), vec_2(LENGTH);
    time_end = get_wall_time();
    std::cout << "vector : " << time_end - time_start << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\nassignment time : " << std::endl;

    time_start = get_wall_time();
    for (k = 0; k < count; k++) {
        memcpy(p_1, test_1, LENGTH*sizeof(double));
        memcpy(p_2, test_2, LENGTH*sizeof(double));
    }
    time_end = get_wall_time();
    std::cout << "pointer : " << (time_end - time_start)/count << std::endl;

    time_start = get_wall_time();
    for (k = 0; k < count; k++) {
        vec_1.assign(test_1, test_1 + LENGTH);
        vec_2.assign(test_2, test_2 + LENGTH);
    }
    time_end = get_wall_time();
    std::cout << "vector : " << (time_end - time_start)/count << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\naddition time : " << std::endl;

    add = 0.0;
    time_start = get_wall_time();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            add = add + p_1[j] + p_2[j];
        }
    }
    time_end = get_wall_time();
    std::cout << "pointer add : " << add << std::endl;
    std::cout << "pointer : " << (time_end - time_start)/count << std::endl;

    add = 0.0;
    time_start = get_wall_time();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            add = add + vec_1[j] + vec_2[j];
        }
    }
    time_end = get_wall_time();
    std::cout << "vector add : " << add << std::endl;
    std::cout << "vector : " << (time_end - time_start)/count << std::endl;

    /*------------------------------------------------------------*/
    std::cout << "\ndot product time : " << std::endl;

    sum = 0.0;
    time_start = get_wall_time();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            sum += p_1[j] * p_2[j];
        }
    }
    time_end = get_wall_time();
    std::cout << "pointer dot : " << sum << std::endl;
    std::cout << "pointer : " << (time_end - time_start)/count << std::endl;
    
    sum = 0.0;
    time_start = get_wall_time();
    for (k = 0; k < count; k++) {
        for (j = 0; j < LENGTH; j++) {
            sum += vec_1[j] * vec_2[j];
        }
    }
    time_end = get_wall_time();
    std::cout << "vector dot : " << sum << std::endl;
    std::cout << "vector : " << (time_end - time_start)/count << std::endl;
 
    delete[] p_1;
    delete[] p_2;
    delete[] test_1;
    delete[] test_2;

    return 0;
}
