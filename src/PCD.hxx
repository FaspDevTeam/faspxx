/**
 * a header file for some preconditioners
 */

#ifndef PCD_HXX
#define PCD_HXX

#include "SOL.hxx"

class IdentityPC : public SOL{
private:
    INT row;
    INT col;

public:

    // default constructor
    IdentityPC():row(0),col(0){};

    // constructor
    explicit IdentityPC(INT row,INT col);

    /// solve by SOL
    virtual FaspRetCode Solve(const VEC& x, VEC& y);

    /// destructor
    ~IdentityPC(){}

};

class Jacobi : public SOL{
public :
    INT row;
    INT col;
    VEC diag;

public:
    // default constructor
    Jacobi():row(0),col(0){}

    // constructor
    explicit Jacobi(INT row,INT col,std::vector<DBL> diag);

    /// solve by SOL
    virtual FaspRetCode Solve(const VEC& x, VEC& y);

    /// destructor
    ~Jacobi(){};

};


#endif // PCD_HXX