/**
 * a function for reading files
 */

#include <fstream>

#include "read.hxx"

/// \brief Read a MTX data file
FaspRetCode ReadMtx(const char *filename, INT &row, INT &col, INT &nnz,
                    std::vector<INT> &rowInd, std::vector<INT> &colInd,
                    std::vector<DBL> &values)
{
    std::cout << __FUNCTION__ << " : reading file " << filename << "..." << std::endl;

    // Open the file to read
    std::ifstream infile(filename);

    // Get matrix size and number of nonzeros
    infile >> row >> col >> nnz;
    if ( row <= 0 || col <= 0 || nnz <= 0 ) return FaspRetCode::ERROR_INPUT_FILE;

    // Reserve memory space for matrix data
    rowInd.resize(nnz);
    colInd.resize(nnz);
    values.resize(nnz);
    
    // Read data from file and store them in vectors
    INT count = 0, rowValue, colValue;
    DBL tmpValue;
    while ( infile >> rowValue >> colValue >> tmpValue ) {
         rowInd[count] = rowValue - 1;
         colInd[count] = colValue - 1;
         values[count] = tmpValue;
         count++;
    }

    if ( count != nnz ) return FaspRetCode::ERROR_INPUT_FILE;

    return FaspRetCode::SUCCESS;
}

/// \brief Convert MTX data to MAT data structure
FaspRetCode MtxToMAT(const INT row, const INT col, const INT nnz,
                     const std::vector<INT> &rowInd, const std::vector<INT> &colInd,
                     const std::vector<DBL> &values, MAT &mat)
{
    // Todo: Check whether this succeeded???
    std::vector<INT> tmpshift(row+1);
    std::vector<INT> csrshift(row+1);
    std::vector<INT> csrindex(nnz);
    std::vector<INT> csrvalues(nnz);

    // Todo: Separate the following into several functions!!!

    // Convert data format from MTX to CSR
    tmpshift[0] = 0;
    for ( INT j = 0; j < nnz; j++ ) tmpshift[rowInd[j] + 1]++;

    csrshift[0] = 0;
    for ( INT j = 1; j < row + 1; j++ ) {
        csrshift[j] = csrshift[j - 1] + tmpshift[j];
        tmpshift[j] = csrshift[j];
    }

    INT iind, jind;
    for ( INT j = 0; j < nnz; j++ ) {
        iind = rowInd[j];
        jind = tmpshift[iind];
        csrindex[jind] = colInd[j];
        csrvalues[jind] = values[j];
        tmpshift[iind] = ++jind;
    }

    // Convert CSR format to CSRx format
    INT begin, end;
    INT flag = 0;
    INT realnnz = 0;
    std::vector<INT> realshift(row+1);
    for ( INT j = 0; j < row + 1; j++ )
        realshift[j] = 0;

    for ( INT j = 0; j < row; j++ ) {
        begin = tmpshift[j];
        end = tmpshift[j + 1];
        if ( begin == end ) {
            realnnz++;
            realshift[j + 1] = realshift[j] + 1;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( csrindex[begin] == j ) {
                realnnz++;
                realshift[j + 1] = realshift[j] + 1;
            } else {
                realnnz += 2;
                realshift[j + 1] = realshift[j] + 2;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( csrindex[k] < csrindex[k + 1] ) {
                realnnz++;
                if ( csrindex[k] == j )
                    flag = 1;
            }
        }
        if ( csrindex[end - 2] < csrindex[end - 1] ) {
            realnnz++;
            if ( csrindex[end - 1] == j )
                flag = 1;
        }

        if ( flag != 1 ) {
            realnnz++;
            realshift[j + 1] = realshift[j] + end - begin + 1;
        } else {
            realshift[j + 1] = realshift[j] + end - begin;
            flag = 0;
        }
    }

    // Todo: Check whether this succeeded???
    std::vector<INT> realcolIndex(realnnz);
    std::vector<DBL> realvalues(realnnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = realshift[j];
        end = realshift[j + 1];
        if ( begin == end ) {
            realcolIndex[count] = j;
            realvalues[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( csrindex[begin] == j ) {
                realcolIndex[count] = j;
                realvalues[count] = csrvalues[begin];
                count++;
            } else {
                if ( csrindex[begin] > j ) {
                    realcolIndex[count] = j;
                    realvalues[count] = 0.0;
                    count++;
                    realcolIndex[count] = csrindex[begin];
                    realvalues[count] = csrvalues[begin];
                    count++;
                }
                if ( csrindex[begin] < j ) {
                    realcolIndex[count] = csrindex[begin];
                    realvalues[count] = csrvalues[begin];
                    count++;
                    realcolIndex[count] = j;
                    realvalues[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( csrindex[begin + 1] < j && csrindex[begin] < csrindex[begin + 1] ) {
                realcolIndex[count] = csrindex[begin];
                realvalues[count] = csrvalues[begin];
                count++;
                realcolIndex[count] = csrindex[begin + 1];
                realvalues[count] = csrvalues[begin + 1];
                count++;
                realcolIndex[count] = j;
                realvalues[count] = 0.0;
                count++;
            }
            if ( csrindex[begin] < j && csrindex[begin] == csrindex[begin + 1] ) {
                realcolIndex[count] = csrindex[begin];
                realvalues[count] += (csrvalues[begin] + csrvalues[begin + 1]);
                count++;
                realcolIndex[count] = j;
                realvalues[count] = 0.0;
                count++;
            }
            if ( csrindex[begin] < j && csrindex[begin + 1] == j ) {
                realcolIndex[count] = csrindex[begin];
                realvalues[count] = csrvalues[begin];
                count++;
                realcolIndex[count] = j;
                realvalues[count] = csrvalues[begin + 1];
                count++;
            }
            if ( csrindex[begin] == j && csrindex[begin + 1] == j ) {
                realcolIndex[count] = j;
                realvalues[count] += (csrvalues[begin] + csrvalues[begin + 1]);
                count++;
            }
            if ( csrindex[begin] == j && csrindex[begin + 1] > j ) {
                realcolIndex[count] = j;
                realvalues[count] = csrvalues[begin];
                count++;
                realcolIndex[count] = csrindex[begin + 1];
                realvalues[count] = csrvalues[begin + 1];
                count++;
            }
            if ( csrindex[begin] > j && csrindex[begin] == csrindex[begin + 1] ) {
                realcolIndex[count] = j;
                realvalues[count] = 0.0;
                count++;
                realcolIndex[count] = csrindex[begin];
                realvalues[count] += (csrvalues[begin] + csrvalues[begin + 1]);
                count++;
            }
            if ( csrindex[begin] > j && csrindex[begin] < csrindex[begin + 1] ) {
                realcolIndex[count] = j;
                realvalues[count] = 0.0;
                count++;
                realcolIndex[count] = csrindex[begin];
                realvalues[count] = csrvalues[begin];
                count++;
                realcolIndex[count] = csrindex[begin + 1];
                realvalues[count] = csrvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( csrindex[k + 1] < j && csrindex[k] < csrindex[k + 1] ) {
                realcolIndex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
            }
            if ( csrindex[k] < j && csrindex[k] == csrindex[k + 1] ) {
                realcolIndex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
            }
            if ( csrindex[k] < j && csrindex[k + 1] == j ) {
                realcolIndex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
            }
            if ( csrindex[k] == j && csrindex[k + 1] == j ) {
                realcolIndex[count] = j;
                realvalues[count] += csrvalues[k];
            }
            if ( csrindex[k] < j && csrindex[k + 1] > j ) {
                realcolIndex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
                realcolIndex[count] = j;
                realvalues[count] = 0.0;
                count++;
            }
            if ( csrindex[k] == j && csrindex[k + 1] > j ) {
                realcolIndex[count] = j;
                realvalues[count] += csrvalues[k];
                count++;
            }
            if ( csrindex[k] > j && csrindex[k] == csrindex[k + 1] ) {
                realcolIndex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
            }
            if ( csrindex[k] > j && csrindex[k + 1] > csrindex[k] ) {
                realcolIndex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
            }
        }
        if ( csrindex[end - 2] < csrindex[end - 1] && csrindex[end - 1] < j ) {
            realcolIndex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
            realcolIndex[count] = j;
            realvalues[count] = 0.0;
            count++;
        }
        if ( csrindex[end - 2] == csrindex[end - 1] && csrindex[end - 1] < j ) {
            realcolIndex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
            realcolIndex[count] = j;
            realvalues[count] = 0.0;
            count++;
        }
        if ( csrindex[end - 2] < csrindex[end - 1] && csrindex[end - 1] == j ) {
            realcolIndex[count] = j;
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] == csrindex[end - 1] && csrindex[end - 1] == j ) {
            realcolIndex[count] = j;
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] < j && csrindex[end - 1] > j ) {
            realcolIndex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] > j && csrindex[end - 1] > csrindex[end - 2] ) {
            realcolIndex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] == j && csrindex[end - 1] > j ) {
            realcolIndex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] > j && csrindex[end - 2] == csrindex[end - 1] ) {
            realcolIndex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
    }

    // Generate diagonal pointer for CSRx
    std::vector<INT> realdiag(row > col ? col : row);
    // Todo: Check whether this succeeded???

    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = realshift[j];
        end = realshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( realcolIndex[k] == j ) {
                realdiag[count] = k;
                count++;
            }
        }
    }

    // Set values for MAT matrix
    mat.SetValues(row, col, realnnz, realvalues, realshift, realcolIndex, realdiag);

    return FaspRetCode::SUCCESS;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/