/**
 * a demo
 */

#include "BiCGStab.hxx"
#include <ios>
#include "PCD.hxx"


/// check and allocate memory
FaspRetCode
BiCGStab::Setup(const LOP &_A, const VEC &b, VEC &x) {
    if (x.GetSize() != _A.GetColSize() || b.GetSize() != _A.GetRowSize() ||
        _A.GetRowSize() != _A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len = b.GetSize();
    try {
        this->r0.SetValues(len, 0.0);
        this->rj.SetValues(len, 0.0);
        this->pj.SetValues(len, 0.0);
        this->gj.SetValues(len, 0.0);
        this->sj.SetValues(len, 0.0);
        this->qj.SetValues(len, 0.0);
        this->uj.SetValues(len, 0.0);
        this->yj.SetValues(len, 0.0);
        this->xj.SetValues(len, 0.0);
        this->tmp.SetValues(len, 0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    this->A = &_A;

    /// identical preconditioner operator by default
    if (pc == nullptr) {
        IdentityLOP lop(len);
        pc = &lop;
    }

    return FaspRetCode::SUCCESS;
}

/// build preconditioner operator
void BiCGStab::SetPC(LOP *lop) {
    this->pc = lop;
}

/// solve by PCG
FaspRetCode BiCGStab::Solve(const VEC &b, VEC &x) {

    const unsigned MaxStag = 20;
    const INT len = b.GetSize();
    const DBL maxdiff = 1e-4 * relTol; // Stagnation tolerance
    const DBL solinftol = 1e-20; // Infinity norm tolerance

    // Local variables
    FaspRetCode errorCode = FaspRetCode::SUCCESS;
    unsigned stagStep = 1, moreStep = 1;
    DBL resAbs = 1e+20, tmpAbs = 1e+20;
    DBL resRel = 1e+20, denAbs = 1e+20;
    DBL factor, alphaj, betaj, omegaj;

    // Output iterative method info
    if (verbose > PRINT_NONE)
        std::cout<< "\nCalling PCG solver ...\n";

    // r0 = b - A * x
    A->Apply(x, r0);
    r0.XPAY(-1.0, b);
    rj = r0;

    while (true) {
        // p0=r0
        pj = r0;

        // gj = P * pj
        pc->Apply(pj, gj);

        // sj = A * gj
        A->Apply(gj, sj);

        // alpha_{j} = (r0, rj)/(r0, sj)
        alphaj = r0.Dot(rj) / r0.Dot(sj);

        // qj = rj - alpha_{j} * sj
        qj.WAXPBY(1.0, rj, -alphaj, sj);

        // uj = P * qj
        pc->Apply(qj, uj);

        // yj = A * uj
        A->Apply(uj, yj);

        // omega_{j} = (qj, yj)/(yj, yj)
        omegaj = qj.Dot(yj) / yj.Dot(yj);

        // x_{j+1} = x_{j} + alpha_{j} * gj + omega_{j} * u_{j}
        tmp.WAXPBY(alphaj, gj, omegaj, uj);
        xj.AXPY(1.0, tmp);

        // r_{j+1} = qj - omegaj * yj
        tmp = rj;
        rj.WAXPBY(1.0, qj, -omegaj, yj);

        if (rj.Norm2() < 1e-4) {
            std::cout << "Norm2 : " << rj.Norm2() << std::endl;
            break;
        }

        // betaj = alphaj/omegaj * (r0, r_{j+1}) / (r0, r_{j})
        betaj = alphaj / omegaj * r0.Dot(rj) / r0.Dot(tmp);

        // p_{i+1} = r_{i+1} + betaj * (pj - omegaj *sj)
        pj.AXPY(-omegaj, sj);
        pj.AXPBY(betaj, 1.0, rj);
    }
}

/// clean preconditioner operator
void BiCGStab::CleanPCD() {
    if (pc != nullptr)
        pc = nullptr;
}

/// Release temporary memory
void BiCGStab::Clean() {
    VEC zero;
    r0 = zero;
    rj = zero;
    pj = zero;
    gj = zero;
    sj = zero;
    qj = zero;
    uj = zero;
    yj = zero;
    xj = zero;
    tmp = zero;
}


