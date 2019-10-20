/**
 * a header file about reading command parameters
 */

#ifndef READCOMMAND_HXX
#define READCOMMAND_HXX

#include "RetCode.hxx"
#include "MAT.hxx"


/// read rhs or inital solution
static FaspRetCode ReadRhs(char *filename, VEC &rhs);

/// convert CSR format to CSRx matrix
FaspRetCode ReadCSRToMAT(char *filename, MAT &mat);

/// read command line parameters to achieve coefficient matrix, right hand side, and
/// initial solution
FaspRetCode ReadCommand(unsigned int argc, char *args[], MAT &mat, VEC &rhs, VEC
&initGuess);

#endif //SRC_READCOMMAND_HXX
