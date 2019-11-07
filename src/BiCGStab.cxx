/**
 * a demo
 */

#include "BiCGStab.hxx"

FaspRetCode BiCGStab::Solve(const LOP &A, const VEC &b, VEC &x, IterParam &param) {

    INT len=b.GetSize();

    VEC r0(len),p0(len),r0star(len),pj(len),rj(len),sj(len);
    VEC tmp(len),tmpb(len),apj(len),asj(len);

    DBL alpha,beta,rjr0star,omegaj;

    // r0 = b - A * x_{0}
    A.Apply(x,tmp);
    r0.WAXPBY(1.0,b,-1.0,tmp);

    // r0_{*} = r0
    r0star=r0;

    // p0 = r0
    p0=r0;
    while(true){

        // \alpha_{j} = (rj,r0star)/(A * pj,r0star)
        rj=r0;
        rjr0star=rj.Dot(r0star);
        A.Apply(pj,apj);
        alpha=rjr0star/apj.Dot(r0star);

        // sj = rj - alpha_{j} * A * p_{j}
        sj.WAXPBY(1.0,rj,-alpha,apj);

        // omegaj = (A * sj,sj)/(A * sj,A * sj)
        A.Apply(sj,asj);
        omegaj=asj.Dot(sj)/asj.Dot(asj);

        // x_{j+1} = x_{j} + alpha_{j} * pj + omegaj * s_{j}
        tmp.WAXPBY(alpha,pj,omegaj,sj);
        x.XPAY(1.0,tmp);

        // r_{j+1} = sj - omegaj * A * sj
        rj.WAXPBY(1.0,sj,-omegaj,asj);

        // beta_{j} = (r_{j+1}, r0^{*})/(rj, r0^{*}) * \frac{alpha_{j}}{omegaj}













    }

}