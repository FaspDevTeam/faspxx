/**
 * the *.cxx file to seek for the fastest method among array, pointer, vector
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/time.h>
#include <cstdlib>

double get_wall_time() {
    struct timeval time;
    if (gettimeofday(&time, NULL)) {
        return 0;
    }
    return (double) time.tv_sec + (double) time.tv_usec * 0.000001;
}
