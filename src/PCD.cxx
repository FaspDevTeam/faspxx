/**
 * source codes for some preconditioner
 */

#include "PCD.hxx"

// overload = operator
IdentityLOP& IdentityLOP::operator=(const IdentityLOP& lop) {
    this->nrow = lop.nrow;
    this->ncol = lop.ncol;
    return *this;
}

// 'y' = operator applys 'x'
void IdentityLOP::Apply(const VEC& x, VEC& y) const {
    y = x;
}

// constructor
Jacobi::Jacobi(const std::vector<double>& diag) {
    this->diag.SetValues(diag);
    this->nrow = diag.size();
    this->ncol = diag.size();
}

// 'y' = operator applys 'x'
void Jacobi::Apply(const VEC& x, VEC& y) const {
    y = x;
    y.PointwiseDivide(diag);
}