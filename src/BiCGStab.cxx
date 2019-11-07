/**
 * a demo
 */

#include "BiCGStab.hxx"
#include <ios>
using namespace std;

/// check and allocate memory
FaspRetCode BiCGStab::Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param){
    if (x.GetSize() != A.GetColSize() || b.GetSize() != A.GetRowSize() ||
        A.GetRowSize() != A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len = b.GetSize();
    try {
        this->r0star.SetValues(len, 0.0);
        this->tmp.SetValues(len, 0.0);
        this->apj.SetValues(len, 0.0);
        this->asj.SetValues(len, 0.0);
        this->pj.SetValues(len,0.0);
        this->rj.SetValues(len,0.0);
        this->sj.SetValues(len,0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    /// identical preconditioner operator by default
    if (lop == nullptr)
        lop = new IdentityLOP(len); //fff这样不太好,容易忘记delete这个lop. 可以强制传入 pc

    return FaspRetCode::SUCCESS;
}

/// build preconditioner operator
void BiCGStab::SetupPCD(LOP *lop){
    this->lop=lop;
}

FaspRetCode BiCGStab::Solve(const LOP &A, const VEC &b, VEC &x, IterParam &param) {

    INT len=b.GetSize();

    DBL alphaj,betaj,rjr0star,rjr0startmp,omegaj;

    // r0 = b - A * x_{0}
    A.Apply(x,this->tmp);
    this->rj.WAXPBY(1.0,b,-1.0,this->tmp);

    // r0_{*} = r0
    this->r0star=this->rj;

    // p0 = r0
    this->pj=this->rj;

    while(true) {

        // alpha_{j} = (rj,r0star)/(A * pj,r0star)
        rjr0star = this->rj.Dot(this->r0star);

        A.Apply(this->pj, this->apj);

        alphaj = rjr0star / this->apj.Dot(this->r0star);

        // sj = rj - alpha_{j} * A * p_{j}
        this->sj.WAXPBY(1.0, this->rj, -alphaj, this->apj);

        // omegaj = (A * sj,sj)/(A * sj,A * sj)
        A.Apply(this->sj, this->asj);

        omegaj = this->asj.Dot(this->sj) / this->asj.Dot(this->asj);

        // x_{j+1} = x_{j} + alpha_{j} * pj + omegaj * s_{j}
        this->tmp.WAXPBY(alphaj, this->pj, omegaj, this->sj);
        x.XPAY(1.0, this->tmp);


        // r_{j+1} = sj - omegaj * A * sj
        this->rj.WAXPBY(1.0, this->sj, -omegaj, this->asj);

        // control condition to exit iterations
        if(rj.Norm2()<param.absTol){
            cout<<"rj.Norm2() : "<<this->rj.Norm2()<<endl;
            break;
        }

        // beta_{j} = (r_{j+1}, r0^{*})/(rj, r0^{*}) * \frac{alpha_{j}}{omegaj}
        rjr0startmp = rjr0star;
        rjr0star = this->rj.Dot(this->r0star);
        betaj = rjr0star / rjr0startmp * alphaj / omegaj;

        // p_{j+1} = r_{j+1} + betaj * (p_{j} - omegaj * A * p_{j})
        this->tmp.WAXPBY(1.0, this->pj, -omegaj, this->apj);
        this->pj.WAXPBY(1.0, this->rj, betaj, this->tmp);
    }
}

/// clean preconditioner operator
void BiCGStab::CleanPCD(){
    LOP *lop;
    this->lop=lop;
}

/// Release temporary memory
void BiCGStab::Clean(){
    VEC zero;
    this->sj=zero;
    this->rj=zero;
    this->pj=zero;
    this->asj=zero;
    this->apj=zero;
    this->tmp=zero;
    this->r0star=zero;
}