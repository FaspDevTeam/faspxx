/**
 * a header file about timing
 */

#include "faspxx.hxx"

/*! \class CountCycle
 *  \brief CPU cycle count
 *
 *  This class read the CPU cycle count with a piece of ASM code and return cycle count upon stop().
 */
class CountCycle {

private:
    unsigned long long CycleClock; //!< Current CPU cycle clock

private:

    //! Read Time Stamp Counter (TSC)
    static __inline__ uint64 startRDTSC (void) {
        unsigned CycleLow, CycleHigh;
        asm volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (CycleHigh), "=r" (CycleLow)::"%rax", "%rbx", "%rcx", "%rdx");
        return (static_cast<uint64>(CycleHigh) << 32) | CycleLow;
    }

    //! Read Time Stamp Counter and Processor ID (TSCP)
    static __inline__ uint64 stopRDTSCP (void) {
        unsigned CycleLow, CycleHigh;
        asm volatile ("RDTSCP\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t"
                      "CPUID\n\t": "=r" (CycleHigh), "=r" (CycleLow)::"%rax", "%rbx", "%rcx", "%rdx");
        return (static_cast<uint64>(CycleHigh) << 32) | CycleLow;
    }

public:

    //! Start the cycle count clock
    __inline__ void start() {CycleClock = startRDTSC();}

    //! Stop the cycle count clock and return number of cycles from start()
    __inline__ unsigned long long stop() const {return stopRDTSCP() - CycleClock;}
};