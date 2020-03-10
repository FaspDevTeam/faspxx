
#ifndef VECCLUSTER_HXX
#define VECCLUSTER_HXX

#include "VEC.hxx"

class VecCluster{
private:
    std::vector<DBL> cluster;
    INT size;
    INT number;
public:

    VecCluster():cluster(0),size(0),number(0){};

    VecCluster(INT size,INT number);

    void SetSize(INT size,INT number);

    INT GetSize();

    INT GetNumber();

    void Get(INT j,VEC &v);

    void Set(INT j,VEC &v);
};

#endif //VECCLUSTER_HXX
