
#ifndef FASP_VECTOR
#define FASP_VECTOR

namespace fasp
{

class Vector
{
protected:
    int size, allocsize;
    doubtl *data;

public:
    Vector() { allocsize = size = 0; data = nullptr;} ///nullptr比0,NULL要好
    explicit Vector(int s);

public:
    Vector &operator += (double c);
    Vector &operator += (const Vector &vec);
    Vector &operator -= (double c);
    Vector &operator -= (const Vector &vec);
    Vector &operator *= (double c);
    Vector &operator /= (double c);

    double Dot(const Vector& vec);

    void GetSubVector();
    double Norml2();

    void Print();
};


}


#endif