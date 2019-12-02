/**
 * source codes for some preconditioner
 */

#include "PCD.hxx"

/// identity preconditioner
FaspRetCode IdentityPC::Solve(const VEC &x, VEC &y) {
    y = x;
    return FaspRetCode::SUCCESS;
}

/// constructor
Jacobi::Jacobi(INT row, INT col, std::vector<DBL> diag) {
    this->row = row;
    this->col = col;

    this->diag.SetValues(diag);
}

/// jacobi preconditioner
FaspRetCode Jacobi::Solve(const VEC &x, VEC &y) {
    VEC tmp = this->diag;
    tmp.Reciprocal();
    y = x;
    y.PointwiseMult(tmp);

    return FaspRetCode ::SUCCESS;
}