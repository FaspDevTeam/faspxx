/**
 * BiCGStab algorithm
 */

#include "BiCGStab.hxx"
#include "PCG.hxx"

/// check and allocate memory
FaspRetCode BiCGStab::Setup(const LOP& A,const VEC& b,VEC& x,const IterParam& param){
    if (x.GetSize() != A.GetColSize() || b.GetSize() != A.GetRowSize() ||
        A.GetRowSize() != A.GetColSize())
        return FaspRetCode::ERROR_NONMATCH_SIZE;

    INT len = b.GetSize();
    try {
        this->r.SetValues(len,0.0);
        this->rt.SetValues(len,0.0);
        this->p.SetValues(len,0.0);
        this->v.SetValues(len,0.0);
        this->ph.SetValues(len,0.0);
        this->xhalf.SetValues(len,0.0);
        this->s.SetValues(len,0.0);
        this->sh.SetValues(len,0.0);
        this->t.SetValues(len,0.0);
        this->xmin.SetValues(len,0.0);
        this->tmp.SetValues(len,0.0);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    /// identical preconditioner operator by default
    if (this->lop == nullptr)
        this->lop = new IdentityLOP(len); //fff这样不太好,容易忘记delete这个lop. 可以强制传入 pc

    return FaspRetCode::SUCCESS;
}

/// build preconditioner operator
void BiCGStab::SetupPCD(const LOP* lop){
    this->lop=lop;
}

FaspRetCode BiCGStab::Solve(const LOP& A, const VEC& b, VEC& x,IterParam& param){
    const INT    m = b.GetSize();

    // local variables
    DBL     n2b,tolb;
    INT      iter=0, stag = 1, moresteps = 1, maxmsteps=1;
    INT      flag, maxstagsteps, half_step=0;
    DBL     absres0 = 1e20, absres = 1e20, relres = 1e20;
    DBL     alpha,beta,omega,rho,rho1,rtv,tt;
    DBL     normr,normr_act,normph,normx,imin;
    DBL     norm_sh,norm_xhalf,normrmin,factor;

    // Output some info for debuging
    if ( param.verbose > PRINT_NONE ) printf("\nCalling BiCGstab solver (CSR) ...\n");

    // this->r = b-A*u
    this->r=b;
    n2b = this->r.Norm2();

    flag = 1;
    this->xmin=x;
    imin = 0;

    iter = 0;

    tolb = n2b*param.absTol;

    // this->r = this->r - A * x
    //fasp_blas_dcsr_aAxpy(-1.0, A, x, this->r);
    A.Apply(x,this->tmp);
    this->r.AXPY(-1.0,this->tmp);
    normr     = this->r.Norm2();
    normr_act = normr;
    relres    = normr/n2b;

    // if initial residual is small, no need to iterate!
    if ( normr <= tolb ) {
        flag = 0;
        iter = 0;
        goto FINISHED;
    }

    // output iteration information if needed
    fasp_itinfo(param.verbose,iter,relres,n2b,0.0);

    // shadow residual this->rt = this->r* := this->r
    this->rt=this->r;
    normrmin  = normr;

    rho = 1.0;
    omega = 1.0;
    stag = 0;
    alpha = 0.0;

    moresteps = 0;
    maxmsteps = 10;
    maxstagsteps = 3;

    // loop over maxit iterations (unless convergence or failure)
    for (iter=1;iter <= param.maxIter;iter++) {

        rho1 = rho;
        rho  = this->rt.Dot(this->r);

        if ((rho ==0.0 )|| (fabs(rho) >= 1e20 )) {
            flag = 4;
            goto FINISHED;
        }

        if (iter==1) {
            this->p=this->r;
        }
        else  {
            beta = (rho/rho1)*(alpha/omega);

            if ((beta == 0)||( fabs(beta) > 1e20 )) {
                flag = 4;
                goto FINISHED;
            }

            // this->p = this->r + beta * (this->p - omega * this->v);
            //fasp_blas_darray_axpy(m,-omega,this->v,this->p); //this->p=this->p - omega*this->v
            this->p.AXPY(-omega,this->v);
            //fasp_blas_darray_axpby(m,1.0, this->r, beta, this->p);  //this->p = 1.0*this->r +beta*this->p
            this->p.AXPBY(beta,1.0,this->r);
        }

        // pp = precond(this->p) ,this->ph
        this->lop->Apply(this->p,this->ph);

        // this->v = A*this->ph
        A.Apply(this->ph,this->v);
        rtv = this->rt.Dot(this->v);

        if (( rtv==0.0 )||( fabs(rtv) > 1e20 )){
            flag = 4;
            goto FINISHED;
        }

        alpha = rho/rtv;

        if ( fabs(alpha) > 1e20 ){
            flag = 4;
            FASPXX_WARNING("Divided by zero!");;
            goto FINISHED;
        }

        normx =  x.Norm2();
        normph = this->ph.Norm2();
        if (fabs(alpha)*normph < DBL_EPSILON*normx )
            stag = stag + 1;
        else
            stag = 0;

        // this->xhalf = x + alpha * this->ph;        // form the "half" iterate
        // this->s = this->r - alpha * this->v;             // residual associated with this->xhalf
        //fasp_blas_darray_axpyz(m, alpha, this->ph, x , this->xhalf);  // z= ax + y
        this->xhalf.WAXPBY(1.0,x,alpha,this->ph);
        //fasp_blas_darray_axpyz(m, -alpha, this->v, this->r, this->s);
        this->s.WAXPBY(1.0,this->r,-alpha,this->v);
        normr = this->s.Norm2();  // normr = norm(this->s);
        normr_act = normr;

        // compute reduction factor of residual ||this->r||
        absres = normr_act;
        factor = absres/absres0;
        fasp_itinfo(param.verbose,iter,normr_act/n2b,absres,factor);

        // check for convergence
        if ((normr <= tolb)||(stag >= maxstagsteps)||moresteps)
        {
            this->s=b;
            //fasp_blas_dcsr_aAxpy(-1.0,A,this->xhalf,this->s);
            A.Apply(this->xhalf,this->tmp);
            this->s.AXPY(-1.0,this->tmp);
            normr_act = this->s.Norm2();

            if (normr_act <= tolb) {
                // x = this->xhalf;
                x=this->xhalf;    // x = this->xhalf;
                flag = 0;
                imin = iter - 0.5;
                half_step++;
                if ( param.verbose >= PRINT_MORE )
                    printf("Flag = %d Stag = %d Itermin = %.1f Half_step = %d\n",
                           flag,stag,imin,half_step);
                goto FINISHED;
            }
            else {
                if ((stag >= maxstagsteps) && (moresteps == 0)) stag = 0;

                moresteps = moresteps + 1;
                if (moresteps >= maxmsteps) {
                    // if ~warned
                    flag = 3;
                    x=this->xhalf;
                    goto FINISHED;
                }
            }
        }

        if ( stag >= maxstagsteps ) {
            flag = 3;
            goto FINISHED;
        }

        if ( normr_act < normrmin ) // update minimal norm quantities
        {
            normrmin = normr_act;
            this->xmin=this->xhalf;
            imin = iter - 0.5;
            half_step++;
            if ( param.verbose >= PRINT_MORE )
                printf("Flag = %d Stag = %d Itermin = %.1f Half_step = %d\n",
                       flag,stag,imin,half_step);
        }

        // this->sh = precond(this->s)
        this->lop->Apply(this->s,this->sh);

        // this->t = A*this->sh;
        A.Apply(this->sh,this->t);

        // tt = this->t' * this->t;
        tt=this->t.Norm2();

        if ( (tt == 0) ||( tt >= 1e20 ) ) {
            flag = 4;
            goto FINISHED;
        }

        // omega = (this->t' * this->s) / tt;
        omega = this->s.Dot(this->t)/tt;
        if ( fabs(omega) > 1e20 ) {
            flag = 4;
            goto FINISHED;
        }

        norm_sh = this->sh.Norm2();
        norm_xhalf = this->xhalf.Norm2();

        if ( fabs(omega)*norm_sh < DBL_EPSILON*norm_xhalf )
            stag = stag + 1;
        else
            stag = 0;

        //fasp_blas_darray_axpyz(m, omega,this->sh,this->xhalf, x);  // x = this->xhalf + omega * this->sh;
        x.WAXPBY(1.0,this->xhalf,omega,this->sh);
        //fasp_blas_darray_axpyz(m, -omega, this->t, this->s, this->r);    // this->r = this->s - omega * this->t;
        this->r.WAXPBY(1.0,this->s,-omega,this->t);
        normr = this->r.Norm2();           // normr = norm(this->r);
        normr_act = normr;

        // check for convergence
        if ( (normr <= tolb) || (stag >= maxstagsteps) || moresteps )
        {
            this->r=b;
            //fasp_blas_dcsr_aAxpy(-1.0,A,x,this->r);
            A.Apply(x,this->tmp);
            this->r.AXPY(-1.0,this->tmp);
            normr_act = this->r.Norm2();
            if ( normr_act <= tolb ) {
                flag = 0;
                goto FINISHED;
            }
            else {
                if ((stag >= maxstagsteps) && (moresteps == 0)) stag = 0;

                moresteps = moresteps + 1;
                if ( moresteps >= maxmsteps ) {
                    flag = 3;
                    goto FINISHED;
                }
            }
        }

        // update minimal norm quantities
        if ( normr_act < normrmin ) {
            normrmin = normr_act;
            this->xmin=x;
            imin = iter;
        }

        if ( stag >= maxstagsteps )
        {
            flag = 3;
            goto FINISHED;
        }

        if ( param.verbose >= PRINT_MORE ) RealRes(relres);

        absres0 = absres;
    }   // for iter = 1 : maxit
    FINISHED:  // finish iterative method
    // returned solution is first with minimal residual
    if (flag == 0)
        relres = normr_act / n2b;
    else {
        this->r=b;
        //fasp_blas_dcsr_aAxpy(-1.0,A,this->xmin,this->r);
        A.Apply(this->xmin,this->tmp);
        this->r.AXPY(-1.0,this->tmp);
        normr = this->r.Norm2();

        if ( normr <= normr_act) {
            x=this->xmin;
            iter = imin;
            relres = normr/n2b;
        }
        else {
            iter = iter;
            relres = normr_act/n2b;
        }
    }

    PrintFinal(param.verbose,iter,param.maxIter,relres);

    if ( param.verbose >= PRINT_MORE )
        printf("Flag = %d Stag = %d Itermin = %.1f Half_step = %d\n",
               flag,stag,imin,half_step);

    if ( iter > param.maxIter )
        return ERROR_SOLVER_MAXIT;
    else
        return SUCCESS;
}

/// Clean up preconditioner. fff应该不需要这个:pc由外面换进来,应该由外面清理它的内存
void BiCGStab::CleanPCD() {
    LOP* lop;
    this->lop = lop;
}

/// Release temporary memory
void BiCGStab::Clean() {
    VEC zero;
    this->r=zero;
    this->rt=zero;
    this->p=zero;
    this->v=zero;
    this->ph=zero;
    this->xhalf=zero;
    this->s=zero;
    this->sh=zero;
    this->t=zero;
    this->xmin=zero;
    this->tmp=zero;
}