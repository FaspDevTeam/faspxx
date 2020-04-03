
#include "GMRES.hxx"

INT GMRES::Locate(INT row, INT col) {
    return row * this->params.restart + col;
}

void GMRES::HouseHolder(std::vector<DBL> H, VEC betae1) {
    DBL cos, sin, sum;
    std::vector<DBL> Row2((this->params.restart + 1) * 2);
    std::vector<DBL> tmpb(2, 0);
    for (INT k = 0; k < this->params.restart; ++k) {
        sum = pow(H[Locate(k, k)], 2) +
              pow(H[Locate(k + 1, k)], 2);
        sin = H[Locate(k + 1, k)] / sqrt(sum);
        cos = H[Locate(k, k)] / sqrt(sum);
        for (INT j = 0; j < this->params.restart + 1; ++j) {
            Row2[j] = cos * H[Locate(k, j)] +
                      sin * H[Locate(k + 1, j)];
            Row2[j + this->params.restart + 1] = -sin * H[Locate(k, j)] +
                                                 cos * H[Locate(k + 1, j)];
        }

        tmpb[0] = cos * betae1[k] + sin * betae1[k + 1];
        tmpb[1] = -sin * betae1[k] + cos * betae1[k + 1];

        for (INT j = 0; j < this->params.restart + 1; ++j) {
            H[Locate(k, j)] = Row2[j];
            H[Locate(k + 1, j)] = Row2[j + this->params.restart + 1];
        }

        betae1[k] = tmpb[0];
        betae1[k + 1] = tmpb[1];
    }
}

void GMRES::SolveMin(std::vector<DBL> H, VEC betae1, VEC qk) {
    qk.SetValues(this->length, 0.0);
    qk[this->length-1] =
            betae1[this->length - 1] /
            H[Locate(this->length - 1, this->length - 1)];
    for (INT j = this->length - 1; j > 0; --j) {
        for (INT k = this->length - 1; k >= j; --k)
            betae1[j - 1] -= H[Locate(j - 1, k)] * qk[k];
        qk[j - 1] = betae1[j - 1] / H[Locate(j - 1, j - 1)];
    }
}

/// Setup the LGMRES method.
FaspRetCode LGMRES::Setup(const LOP &A) {
    // Set solver type
    SetSolType(SOLType::GMRES);

    // Allocate memory for temporary vectors
    try {
        const INT len = A.GetColSize();
        p0.SetValues(len, 0.0);
        pk.SetValues(len, 0.0);
        r0.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        V.SetSize(len, this->params.restart);
        vk.SetValues(len, 0.0);
        wk.SetValues(len, 0.0);
        betae1.SetValues(this->params.restart + 1,0.0);
        H.resize((this->params.restart + 1) * this->params.restart);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

/// Solve Ax=b using the LGMRES method.
FaspRetCode LGMRES::Solve(const VEC &b, VEC &x) {
    // Local variables
    const INT len = b.GetSize();
    const int maxStag = MAX_STAG_NUM; // max number of stagnation checks
    const double solStagTol = 1e-4 * params.relTol; // solution stagnation tolerance
    const double solZeroTol = CLOSE_ZERO; // solution close to zero tolerance

    int stagStep = 0, moreStep = 0;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;
    double alpha, beta, tmpa, tmpb;

    PrintHead();

    // Initialize iterative method
    numIter = 0;
    A->Apply(x, tmp); // A * x -> rk
    tmp.XPAY(-1.0, b); // b - rk -> rk

    resAbs = tmp.Norm2();
    if (resAbs < this->params.absTol)
        return FaspRetCode::SUCCESS;

    while (1) {

        pc->Solve(tmp, r0);

        beta = r0.Norm2();
        vk = r0;
        vk.Scale(1 / beta);
        V.Set(0, vk);
        for (INT j = 0; j < this->params.restart; ++j) {
            A->Apply(vk, tmp);
            pc->Solve(tmp, wk);
            for (INT i = 0; i < j; ++i)
                H[Locate(i, j)] = vk.Dot(wk);
            for (INT k = 0; k < j; ++k) {
                V.Get(k, vk);
                vk.Scale(H[Locate(k, j)]);
                wk -= vk;
            }
            H[Locate(j + 1, j)] = wk.Norm2();
            if (H[Locate(j + 1, j)] < CLOSE_ZERO) {
                length = j+1;
                break;
            }
            wk.Scale(1 / H[Locate(j + 1, j)]);
            vk = wk;
            V.Set(j + 1, vk);
        }
        HouseHolder(H, betae1);
        SolveMin(H, betae1, qk);
        pk = x;
        for (INT j = 0; j < this->params.restart; ++j) {
            V.Get(j, vk);
            vk.Scale(qk[j]);
            pk += vk;
        }

        A->Apply(pk, tmp); // A * x -> rk
        tmp.XPAY(-1.0, b); // b - rk -> rk

        resAbs = tmp.Norm2();
        if (resAbs < this->params.absTol) {
            x = pk;
            break;
        }
    }
}
