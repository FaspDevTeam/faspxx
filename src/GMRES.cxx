
#include "GMRES.hxx"

/// Setup the LGMRES method.
FaspRetCode LGMRES::Setup(const LOP& A){
    // Set solver type
    SetSolType(SOLType::GMRES);

    // Allocate memory for temporary vectors
    try {
        const INT len = A.GetColSize();
        p0.SetValues(len, 0.0);
        pk.SetValues(len, 0.0);
        r0.SetValues(len, 0.0);
        tmp.SetValues(len, 0.0);
        V.SetSize(len,this->params.restart);
        betae1.SetValues(this->params.restart+1);
        H.resize((this->params.restart+1)*this->params.restart);
    } catch (std::bad_alloc &ex) {
        return FaspRetCode::ERROR_ALLOC_MEM;
    }

    // Setup the coefficient matrix
    this->A = &A;

    // Print used parameters
    if ( params.verbose > PRINT_MIN ) PrintParam(std::cout);

    return FaspRetCode::SUCCESS;
}