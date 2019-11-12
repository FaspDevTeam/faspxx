/**
 * a header file for some preconditioners
 */

#ifndef PCD_HXX
#define PCD_HXX

#include "LOP.hxx"

// Identity linear operator
class IdentityLOP : public LOP {
public:
    // constructor
    explicit IdentityLOP(INT n) : LOP(n, n) {}

    // overload = operator
    IdentityLOP &operator=(const IdentityLOP& lop);

    // 'y' = operator applys 'x'
    virtual void Apply(const VEC& x, VEC& y) const;
};

class Jacobi:public LOP {
private:
    // diagonal elements
    VEC diag;

public:
    // constructor
    Jacobi(const std::vector<double>& diag);

    // 'y' = operator applys 'x'
    void Apply(const VEC& x, VEC& y) const;
};

#endif // PCD_HXX
