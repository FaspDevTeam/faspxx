
#include "GMRES.hxx"

FaspRetCode RGMRES::Setup(const LOP &A) {
    // Set solver type
    SetSolType(SOLType::CG);

    this->params.restart=this->params.restart>this->params.maxIter?this->params
            .maxIter:this->params.restart;

    // Allocate memory for temporary vectors
    while(this->params.restart>5){
        try {
            const INT len = A.GetColSize();

            vk.SetValues(len, 0.0);
            wk.SetValues(len,0.0);
            p0.SetValues(len,0.0);
            pk.SetValues(len, 0.0);
            qk.SetValues(len, 0.0);
            var.SetValues(len,0.0);

            hh.resize(this->params.restart+1);
            for(int j=0;j<this->params.restart;++j)
                hh[j].resize(this->params.restart);

            cos.resize(this->params.restart+1);
            sin.resize(this->params.restart);

            norms.resize(this->params.maxIter+1);

            VEC testVariable(len,3.14);
            for(int j=0;j<this->params.restart;++j)
                queue_v.push(testVariable);

        } catch (std::bad_alloc &ex) {

            this->params.restart-=5;

            while(!queue_v.empty())
                queue_v.pop();
        }
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}