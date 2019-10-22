/**
 * a header file about reading command parameters
 */

#ifndef READCOMMAND_HXX
#define READCOMMAND_HXX

#include "RetCode.hxx"
#include "MAT.hxx"
#include "Param.hxx"

// print help information
void Help();

// convert command line choices to integer
INT StringToInt(char *ch)

// read parameters from command lines
FaspRetCode ReadParam(int argc, char *args[], InitParam &init);

#endif //SRC_READCOMMAND_HXX
