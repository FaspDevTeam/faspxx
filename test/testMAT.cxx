/**
 * a test file
 */

#include "mat.hxx"
#include <iostream>
#include <vector>

using namespace std;

/**
 * the first MAT object :
 * \[
 * \begin{pmatrix}
 * 0 & 0 & 1 & 0 \\
 * 2 & 3 & 0 & 4 \\
 * 5 & 0 & 2 & 1 \\
 * 0 & 0 & 2 & 1 \\
 * \end{pmatrix}
 * \]
 * mat1.row=4, mat1.column=4, mat1.nnz=10
 * mat1.values={0,1,2,3,4,5,2,1,2,1}
 * mat1.colindex={0,2,0,1,3,0,2,3,2,3}
 * mat1.rowshift={0,2,5,8,10}
 * mat1.diag={0,3,6,9}
 */

int main() {
    vector<INT> vrow1(5), vcol1(10), vdia1(4);
    vector<DBL> vval1(10);
    int row1 = 4, col1 = 4, nnz1 = 10;
    //vrow
    vrow1[0] = 0;
    vrow1[1] = 2;
    vrow1[2] = 5;
    vrow1[3] = 8;
    vrow1[4] = 10;
    //vdia
    vdia1[0] = 0;
    vdia1[1] = 3;
    vdia1[2] = 6;
    vdia1[3] = 9;
    //vcol
    vval1[0] = 0;
    vval1[1] = 1;
    vval1[2] = 2;
    vval1[3] = 3;
    vval1[4] = 4;
    vval1[5] = 5;
    vval1[6] = 2;
    vval1[7] = 1;
    vval1[8] = 2;
    vval1[9] = 1;
    //vcol
    vcol1[0] = 0;
    vcol1[1] = 2;
    vcol1[2] = 0;
    vcol1[3] = 1;
    vcol1[4] = 3;
    vcol1[5] = 0;
    vcol1[6] = 2;
    vcol1[7] = 3;
    vcol1[8] = 2;
    vcol1[9] = 3;
    VEC vec(vval1);

    //MAT() : row(0), column(0), nnz(0), values(0), rowshift(0),
    //       colindex(0), diag(0) {};
    /**
     * output :
     * MAT() 1 :
     * Getnnz() : 0
     */
    MAT m0;
    cout << "MAT() 1 : " << endl;
    cout << "Getnnz() : " << m0.Getnnz() << endl;

    //MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
    //values, const std::vector<INT> rowshift, const std::vector<INT> colindex,
    //const std::vector<INT> diag);
    /**
     * MAT() 2 :
     * 0  0  1  0
     * 2  3  0  4
     * 5  0  2  1
     * 0  0  2  1
     */
    cout << "MAT() 2 : " << endl;
    MAT m1(row1, col1, nnz1, vval1, vrow1, vcol1, vdia1);
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
    //rowshift, const std::vector<INT> colindex, const std::vector<INT>
    //diag);
    /**
     * output :
     * MAT() 3 :
     * 1  0  1  0
     * 1  1  0  1
     * 1  0  1  1
     * 0  0  1  1
     */
    MAT m2(row1, col1, nnz1, vrow1, vcol1, vdia1);
    cout << "MAT() 3 : " << endl;
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m2.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT(const INT row, const INT column, const INT nnz, const std::vector<DBL>
    //values, const std::vector<INT> rowshift, const std::vector<INT>
    //colindex);
    /**
     * output :
     * MAT() 4 :
     * 0  0  1  0
     * 2  3  0  4
     * 5  0  2  1
     * 0  0  2  1
     */
    MAT m3(row1, col1, nnz1, vval1, vrow1, vcol1);
    cout << "MAT() 4 : " << endl;
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m3.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT(const INT row, const INT column, const INT nnz, const std::vector<INT>
    //rowshift, const std::vector<INT> colindex);
    /**
     * output :
     * MAT() 5 :
     * 1  0  1  0
     * 1  1  0  1
     * 1  0  1  1
     * 0  0  1  1
     */
    MAT m4(row1, col1, nnz1, vrow1, vcol1);
    cout << "MAT() 5 : " << endl;
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m4.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT(VEC &vec_obj);
    /**
     * output :
     * MAT() 6 :
     * 0  0  0  0  0  0  0  0  0  0
     * 0  1  0  0  0  0  0  0  0  0
     * 0  0  2  0  0  0  0  0  0  0
     * 0  0  0  3  0  0  0  0  0  0
     * 0  0  0  0  4  0  0  0  0  0
     * 0  0  0  0  0  5  0  0  0  0
     * 0  0  0  0  0  0  2  0  0  0
     * 0  0  0  0  0  0  0  1  0  0
     * 0  0  0  0  0  0  0  0  2  0
     * 0  0  0  0  0  0  0  0  0  1
     */
    MAT m5(vec);
    cout << "MAT() 6 : " << endl;
    for ( int j = 0; j < vec.GetSize(); j++ ) {
        for ( int k = 0; k < vec.GetSize(); k++ ) {
            cout << m5.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT(const std::vector<DBL> &vector_obj);
    /**
     * output :
     * MAT() 7 :
     * 0  0  0  0  0  0  0  0  0  0
     * 0  1  0  0  0  0  0  0  0  0
     * 0  0  2  0  0  0  0  0  0  0
     * 0  0  0  3  0  0  0  0  0  0
     * 0  0  0  0  4  0  0  0  0  0
     * 0  0  0  0  0  5  0  0  0  0
     * 0  0  0  0  0  0  2  0  0  0
     * 0  0  0  0  0  0  0  1  0  0
     * 0  0  0  0  0  0  0  0  2  0
     * 0  0  0  0  0  0  0  0  0  1
     */
    MAT m6(vval1);
    cout << "MAT() 7 : " << endl;
    for ( int j = 0; j < vval1.size(); j++ ) {
        for ( int k = 0; k < vval1.size(); k++ ) {
            cout << m6.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT(const MAT &mat);
    /**
     * output :
     * MAT() 8 :
     * 1  0  1  0
     * 1  1  0  1
     * 1  0  1  1
     * 0  0  1  1
     */
    MAT m7(m2);
    cout << "MAT() 8 : " << endl;
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m7.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //MAT &operator=(const MAT &mat);
    /**
     * output :
     * operator= :
     * 1  0  1  0
     * 1  1  0  1
     * 1  0  1  1
     * 0  0  1  1
     */
    MAT m8;
    m8.operator=(m2);
    cout << "operator= : " << endl;
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m8.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //void SetValues(const INT row, const INT column, const INT nnz,
    //               const std::vector<DBL> values, const std::vector<INT> rowshift,
    //               const std::vector<INT> colindex, const std::vector<INT> diag);
    /**
     * output :
     * SetValues 1 :
     * 0  0  1  0
     * 2  3  0  4
     * 5  0  2  1
     * 0  0  2  1
     */
    MAT m9;
    cout << "SetValues 1 : " << endl;
    m9.SetValues(row1, col1, nnz1, vval1, vrow1, vcol1, vdia1);
    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m9.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //void SetValues(const INT row, const INT column, const INT nnz,
    //               const std::vector<DBL> values, const std::vector<INT> rowshift,
    //               const std::vector<INT> colindex);
    /**
     * output :
     * SetValues 2 :
     * 0  0  1  0
     * 2  3  0  4
     * 5  0  2  1
     * 0  0  2  1
     */
    MAT m10;
    cout << "SetValues 2 : " << endl;
    m10.SetValues(row1, col1, nnz1, vval1, vrow1, vcol1);

    for ( int j = 0; j < row1; j++ ) {
        for ( int k = 0; k < col1; k++ ) {
            cout << m10.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //void GetSizes(INT &row, INT &col) const;
    /**
     * output :
     * GetSize() :
     * row : 4
     * col : 4
     */
    int tmprow, tmpcol;
    cout << "GetSizes() : " << endl;
    m10.GetSizes(tmprow, tmpcol);
    cout << "row : " << tmprow << endl;
    cout << "col : " << tmpcol << endl;

    //INT Getnnz() const;
    /**
     * output :
     * Getnnz : 10
     */
    cout << "Getnnz : " << m10.Getnnz() << endl;

    //FaspErrorType GetRow(const INT row, std::vector<DBL> &vector_obj) const;
    /**
     * output :
     * Getnnz :
     * 2  3  4
     */
    vector<DBL> vecrow;
    vecrow = m10.GetRow(1);
    cout << "GetRow : " << endl;
    for ( int j = 0; j < vecrow.size(); j++ ) {
        cout << vecrow[j] << "  ";
    }
    cout << endl;

    //FaspErrorType GetColumn(const INT column, std::vector<DBL> &vec_obj) const;
    /**
     * output :
     * GetColumn :
     * 3
     */
    vector<DBL> veccol;
    veccol = m10.GetColumn(1);
    cout << "GetColumn : " << endl;
    for ( int j = 0; j < veccol.size(); j++ ) {
        cout << veccol[j] << "  ";
    }
    cout << endl;

    //void GetDiag(std::vector<DBL> &vector_obj) const;
    /**
     * output :
     * GetDiag :
     * 0  3  2  1
     */
    vector<DBL> diag;
    diag.operator=(m10.GetDiag());
    cout << "GetDiag : " << endl;
    for ( int j = 0; j < diag.size(); j++ ) {
        cout << diag[j] << "  ";
    }
    cout << endl;

    //void Zero();
    /**
     * output :
     * Zero :
     * 0  0  0  0
     * 0  0  0  0
     * 0  0  0  0
     * 0  0  0  0
     */
    m10.Zero();
    cout << "Zero : " << endl;
    for ( int j = 0; j < tmprow; j++ ) {
        for ( int k = 0; k < tmpcol; k++ ) {
            cout << m10.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //void Copy(MAT &mat) const;
    /**
     * output :
     * m1.CopyTo() :
     * 0  0  1  0
     * 2  3  0  4
     * 5  0  2  1
     * 0  0  2  1
     */
    MAT m11;
    cout << "m1.CopyTo() : " << endl;
    m1.CopyTo(m11);
    for ( int j = 0; j < tmprow; j++ ) {
        for ( int k = 0; k < tmpcol; k++ ) {
            cout << m11.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //void Scale(const DBL a)
    /**
     * output :
     * Scale :
     * 0  0  2.87  0
     * 5.74  8.61  0  11.48
     * 14.35  0  5.74  2.87
     * 0  0  5.74  2.87
     */
    cout << "Scale : " << endl;
    m1.Scale(2.87);
    for ( int j = 0; j < tmprow; j++ ) {
        for ( int k = 0; k < tmpcol; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //void Shift(const DBL a);
    /**
     * output :
     * Shift :
     * 3.14  0  2.87  0
     * 5.74  11.75  0  11.48
     * 14.35  0  8.88  2.87
     * 0  0  5.74  6.01
     */
    m1.Shift(3.14);
    cout << "Shift : " << endl;
    for ( int j = 0; j < tmprow; j++ ) {
        for ( int k = 0; k < tmpcol; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //FaspErrorType MAT::MultVec(const VEC vec_x, VEC &vec_b) const
    /**
     * output :
     * MultVec :
     * 0.314  0.314  0.314  0.314
     * 1.88714  9.09658  8.1954  3.6895
     */
    VEC v(4, 0.314);
    VEC w;
    w.operator=(m1.MultVec(v));
    cout << "MultVec : " << endl;
    for ( int j = 0; j < 4; j++ )
        cout << v[j] << "  ";
    cout << endl;
    for ( int j = 0; j < 4; j++ )
        cout << w[j] << "  ";
    cout << endl;

    //void Transpose();
    /**
     * Transpose :
     * 3.14  5.74  14.35  0
     * 0  11.75  0  0
     * 2.87  0  8.88  5.74
     * 0  11.48  2.87  6.01
     */
    m1.Transpose();
    cout << "Transpose : " << endl;
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //FaspErrorType MultTransposeAdd(const VEC vec1,const VEC vec2, VEC &vec3) const
    /**
     * output :
     * 3.77428  18.1932  16.3908  7.379
     */
    VEC v3;
    MAT m12;
    m1.CopyTo(m12);
    cout << "MultTransposeAdd : " << endl;
    v3.operator=(m12.MultTransposeAdd(v, w));
    for ( int j = 0; j < 4; j++ ) {
        cout << v3[j] << "  ";
    }
    cout << endl;

    //FaspErrorType Add(const DBL a, const DBL b, const MAT mat)
    /**
     * output :
     * Add 1 :
     * 3.14 5.74  15.35  0
     * 2  14.75  0  4
     * 7.87  0  10.88  6.74
     * 0 11.48  4.87  7.01
     */
    MAT m13(row1, col1, nnz1, vval1, vrow1, vcol1, vdia1);
    cout << "Add 1 : " << endl;
    m12.Add(1, 1, m13);
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m12.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //friend FaspErrorType
    //Add(const DBL a, const MAT mat1, const DBL b, const MAT mat2,
    //    MAT &mat3);
    /**
     * output :
     * Add 2 :
     * 3.8622  7.0602  27.5205  0
     * 19.74  44.0625  0  39.48
     * 52.8801  0  30.6624  16.9302
     * 0  14.1204  23.2701  17.2623
     */
    MAT m14(m1);
    MAT m15;
    cout << "Add 2 : " << endl;
    m15.operator=(Add(1.23, m1, 9.87, m13));
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m15.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //friend FaspErrorType Mult2(const MAT mat1, const MAT mat2, MAT &mat3);
    /**
     * output :
     * Mult2 :
     * 83.23  17.22 31.84  37.31
     * 23.5  35.25  0  47
     * 44.4  0  32.11  14.62
     * 37.31  34.44  17.76  54.8
     * MultLeft :
     * 83.23  17.22  31.84  37.31
     * 23.5  35.25  0  47
     * 44.4  0  32.11  14.62
     * 37.31  34.44  17.76  54.8
     * MultRight :
     * 44.4  0  32.11  14.62
     * 386.2  277.95  134.72  434.82
     * 542.26  120.54  241.18  270.59
     * 126.11  34.44  81.98  84.04
     */
    MAT m16;
    m16 = Mult2(m1, m13);
    cout << "Left : " << endl;
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }
    cout << "Right : " << endl;
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m13.GetElem(j, k) << "  ";
        }
        cout << endl;
    }
    cout << "Mult2 : " << endl;
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m16.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    cout << "MultLeft : " << endl;
    m1.MultLeft(m13);
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }


    cout << "MultRight : " << endl;
    m1.MultRight(m13);
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m1.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    //friend MAT ConvertCSR(const INT row, const INT column, const INT nnz,
    //                      const std::vector<DBL> values,
    //                      const std::vector<INT> rowshift,
    //                      const std::vector<INT> colindex);
    /**
     * output :
     * ConvertCSR :
     * 0  0  1  0
     * 2  3  0  4
     * 5  0  2  1
     * 0  0  2  1
     */
    cout << "ConvertCSR : " << endl;
    MAT m18;
    m18 = ConvertCSR(row1, col1, nnz1, vval1, vrow1, vcol1);
    for ( int j = 0; j < 4; j++ ) {
        for ( int k = 0; k < 4; k++ ) {
            cout << m18.GetElem(j, k) << "  ";
        }
        cout << endl;
    }

    return 0;
}