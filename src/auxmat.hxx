/**
 * a header file about some anxiliary functions
 */

#ifndef SRC_AUXMAT_HXX
#define SRC_AUXMAT_HXX

#include "mat.hxx"

//! check GetElem function
FaspRetCode MatCheck_CrossOver(const MAT mat, const INT row, const INT column);

//! Check GetRow function
FaspRetCode MatCheck_CrossOverRow(const MAT mat,const INT row);

//! Check GetColumn function
FaspRetCode MatCheck_CrossOverColumn(const MAT mat,const INT column);

//! Check MultVec function
FaspRetCode MatCheck_MultVec(const MAT mat,const VEC vec);

//! Check MultTransposeAdd function
FaspRetCode MatCheck_MultTransposeAdd(const MAT mat,const VEC vec1,const VEC vec2);

//! Check Add function
FaspRetCode MatCheck_Add(const MAT mat1,const MAT mat2);

//! Check Mult function
FaspRetCode MatCheck_Mult(const MAT mat1,const MAT mat2);

//! Sort "colindex" on the order from small to large by its rows and change the
//! positions of "values" according to "colindex"
FaspRetCode Sort(const INT row,const std::vector<INT> rowshift,std::vector<INT>
        &colindex, std::vector<DBL> &values);

//! check whether the input data are CSRx
FaspRetCode CheckCSRx(const INT row, const INT column, const INT nnz,
              const std::vector<DBL> values,
              const std::vector<INT> rowshift,
              const std::vector<INT> colindex,
              const std::vector<INT> diag);

//! check whether the input data are CSR
FaspRetCode CheckCSR(const INT row, const INT column, const INT nnz,
             const std::vector<DBL> values,
             const std::vector<INT> rowshift,
             const std::vector<INT> colindex);

#endif //SRC_AUXILIARY_HXX

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/