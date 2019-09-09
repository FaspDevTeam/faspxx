// Usage: g++ -O2 -o testvector testvector.cpp

#include <sys/stat.h>
#include <iostream>
#include <vector>

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

    friend bool operator ==(iota_iterator const& lhs, iota_iterator const& rhs) {
        return lhs.value == rhs.value;
    }

    friend bool operator !=(iota_iterator const& lhs, iota_iterator const& rhs) {
        return not (lhs == rhs);
    }

private:
    int value;
};

void runtestgenerator(size_t N) {
    // Extra parentheses to prevent most vexing parse.
    std::vector<int> bigarray((iota_iterator(1)), iota_iterator(N+1));
    for (unsigned int k = 0; k<N; ++k)
        std::cout<<bigarray[k]<<"  ";
}

// Main test function for std::vector
int main() {
    return 0;
}
