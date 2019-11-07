/**
 * a demo
 */

#include "BiCGStab.hxx"
#include <ios>
using namespace std;

FaspRetCode BiCGStab::Solve(const LOP &A, const VEC &b, VEC &x, IterParam &param) {

    for(int j=0;j<10;j++){
    }



    INT len=b.GetSize();

    VEC r0(len),p0(len),r0star(len),pj(len),rj(len),sj(len);
    VEC tmp(len),apj(len),asj(len);

    DBL alphaj,betaj,rjr0star,rjr0startmp,omegaj;

    // r0 = b - A * x_{0}
    A.Apply(x,tmp);
    r0.WAXPBY(1.0,b,-1.0,tmp);

    // r0_{*} = r0
    r0star=r0;

    INT count=0;

    // p0 = r0
    pj=r0;
    rj=r0;

    while(true) {

        // alpha_{j} = (rj,r0star)/(A * pj,r0star)
        rjr0star = rj.Dot(r0star);

        A.Apply(pj, apj);

        alphaj = rjr0star / apj.Dot(r0star);

        // sj = rj - alpha_{j} * A * p_{j}
        sj.WAXPBY(1.0, rj, -alphaj, apj);

        // omegaj = (A * sj,sj)/(A * sj,A * sj)
        A.Apply(sj, asj);

        omegaj = asj.Dot(sj) / asj.Dot(asj);

        // x_{j+1} = x_{j} + alpha_{j} * pj + omegaj * s_{j}
        tmp.WAXPBY(alphaj, pj, omegaj, sj);
        x.XPAY(1.0, tmp);


        // r_{j+1} = sj - omegaj * A * sj
        rj.WAXPBY(1.0, sj, -omegaj, asj);

        // control condition to exit iterations
        if(rj.Norm2()<param.absTol){
            cout<<"rj.Norm2() : "<<rj.Norm2()<<endl;
            break;
        }

        // beta_{j} = (r_{j+1}, r0^{*})/(rj, r0^{*}) * \frac{alpha_{j}}{omegaj}
        rjr0startmp = rjr0star;
        rjr0star = rj.Dot(r0star);
        betaj = rjr0star / rjr0startmp * alphaj / omegaj;

        // p_{j+1} = r_{j+1} + betaj * (p_{j} - omegaj * A * p_{j})
        tmp.WAXPBY(1.0, pj, -omegaj, apj);
        pj.WAXPBY(1.0, rj, betaj, tmp);
    }
}