/** \file    Timing.hxx
 *  \brief   Elapsed wall-time and CPU-cycles declaration
 *  \author  Chensong Zhang
 *  \date    Sep/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __TIMING_HEADER__      /*-- allow multiple inclusions --*/
#define __TIMING_HEADER__      /**< indicate timing.hxx has been included --*/

typedef unsigned long long uint64; ///< Unsigned long long int

// Standard header files
#include <chrono>    // For high-resolution CPU time
#include <string>    // For output string
#include <iomanip>
#include <iostream>

/*! \class GetWallTime
 *  \brief Get elapsed wall-time in millisecond
 *
 *  Read the current wall-time and return duration from start() to stop().
 */
class GetWallTime {

private:

    std::chrono::system_clock::time_point timeStamp; ///< Current CPU time

public:

    /// Start the timer
    __inline__ void Start ( ) {timeStamp = std::chrono::system_clock::now();}

    /// Stop the timer and return duration from start() in ms
    __inline__ double Stop ( ) const {
        auto elapsedTime = std::chrono::system_clock::now() - timeStamp;
        return std::chrono::duration <double, std::milli> (elapsedTime).count();
    }

    /// Stop the timer and print out duration time
    void StopInfo(const std::string& info, std::ostream& out = std::cout) const;
};

/*! \class GetCycleNum
 *  \brief Get CPU-cycle number
 *
 *  Read the CPU cycles and return number of cycles from start() to stop().
 */
class GetCycleNum {

private:

    uint64 cycleClock = 0; ///< Current CPU cycle counter

    /// Read Time Stamp Counter (TSC)
    static __inline__ uint64 startRDTSC ( ) {
        unsigned cycleLow, cycleHigh;
        asm volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (cycleHigh), "=r" (cycleLow)::"%rax", "%rbx", "%rcx", "%rdx");
        return (static_cast<uint64>(cycleHigh) << 32) | cycleLow;
    }

    /// Read Time Stamp Counter and Processor ID (TSCP)
    static __inline__ uint64 stopRDTSCP ( ) {
        unsigned cycleLow, cycleHigh;
        asm volatile ("RDTSCP\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t"
                      "CPUID\n\t": "=r" (cycleHigh), "=r" (cycleLow)::"%rax", "%rbx", "%rcx", "%rdx");
        return (static_cast<uint64>(cycleHigh) << 32) | cycleLow;
    }

public:

    /// Start the cycle count clock
    __inline__ void Start ( ) {cycleClock = startRDTSC();}

    /// Stop the cycle count clock and return number of cycles from start()
    __inline__ unsigned long long Stop ( ) const {return stopRDTSCP() - cycleClock;}

};

#endif /*-- end if for __TIMING_HEADER__ --*/

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/