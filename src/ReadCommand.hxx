/**
 * a header file about reading command parameters
 */

#ifndef READCOMMAND_HXX
#define READCOMMAND_HXX

#include "RetCode.hxx"
#include "MAT.hxx"

/// read rhs or inital solution
FaspRetCode ReadRhs(char *filename,VEC &rhs);

/// convert CSR format to CSRx matrix
FaspRetCode CSRToMAT(char *filename,MAT &mat);

/// read command parameters to achieve cofficient matrix, right hand item,
/// initial solution.
FaspRetCode ReadCommad(unsigned int argc, char *args[], MAT &mat, VEC &rhs, VEC
    &initGuess);

#endif //SRC_READCOMMAND_HXX
