/**
 * a function for reading files
 */

#include "read.hxx"

FaspErrorCode ReadMtx(const char *filename, INT &row, INT &column, INT &nnz,
                      std::vector<INT> &rowind, std::vector<INT> &colind,
                      std::vector<DBL> &data) {

    FILE *fp = fopen(filename, "r");

    if ( fp == nullptr )
        return FaspErrorCode::ERROR_OPEN_FILE;

    std::cout << __FUNCTION__ << " : reading file " << filename << " ...\n"
              << std::endl;

    if (fscanf(fp,"%d %d %d", &row, &column, &nnz) <= 0 )
        return FaspErrorCode::ERROR_INPUT_FILE;

    if ( row <= 0 || column <= 0 || nnz <= 0 )
        return FaspErrorCode::ERROR_INPUT_FILE;

    rowind.resize(nnz);
    colind.resize(nnz);
    data.resize(nnz);

    INT rowvalue, columnvalue;
    DBL value;

    // read data from file
    INT count = 0;
    while ( count < nnz ) {
        if (fscanf(fp,"%d %d %lf", &rowvalue, &columnvalue, &value) != EOF) {
            rowind[count] = rowvalue - 1;
            colind[count] = columnvalue - 1;
            data[count] =value;
            count++;
        } else {
            return FaspErrorCode::ERROR_INPUT_FILE;
        }
    }

    //close connection
    fclose(fp);

    return FaspErrorCode::SUCCESS;
}



void MtxtoCSRx(INT row,INT column,INT nnz,std::vector<INT> rowind,
                        std::vector<INT> colind,std::vector<DBL> data,MAT &mat){

    std::vector<INT> tmpshift(row+1);
    std::vector<INT> csrshift(row+1);
    std::vector<INT> csrindex(nnz);
    std::vector<INT> csrvalues(nnz);

    // convert data format from MTX to CSR
    tmpshift[0] = 0;
    for ( INT j = 0; j < nnz; j++ )
        tmpshift[rowind[j] + 1]++;

    csrshift[0] = 0;
    for ( INT j = 1; j < row + 1; j++ ) {
        csrshift[j] = csrshift[j - 1] + tmpshift[j];
        tmpshift[j] = csrshift[j];
    }

    INT iind, jind;
    for ( INT j = 0; j < nnz; j++ ) {
        iind = rowind[j];
        jind = tmpshift[iind];
        csrindex[jind] = colind[j];
        csrvalues[jind] = data[j];
        tmpshift[iind] = ++jind;
    }

    //convert CSR format to CSRx format
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

    std::vector<INT> realcolindex(realnnz);
    std::vector<DBL> realvalues(realnnz);

    INT count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = realshift[j];
        end = realshift[j + 1];
        if ( begin == end ) {
            realcolindex[count] = j;
            realvalues[count] = 0.0;
            count++;
            continue;
        }
        if ( begin == end - 1 ) {
            if ( csrindex[begin] == j ) {
                realcolindex[count] = j;
                realvalues[count] = csrvalues[begin];
                count++;
            } else {
                if ( csrindex[begin] > j ) {
                    realcolindex[count] = j;
                    realvalues[count] = 0.0;
                    count++;
                    realcolindex[count] = csrindex[begin];
                    realvalues[count] = csrvalues[begin];
                    count++;
                }
                if ( csrindex[begin] < j ) {
                    realcolindex[count] = csrindex[begin];
                    realvalues[count] = csrvalues[begin];
                    count++;
                    realcolindex[count] = j;
                    realvalues[count] = 0.0;
                    count++;
                }
            }
            continue;
        }
        if ( begin == end - 2 ) {
            if ( csrindex[begin + 1] < j && csrindex[begin] < csrindex[begin + 1] ) {
                realcolindex[count] = csrindex[begin];
                realvalues[count] = csrvalues[begin];
                count++;
                realcolindex[count] = csrindex[begin + 1];
                realvalues[count] = csrvalues[begin + 1];
                count++;
                realcolindex[count] = j;
                realvalues[count] = 0.0;
                count++;
            }
            if ( csrindex[begin] < j && csrindex[begin] == csrindex[begin + 1] ) {
                realcolindex[count] = csrindex[begin];
                realvalues[count] += (csrvalues[begin] + csrvalues[begin + 1]);
                count++;
                realcolindex[count] = j;
                realvalues[count] = 0.0;
                count++;
            }
            if ( csrindex[begin] < j && csrindex[begin + 1] == j ) {
                realcolindex[count] = csrindex[begin];
                realvalues[count] = csrvalues[begin];
                count++;
                realcolindex[count] = j;
                realvalues[count] = csrvalues[begin + 1];
                count++;
            }
            if ( csrindex[begin] == j && csrindex[begin + 1] == j ) {
                realcolindex[count] = j;
                realvalues[count] += (csrvalues[begin] + csrvalues[begin + 1]);
                count++;
            }
            if ( csrindex[begin] == j && csrindex[begin + 1] > j ) {
                realcolindex[count] = j;
                realvalues[count] = csrvalues[begin];
                count++;
                realcolindex[count] = csrindex[begin + 1];
                realvalues[count] = csrvalues[begin + 1];
                count++;
            }
            if ( csrindex[begin] > j && csrindex[begin] == csrindex[begin + 1] ) {
                realcolindex[count] = j;
                realvalues[count] = 0.0;
                count++;
                realcolindex[count] = csrindex[begin];
                realvalues[count] += (csrvalues[begin] + csrvalues[begin + 1]);
                count++;
            }
            if ( csrindex[begin] > j && csrindex[begin] < csrindex[begin + 1] ) {
                realcolindex[count] = j;
                realvalues[count] = 0.0;
                count++;
                realcolindex[count] = csrindex[begin];
                realvalues[count] = csrvalues[begin];
                count++;
                realcolindex[count] = csrindex[begin + 1];
                realvalues[count] = csrvalues[begin + 1];
                count++;
            }
            continue;
        }
        for ( INT k = begin; k < end - 1; k++ ) {
            if ( csrindex[k + 1] < j && csrindex[k] < csrindex[k + 1] ) {
                realcolindex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
            }
            if ( csrindex[k] < j && csrindex[k] == csrindex[k + 1] ) {
                realcolindex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
            }
            if ( csrindex[k] < j && csrindex[k + 1] == j ) {
                realcolindex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
            }
            if ( csrindex[k] == j && csrindex[k + 1] == j ) {
                realcolindex[count] = j;
                realvalues[count] += csrvalues[k];
            }
            if ( csrindex[k] < j && csrindex[k + 1] > j ) {
                realcolindex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
                realcolindex[count] = j;
                realvalues[count] = 0.0;
                count++;
            }
            if ( csrindex[k] == j && csrindex[k + 1] > j ) {
                realcolindex[count] = j;
                realvalues[count] += csrvalues[k];
                count++;
            }
            if ( csrindex[k] > j && csrindex[k] == csrindex[k + 1] ) {
                realcolindex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
            }
            if ( csrindex[k] > j && csrindex[k + 1] > csrindex[k] ) {
                realcolindex[count] = csrindex[k];
                realvalues[count] += csrvalues[k];
                count++;
            }
        }
        if ( csrindex[end - 2] < csrindex[end - 1] && csrindex[end - 1] < j ) {
            realcolindex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
            realcolindex[count] = j;
            realvalues[count] = 0.0;
            count++;
        }
        if ( csrindex[end - 2] == csrindex[end - 1] && csrindex[end - 1] < j ) {
            realcolindex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
            realcolindex[count] = j;
            realvalues[count] = 0.0;
            count++;
        }
        if ( csrindex[end - 2] < csrindex[end - 1] && csrindex[end - 1] == j ) {
            realcolindex[count] = j;
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] == csrindex[end - 1] && csrindex[end - 1] == j ) {
            realcolindex[count] = j;
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] < j && csrindex[end - 1] > j ) {
            realcolindex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] > j && csrindex[end - 1] > csrindex[end - 2] ) {
            realcolindex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] == j && csrindex[end - 1] > j ) {
            realcolindex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
        if ( csrindex[end - 2] > j && csrindex[end - 2] == csrindex[end - 1] ) {
            realcolindex[count] = csrindex[end - 1];
            realvalues[count] += csrvalues[end - 1];
            count++;
        }
    }

    std::vector<INT> realdiag(row > column ? column : row);

    count = 0;
    for ( INT j = 0; j < row; j++ ) {
        begin = realshift[j];
        end = realshift[j + 1];
        for ( INT k = begin; k < end; k++ ) {
            if ( realcolindex[k] == j ) {
                realdiag[count] = k;
                count++;
            }
        }
    }

    mat.SetValues(row, column, realnnz, realvalues, realshift, realcolindex,
                  realdiag);

}