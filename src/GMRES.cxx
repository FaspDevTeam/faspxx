
#include "GMRES.hxx"

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
        betae1.SetValues(len + 1, 0.0);
        safe.SetValues(len, 0.0);
        cluster.SetSize(len, this->params.restart);
        Hij.assign((len + 1) * len, 0);
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
    for (INT j = 0; j < this->params.restart; ++j) {
        for (INT i = 0; i <= j; ++i) {
            cluster.Get(j, vk);
            A->Apply(vk, tmp);
            cluster.Get(i, vk);
            Hij[SetPosition(i, j)] = vk.Dot(tmp);
        }
        wk = tmp;
        for (INT k = 0; k <= j; ++k) {
            cluster.Get(k, vk);
            vk.Scale(Hij[SetPosition(k, j)]);
            wk.AXPY(-1.0, vk);
        }
        Hij[SetPosition(j + 1, j)] = wk.Norm2();
        if (Hij[SetPosition(j + 1, j)] == 0)
            break;
        wk.Scale(1 / Hij[SetPosition(j + 1, j)]);
        vk = wk;
        cluster.Set(j + 1, vk);
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
                      sin * Hij[SetPosition(k + 1, j)];
            Row2[j + this->A->GetColSize()] = -sin * Hij[SetPosition(k, j)] +
                                              cos * Hij[SetPosition(k + 1, j)];
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
    x.SetValues(this->A->GetColSize(), 0.0);
    x[this->A->GetColSize() - 1] =
            b[this->A->GetColSize() - 1] / Hij[SetPosition(this->A->GetColSize() - 1, this->A->GetColSize() - 1)];
    for (INT j = this->A->GetColSize() - 1; j > 0; --j) {
        for (INT k = this->A->GetColSize() - 1; k >= j; --k)
            b[j - 1] -= Hij[SetPosition(j - 1, k)] * x[k];
        x[j - 1] = b[j - 1] / Hij[SetPosition(j - 1, j - 1)];
    }
}

FaspRetCode GMRES::Solve(const VEC &b, VEC &x) {

    FaspRetCode errorCode = FaspRetCode::SUCCESS;

    DBL tmpval;
    VEC vvv(this->A->GetColSize());

    // Initialize iterative method
    numIter = 0;

    std::cout<<"x : "<<numIter<<"  "<<x[0]<<"  "<<x[1]<<"  "<<x[2]<<"  "<<x[3]<<"  "<<x[4]<<std::endl;
    A->Apply(x, rk); // A * x -> rk
    std::cout<<"rk : "<<numIter<<"  "<<rk[0]<<"  "<<rk[1]<<"  "<<rk[2]<<"  "<<rk[3]<<"  "<<rk[4]<<std::endl;
    rk.XPAY(-1.0, b); // b - rk -> rk
    std::cout<<"rk : "<<numIter<<"  "<<rk[0]<<"  "<<rk[1]<<"  "<<rk[2]<<"  "<<rk[3]<<"  "<<rk[4]<<std::endl;
    vk = rk;
    tmpval = vk.Norm2();
    vk.Scale(1.0 / tmpval);
    std::cout<<"vk : "<<numIter<<"  "<<vk[0]<<"  "<<vk[1]<<"  "<<vk[2]<<"  "<<vk[3]<<"  "<<vk[4]<<std::endl;
    cluster.Set(0, vk);
    while (1) {
        Hij.assign((A->GetColSize() + 1) * A->GetColSize(), 0.0);
        this->Arnoldi(Hij, cluster);
        std::cout<<"Hij "<<numIter<<" : "<<std::endl;
        for(INT j=0;j<this->A->GetColSize()+1;++j){
            for(INT k=0;k<this->A->GetColSize();++k)
                std::cout<<Hij[j*this->A->GetColSize()+k]<<"  ";
            std::cout<<std::endl;
        }
        std::cout<<"cluster "<<numIter<<" : "<<std::endl;
        for(INT j=0;j<this->A->GetColSize();++j){
            cluster.Get(j,vvv);
            for(INT j=0;j<this->A->GetColSize();++j)
                std::cout<<vvv[j]<<"  ";
            std::cout<<std::endl;
        }


        betae1.SetValues(A->GetColSize() + 1, 0.0);
        betae1[0] = rk.Norm2();
        std::cout<<"betae1 : "<<numIter<<std::endl;
        for(INT j=0;j<this->A->GetColSize()+1;++j)
            std::cout<<betae1[j]<<"  ";
        std::cout<<std::endl;
        this->HouseHolder(Hij, betae1);
        std::cout<<"Hij HouseHolder : "<<numIter<<" : "<<std::endl;
        for(INT j=0;j<this->A->GetColSize()+1;++j){
            for(INT k=0;k<this->A->GetColSize();++k)
                std::cout<<Hij[j*this->A->GetColSize()+k]<<"  ";
            std::cout<<std::endl;
        }
        std::cout<<"betae1 HouseHolder : "<<numIter<<std::endl;
        for(INT j=0;j<this->A->GetColSize()+1;++j)
            std::cout<<betae1[j]<<"  ";
        std::cout<<std::endl;
        this->SolveMin(Hij, betae1, yk);
        std::cout<<"yk SolveMin : "<<numIter<<std::endl;
        for(INT j=0;j<this->A->GetColSize();++j)
            std::cout<<yk[j]<<"  ";
        std::cout<<std::endl;

        for (INT l = 0; l < cluster.GetNumber(); ++l) {
            cluster.Get(l, tmp);
            tmp.Scale(yk[l]);
            x += tmp;
        }
        std::cout<<"x solver : "<<numIter<<std::endl;
        for(INT j=0;j<this->A->GetColSize()+1;++j)
            std::cout<<x[j]<<"  ";
        std::cout<<std::endl;

        ++numIter;

        if (numIter < params.maxIter) {
            A->Apply(x, rk); // A * x -> rk
            rk.XPAY(-1.0, b); // b - rk -> rk
            vk = rk;
            tmpval = vk.Norm2();
            vk.Scale(1.0 / tmpval);
            cluster.Set(0, vk);
        } else {
            break;
        }
    }
    // If minIter == numIter == maxIter (preconditioner only), skip this
    if (not(numIter == params.minIter && numIter== params.maxIter)) {
        this->norm2 = rk.Norm2();
        this->normInf = rk.NormInf();
    }

    return errorCode;
}
