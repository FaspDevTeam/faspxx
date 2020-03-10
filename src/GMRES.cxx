
#include "Gmres.hxx"

FaspRetCode GMRES::Setup(const LOP &A) {

    // Set solver type
    SetSolType(SOLType::GMRES);

    // Allocate memory for temporary vectors
    try {
        const INT len = A.GetColSize();
        vk.SetValues(len, 0.0);
        rk.SetValues(len, 0.0);
        wk.SetValues(len, 0.0);
        yk.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        betae1.SetValues(len, 0.0);
        safe.SetValues(len, 0.0);
        cluster.SetSize(len, len);
        Hij.resize((len + 1) * len);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if (params.verbose > PRINT_MIN) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}

INT GMRES::SetPosition(INT row, INT col) {
    return row * this->A->GetColSize() + col;
}

void GMRES::Arnoldi(std::vector<DBL> &Hij, VecCluster &cluster) {
    DBL norm;
    for (INT j = 0; j < this->params.restart; ++j) {
        for (INT i = 0; i <= j; ++i) {
            cluster.Get(j, vk);
            A->Apply(vk, tmp);
            cluster.Get(i, vk);
            Hij[SetPosition(i, j)] = vk.Dot(tmp);
        }
        wk = tmp;
        for (INT k = 0; k < j; ++k) {
            cluster.Get(k, vk);
            vk.Scale(Hij[SetPosition(k, j)]);
            wk -= vk;
        }
        Hij[SetPosition(j + 1, j)] = wk.Norm2();
        if (Hij[SetPosition(j + 1, j)] == 0)
            break;
        wk.Scale(Hij[1 / SetPosition(j + 1, j)]);
        vk = wk;
    }
}

void GMRES::HouseHolder(std::vector<DBL> &Hij, VEC &b) {
    DBL cos, sin, sum;
    std::vector<DBL> Row2(this->A->GetColSize() * 2);
    std::vector<DBL> tmpb(2, 0);
    for (INT k = 0; k < this->A->GetRowSize(); ++k) {
        sum = pow(Hij[SetPosition(k, k)], 2) +
              pow(Hij[SetPosition(k + 1, k)], 2);
        sin = Hij[SetPosition(k + 1, k)] / sqrt(sum);
        cos = Hij[SetPosition(k, k)] / sqrt(sum);

        for (INT j = 0; j < this->A->GetColSize(); ++j) {
            Row2[j] = cos * Hij[SetPosition(k, j)] +
                      sin * Hij[SetPosition(k + 1, 1)];
            Row2[j + this->A->GetColSize()] = -sin * Hij[SetPosition(k, j)] +
                                              cos * Hij[SetPosition(k + 1, 1)];
        }

        tmpb[0] = cos * b[k] + sin * b[k + 1];
        tmpb[1] = -sin * b[k] + cos * b[k + 1];

        for (INT j = 0; j < this->A->GetColSize(); ++j) {
            Hij[SetPosition(k, j)] = Row2[j];
            Hij[SetPosition(k + 1, j)] = Row2[j + this->A->GetColSize()];
        }

        b[k] = tmpb[0];
        b[k + 1] = tmpb[1];
    }
}

void GMRES::SolveMin(std::vector<DBL> Hij, VEC b, VEC &x) {
    x.SetValues(b.GetSize(), 0.0);
    x[b.GetSize() - 1] = b[b.GetSize()] / Hij[SetPosition(b.GetSize(), b.GetSize())];

    for (INT j = b.GetSize() - 1; j > 0; --j) {
        for (INT k = b.GetSize(); k >= j; --k)
            b[j - 1] -= Hij[SetPosition(j, k)] * x[k];
        x[j - 1] = b[j - 1] / Hij[SetPosition(j, j)];
    }
}

FaspRetCode GMRES::Solve(const VEC &b, VEC &x) {
    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    // Local variables
    const INT len = b.GetSize();
    const int maxStag = MAX_STAG_NUM; // max number of stagnation checks
    const double solStagTol = 1e-4 * params.relTol; // solution stagnation tolerance
    const double solZeroTol = CLOSE_ZERO;

    int stagStep = 0, moreStep = 0;
    double resAbs = 1.0, resRel = 1.0, denAbs = 1.0, ratio = 0.0, resAbsOld = 1.0;
    double alpha, beta, tmpa, tmpb;

    PrintHead();

    // Initialize iterative method;
    numIter = 0;

    while (1) {
        A->Apply(x, rk); // A * x -> rk
        rk.XPAY(-1.0, b); // b - rk -> rk

        // Start from minIter instead of 0
        if (numIter == params.minIter) {
            resAbs = rk.Norm2();
            denAbs = (CLOSE_ZERO > resAbs) ? CLOSE_ZERO : resAbs;
            resRel = resAbs / denAbs;
            if (resRel < params.relTol || resAbs < params.absTol) break;
        }

        if (numIter >= params.minIter) PrintInfo(numIter, resRel, resAbs, ratio);

        vk = rk;

        norm2 = vk.Norm2();
        vk.Scale(1 / norm2);

        this->Arnoldi(Hij, cluster);
        betae1[0] = norm2;
        this->HouseHolder(Hij, betae1);
        this->SolveMin(Hij, betae1, yk);

        for (INT l = 0; l < cluster.GetNumber(); ++l) {
            cluster.Get(l, tmp);
            tmp.Scale(yk[l]);
            x += tmp;
        }

        ++numIter;

        // Save the best solution so far
        if (numIter >= params.safeIter && resAbs < resAbsOld) safe = x;

        // Apply several checks for robustness
        if (numIter >= params.minIter) {
            // Compute norm of residual and output iteration information if needed
            resAbs = rk.Norm2();
            resRel = resAbs / denAbs;
            ratio = resAbs / resAbsOld; // convergence ratio between two steps

            // Save the best solution so far
            if (numIter >= params.safeIter && resAbs < resAbsOld) safe = x;

            // Prepare for the next iteration
            if (numIter < params.maxIter) {
                // Save the residual for next iteration
                resAbsOld = resAbs;

                A->Apply(x, rk); // A * x -> rk
                rk.XPAY(-1.0, b); // b - rk -> rk
                resAbs = rk.Norm2();
                resRel = resAbs / denAbs;
                if (resRel < params.relTol || resAbs < params.absTol) break;
            }
        }
    }
    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (not(numIter == params.minIter && numIter == params.maxIter)) {
        this->norm2 = resAbs;
        this->normInf = rk.NormInf();
        PrintFinal(numIter, resRel, resAbs, ratio);
    }

    // Restore the saved best iteration if needed
    if (numIter > params.safeIter) x = safe;
}
