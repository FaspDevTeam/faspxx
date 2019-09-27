/**
 * a source file for PCG
 */

#include "pcg.hxx"
#include "MATUtil.hxx"
#include "VECUtil.hxx"

FaspRetCode CG(const MAT &A, const VEC &b, VEC &x) {

    //some basic examination
    if ( A.GetRowSize() != A.GetColSize())
        return FaspRetCode::ERROR_MAT_SIZE;

    if ( CheckMATVECSize(A, x) != FaspRetCode::SUCCESS )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT count = 0;
    DBL norm_prev, normk;
    DBL alpha, beta;
    VEC rk, pk, tmp;

    DBL R_TOL = 1e-4;

    //! r_{0} = b - A * x_{0}
    rk = A.MultVec(x);

    if ( CheckVECSize(rk, b) != FaspRetCode::SUCCESS )
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    rk.Add(-1.0, 1.0, b);

    pk = rk;

    normk = rk.Dot(rk);

    while ( count < A.GetRowSize() && pk.Norm2() > R_TOL ) {

        tmp = A.MultVec(pk);
        alpha = (normk / tmp.Dot(pk));

        x.Add(1.0, alpha, pk);

        rk.Add(1.0, -1.0 * alpha, tmp);
        norm_prev = normk;
        normk = rk.Dot(rk);
        beta = normk / norm_prev;

        pk.Add(beta, 1.0, rk);

        count++;

    }

    return FaspRetCode::SUCCESS;
}