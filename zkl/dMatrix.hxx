/**
 *  Matrix class
 */

#include"dVector.hxx"

//------------------------------------------------------------------------------
class Matrix{
    private:
        //! row number of matrix : m
        int row;
        //! column of matrix : n
        int col;
        //! number of nonzero entries
        int nnz;
        //! integer array of row pointers, the size is m+1
        int *IA;
        //! integer array of column indexes, the size is nnz
        int *JA;
        //! nonzero entries of A
        double *val;
        //! integer array of diagonal indexes, the size is min(m,n)
        int *dia;
    public:
        Matrix(){
            row=1;
            col=1;
            nnz=0;
            IA=NULL;
            JA=NULL;
            val=NULL;
            dia=NULL;
        }
        FaspErrorCode Create(int row,int column,int nnz,int *IA,int *JA,
        double *val,int *dia){
            if(row<1 || column<1){
                //! 5 represents illegal row and column values of matrices
                return 5;
            }
            ////attention: need to judge the relation among these values
            this->row=row;
            this->col=column;
            this->nnz=nnz;
            this->IA=IA;
            this->JA=JA;
            this->val=val;
            this->dia=dia;

            return 0;
        }
        //// attention and question : if there is a matrix , its elements in 
        //// a row are all zero, 
        //// can CSR still store the matrix?
        //! get a value from the matrix
        FaspErrorCode GetValue(int row,int column,double *value){
            // attention : the row and column index is starting from 1
            if(row<1 || column<1){
                //! 5 represents illegal row and column values of matrices
                return 5;
            }
            int start=IA[row];
            int end=IA[row+1];
            int j;
            for(j=start;j<end;j++){
                if(column==JA[j]){
                    *value=val[j];
                }
            }

            return 0;
        }
        //! get a row's values from the matrix
        FaspErrorCode GetRowValues(int row,int **column,double **values){
            if(row<1 || row>this->row){
                //! 5 represents illegal row and column values of matrices
                return 5;
            }
            int start=IA[row];
            int end=IA[row+1];
            int j;
            int count=0;
            for(j=start;j<end;j++){
                *column[count]=JA[j];
                *values[count]=val[j];
                count++;
            }

            return 0;
        }
        //! get a column's values from the matrix
        FaspErrorCode GetColumnValues(int column,int **row,double **values){
            if(column<1 || column>this->col){
                //! 5 represents illegal row and column values of matrices
                return 5;
            }
            int r,start,end;
            int j;
            int count=0;
            for(r=0;r<this->row;r++){
                start=IA[r];
                end=IA[r+1];
                for(j=start;j<end;j++){
                    if(column==JA[j]){
                        *row[count]=r;
                        *values[count]=val[j];
                    }
                }
            }
            
            return 0;
        }
        //! y = this->Matrix * x
        FaspErrorCode MultVec(Vector x,Vector *y){
            int row;
            x.GetRow(&row);
            if(this->col!=row){
                //! 7 is mismatch between vector dimension and 
                //! matrix dimension
                return 7;
            }
            y->SetSameValue(row,0.0);

            double tmp,value;
            int start,end;
            int j,k;
            for(j=0;j<this->row;j++){
                tmp=0.0;
                start=IA[j];
                end=IA[j+1];
                for(k=start;k<end;k++){
                    x.GetValue(JA[k],&value);
                    tmp+=val[k]*value;
                }
                y->SetValue(j,tmp);
            }

            return 0;
        }
        //! this->Matrix = a * this->Matrix
        FaspErrorCode Multa(double a){
            int j;
            for(j=0;j<nnz;j++){
                val[j]=val[j]*a;
            }

            return 0;
        }
        //! vector_z = vector_y + this->Matrix * vector_x
        FaspErrorCode VecAddMatMultVec(Vector x,Vector y,Vector *z){
            int xrow,yrow,zrow;
            Vector tmp;

            tmp.SetSameValue(row,0.0);

            x.GetRow(&xrow);
            y.GetRow(&yrow);
            z->GetRow(&zrow);

            if(col!=xrow || row!=yrow){
                //! 9 is the mismatchs of vector dimension and matrix dimension
                return 9; 
            }

            this->MultVec(x,&tmp);
            z->VecAddVec(x,y);

            return 0;
        }

        //! this->Matrix = Preconditioner * this->Matrix
        FaspErrorCode PreMult(Matrix P);
        //! this->Matrix = this->Matrix * Preconditioner
        FaspErrorCode MultPre(Matrix P);

        //! NormInfinity(this->Matrix)
        FaspErrorCode NormInifinity(double *norminfinity){
            int start,end;
            double sum=0.0;
            int j=0,k;

            start=IA[j];
            end=IA[j+1];
            for(j=start;j<end;j++){
                sum+=fabs(val[j]);
            }
            *norminfinity=sum;
            for(j=1;j<row;j++){
                start=IA[j];
                end=IA[j+1];
                sum=0.0;
                for(k=start;k<end;k++){
                    sum+=fabs(val[j]);
                }
                if(*norminfinity<sum){
                    *norminfinity=sum;
                }
            }

            return 0;
        }
        //! NormF(this->matrix)
        FaspErrorCode NormF(double *normF){
            int j;
            *normF=0.0;
            for(j=0;j<nnz;j++){
                *normF=val[j]*val[j];
            }
            *normF=sqrt(*normF);

            return 0;
        }
};
        