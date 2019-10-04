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
        this->pc = 1;
    } catch ( std::bad_alloc &ex ) {
        throw (FaspBadAlloc(__FILE__, __FUNCTION__, __LINE__));
    }

    return FaspRetCode::SUCCESS;
}

FaspRetCode PCG::Start(VEC &x, INT &iter,StopType type) {

    if ( x.GetSize() != A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    const INT MaxStag=20,MaxRestartStep=20;
    const INT maxdiff=rtol*1e-4;
    const DBL solinftol=1e-20;

    // local variables
    INT stag=1,morestep=1,flag=0;
    DBL absres=1e+20,abstmp=1e+20;
    DBL relres=1e+20,norm=1e+20,normtmp=1e+20;
    DBL reldiff,factor,norminf;
    DBL alpha,beta,tmpa,tmpb;

    iter=0;

    VEC rk,pk,zk,tmp;

    // rk = b - A * x
    tmp=A.MultVec(x);
    rk.Add(1.0,b,-1.0,tmp);

    if(pc==1)
        ApplyPreconditioner();
    else
        zk=rk;

    // compute initial residuals
    switch(type){
        case STOP_REL_RES:
            abstmp=rk.Norm2();
            normtmp=(1e-20>abstmp)?1e-20:abstmp;
            relres=abstmp/normtmp;
            break;
        case STOP_REL_PRECRES:
            abstmp=sqrt(rk.Dot(zk));
            normtmp=(1e-20>abstmp)?1e-20:abstmp;
            relres=abstmp/normtmp;
            break;
        case STOP_MOD_REL_RES:
            abstmp=rk.Norm2();
            norm=(1e-20>x.Norm2())?1e-20:x.Norm2();
            relres=abstmp/norm;
            break;
        default:
            std::cout<<"### ERROR: Unknown stopping type! ["<<__FUNCTION__<<"]"<<std::endl;
            return FaspRetCode ::ERROR_INPUT_PAR;
    }

    if(relres<rtol || abstmp<1e-3*rtol) return FaspRetCode ::SUCCESS;

    pk=zk;
    tmpa=zk.Dot(rk);

    while(iter++<maxIt) {

        tmp = A.MultVec(pk);
        tmpb = tmp.Dot(pk);
        if (fabs(tmpb) > 1e-20)
            alpha = tmpa / tmpb;
        else
            return FaspRetCode::ERROR_DIVIDE_ZERO;

        x.Add(1.0, alpha, pk);
        tmp=A.MultVec(pk);
        rk.Add(1.0,-alpha,pk);

        switch (type) {
            case STOP_REL_RES:
                absres = rk.Norm2();
                relres = absres / normtmp;
                break;
            case STOP_REL_PRECRES:
                if (this->pc == 1)
                    ApplyPreconditioner();
                else
                    zk = rk;

                absres = sqrt(fabs(zk.Dot(rk)));
                relres = absres / normtmp;
                break;
            case STOP_MOD_REL_RES:
                absres = rk.Norm2();
                relres = absres / norm;
                break;
        }

        if (absres / abstmp > 0.9) {
            norminf = x.NormInf();
            if (norminf <= solinftol) return FaspRetCode::ERROR_SOLVER_SOLSTAG;

            norm = x.Norm2();
            reldiff = fabs(alpha) * pk.Norm2() / norm;

            if ((stag <= MaxStag) && (reldiff < maxdiff)) {

                tmp = A.MultVec(x);
                rk.Add(1.0, b, -1.0, tmp);

                switch (type) {
                    case STOP_REL_RES:
                        absres = rk.Norm2();
                        relres = absres / normtmp;
                        break;
                    case STOP_REL_PRECRES:
                        if (pc == 1)
                            ApplyPreconditioner();
                        else
                            zk = rk;

                        absres = sqrt(fabs(zk.Dot(rk)));
                        relres = absres / normtmp;
                        break;
                    case STOP_MOD_REL_RES:
                        absres = rk.Norm2();
                        relres = absres / norm;
                        break;
                }

                if (relres < rtol)
                    break;
                else {
                    if (stag >= MaxStag) return FaspRetCode::ERROR_SOLVER_STAG;

                    pk.SetValues(pk.GetSize(), 0.0);
                    ++stag;
                }
            }
        }

        if (relres < rtol) {

            DBL updated_relres = relres;
            tmp = A.MultVec(x);
            rk.Add(1.0, b, -1.0, tmp);

            switch (type) {
                case STOP_REL_RES:
                    absres = rk.Norm2();
                    relres = absres / normtmp;
                    break;
                case STOP_REL_PRECRES:
                    if (pc == 1)
                        ApplyPreconditioner();
                    else
                        zk = rk;

                    absres = sqrt(zk.Dot(rk));
                    relres = absres / normtmp;
                    break;
                case STOP_MOD_REL_RES:
                    absres = rk.Norm2();
                    relres = absres / norm;
                    break;
            }

            if (relres < rtol) break;

            if (morestep >= MaxRestartStep) return FaspRetCode::ERROR_SOLVER_TOLSMALL;

            pk.SetValues(pk.GetSize(), 0.0);
            ++morestep;
        }

        abstmp = absres;

        if (type != STOP_REL_PRECRES) {
            if (pc == 1)
                ApplyPreconditioner();
            else
                zk = rk;
        }

        tmpb = zk.Dot(rk);
        beta = tmpb / tmpa;
        tmpa = tmpb;

        pk.Add(beta,1.0,zk);
    }

    if(iter>maxIt) return FaspRetCode ::ERROR_SOLVER_MAXIT;

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