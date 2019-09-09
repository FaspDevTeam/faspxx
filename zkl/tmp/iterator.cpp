// Usage: g++ -O2 -o testvector testvector.cpp

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <iterator>

typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned long long ticks;

struct iota_iterator : std::iterator<std::forward_iterator_tag, int> {
    iota_iterator(int value = int()) : value(value) { }

    iota_iterator& operator ++() {
        ++value;
        return *this;
    }

    iota_iterator operator ++(int) {
        iota_iterator copy = *this;
        ++*this;
        return copy;
    }

    int const& operator *() const {
        return value;
    }

    int const* operator ->() const {
        return &value;
    }

    friend bool operator ==(iota_iterator const& lhs, iota_iterator const& rhs) {
        return lhs.value == rhs.value;
    }

    friend bool operator !=(iota_iterator const& lhs, iota_iterator const& rhs) {
        return not (lhs == rhs);
    }

private:
    int value;
};

int runtestgenerator(size_t N) {
    // Extra parentheses to prevent most vexing parse.
    vector<int> bigarray((iota_iterator<int>()), iota_iterator<int>(N));
    for (unsigned int k = 0; k<N; ++k)
        std::cout<<k<<"  ";
}

// Main test function for std::vector
int main() {

    const size_t N = 10;

    runtestgenerator(N);

    return 0;
}
