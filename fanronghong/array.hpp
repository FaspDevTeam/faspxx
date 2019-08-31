
#ifndef FASPXX_ARRAY_HPP
#define FASPXX_ARRAY_HPP


namespace fasp
{

template <class T>
class Array
{
protected:
    int size, allocsize, increase;
    void *data;

public:
    Array() {}
    explicit Array(int s);
    ~Array();

public:
    void Print();
};

}

#endif //FASPXX_ARRAY_HPP
