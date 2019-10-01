/**
 * a source file for PCG
 */

#include "PCG.hxx"
#include "VECUtil.hxx"

FaspRetCode PCG::SetUp(const MAT &A_, const VEC &b_, const DBL &rtol_, const INT
&maxIt_) {
    if ( A_.GetNNZ() == 0 || b_.GetSize() == 0 || A_.GetRowSize() != b_.GetSize()){
        return FaspRetCode::ERROR_INPUT_PAR;
    }

    try {
        this->A = A_;
        this->b = b_;
    } catch ( std::bad_alloc &ex ) {
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }
    this->rtol = rtol_;
    this->maxIt = maxIt_;

    return FaspRetCode::SUCCESS;
}

FaspRetCode PCG::SetUpPCD(const MAT &P_) {
    if ( P_.GetColSize() != this->A.GetRowSize())
        return FaspRetCode::ERROR_INPUT_PAR;

    try {
        this->P = P_;
        this->pflag = 1;
    } catch ( std::bad_alloc &ex ) {
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }

    return FaspRetCode::SUCCESS;
}

FaspRetCode PCG::Start(VEC &x, INT &iter) {

    INT count = 0;
    DBL norm_prev, normk;
    DBL alpha, beta;
    VEC rk, pk, tmp;

    try {
        if ( x.GetSize() != A.GetColSize()) {
            FaspRetCode retCode = FaspRetCode::ERROR_NONMATCH_SIZE;
            throw (FaspRunTime(retCode, __FILE__, __FUNCTION__, __LINE__));
        }
    } catch ( FaspRunTime &ex ) {
        return FaspRetCode::ERROR_NONMATCH_SIZE;
    }

    //! r_{0} = b - A * x_{0}
    rk = this->A.MultVec(x);

    rk.Add(-1.0, 1.0, b);

    if ( pflag == 1 ) {
        /* z0=M^{-1} * r0 */
        /* p0=z0 */
        /* normk=r_{j} * z_{j} */
    } else {
        pk = rk;
        normk = rk.Dot(rk);
    }

    while ( count < maxIt && rk.Norm2() > rtol ) {
        if ( pflag == 1 ) {
            /* alpha = (r_{j},z_{j}) / (Ap_{j},p_{j}) */
        } else {
            tmp = A.MultVec(pk);
            alpha = (normk / tmp.Dot(pk));
        }

        x.Add(1.0, alpha, pk);

        rk.Add(1.0, -alpha, tmp);

        if ( pflag == 1 ) {
            /* z_{j+1} = M^{-1} r_{j+1} */
            /* beta_{j} = (r_{j+1},z_{j+1}) / (r_{j},z_{j}) */
            /* p_{j+1} = z_{j+1} + beta_{j} p_{j} */
        } else {
            norm_prev = normk;
            normk = rk.Dot(rk);
            beta = normk / norm_prev;
            pk.Add(beta, 1.0, rk);
        }

        count++;
    }
    iter = count;

    return FaspRetCode::SUCCESS;
}

void PCG::CleanPCD() {
    MAT Pre;
    this->P = Pre;
}

void PCG::Clean() {
    MAT Atmp;
    this->A = Atmp;
    VEC v;
    this->b = v;
}