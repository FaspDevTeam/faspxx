
#include "vector.hpp"

namespace fasp
{

inline Vector::Vector(int s)
{
    if (s > 0)
    {
        allocsize = size = s;
        data = new double[s];
    }
    else
    {
        allocsize = size = 0;
        data = nullptr;
    }
}

Vector& Vector::operator*=(double c)
{
    for (int i=0; i<size; i++)
    {
        data[i] *= c;
    }
    return *this;
}


}


