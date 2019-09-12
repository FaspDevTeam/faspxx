/** \file cyclecount.hxx
 *  \brief Count CPU cycles used
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __CYCLECOUNT_HEADER__      /*-- allow multiple inclusions --*/
#define __CYCLECOUNT_HEADER__      /**< indicate cyclecount.hxx has been included before */

typedef unsigned long long uint64; //! Long long int type

/*! \class CountCycle
 *  \brief CPU cycle count
 *
 *  This class read the CPU cycle count with a piece of ASM code and return cycle count upon stop().
 */
class CycleCount {

private:
    uint64 cycleClock; //!< Current CPU cycle clock

private:

    //! Read Time Stamp Counter (TSC)
    static __inline__ uint64 startRDTSC (void) {
        unsigned cycleLow, cycleHigh;
        asm volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (cycleHigh), "=r" (cycleLow)::"%rax", "%rbx", "%rcx", "%rdx");
        return (static_cast<uint64>(cycleHigh) << 32) | cycleLow;
    }

    //! Read Time Stamp Counter and Processor ID (TSCP)
    static __inline__ uint64 stopRDTSCP (void) {
        unsigned cycleLow, cycleHigh;
        asm volatile ("RDTSCP\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t"
                      "CPUID\n\t": "=r" (cycleHigh), "=r" (cycleLow)::"%rax", "%rbx", "%rcx", "%rdx");
        return (static_cast<uint64>(cycleHigh) << 32) | cycleLow;
    }

public:

    //! Start the cycle count clock
    __inline__ void start() {cycleClock = startRDTSC();}

    //! Stop the cycle count clock and return number of cycles from start()
    __inline__ unsigned long long stop() const {return stopRDTSCP() - cycleClock;}
};

#endif /* end if for __CYCLECOUNT_HEADER__ */

/*---------------------------------*/
/*--     Beginning of main       --*/
/*---------------------------------*/