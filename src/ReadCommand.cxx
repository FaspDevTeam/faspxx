/**
 * a source file about reading command parameters
 */

#include <fstream>
#include "ReadData.hxx"
#include "ReadCommand.hxx"

FaspRetCode ReadCommand(unsigned int argc, char *args[],
                        MAT &mat, VEC &rhs, VEC &initGuess) {
    FaspRetCode retCode = FaspRetCode::SUCCESS;

    switch (argc) {
        case 2: // only read matrix
            retCode = ReadCSRToMAT(args[1], mat);
            break;
        case 3: // read matrix and right hand item
            retCode = ReadCSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadVEC(args[2], rhs);
            break;
        case 4: // read matrix, right hand item and initial solution
            retCode = ReadCSRToMAT(args[1], mat);
            if (retCode < 0) break;
            retCode = ReadVEC(args[2], rhs);
            if (retCode < 0) break;
            retCode = ReadVEC(args[3], initGuess);
            break;
        default: // if no data file is specified, trigger error mechanism
            std::cout << "### ERROR: No data file specified!" << std::endl;
            retCode = FaspRetCode::ERROR_INPUT_PAR;
            break;
    }

    return retCode;
}