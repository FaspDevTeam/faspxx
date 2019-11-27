/**
 * source codes for some preconditioner
 */

#include "PCD.hxx"

/// identity preconditioner
FaspRetCode IdentityPC::Solve(const VEC &x, VEC &y) {
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    if (x.GetSize() != y.GetSize())
        errorCode = FaspRetCode::ERROR_VEC_SIZE;
    else
        y = x;

    return errorCode;
}

/// constructor
Jacobi::Jacobi(INT row, INT col,std::vector<DBL> diag) {
    this->row = row;
    this->col = col;

    this->diag.SetValues(diag);
}

/// jacobi preconditioner
FaspRetCode Jacobi::Solve(const VEC &x, VEC &y) {
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    if(x.GetSize()!=y.GetSize())
        errorCode=FaspRetCode ::ERROR_VEC_SIZE;

    VEC tmp=this->diag;
    tmp.Reciprocal();
    y=x;
    y.PointwiseMult(tmp);

    return errorCode;
}