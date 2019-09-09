/**
 *  Vector class
 */

#include<math.h>
#include<stddef.h>
//------------------------------------------------------------------------------
/* FaspErrorCode is the type of error */
typedef int FaspErrorCode;

class Vector{
    private:
        //! number of rows
        int row;
        //! actual vector entries
        double *val;
    public:
        //! constructed function
        Vector(){
            row=1;
            val=NULL;
        }
        //! assign the same value to class object
        FaspErrorCode SetSameValue(int row,double a){
            int j;
            this->row=row;
            val=new double[this->row];

            for(j=0;j<this->row;++j){
                val[j]=a;
            }

            return 0;
        }
        //! assign values to class object
        FaspErrorCode SetValues(int row,double *value){
            this->row=row;
            this->val=value;
            return 0;
        }
        //! get the row number from class object
        FaspErrorCode GetRow(int *row){
            *row=this->row;
            return 0;
        }
        //! get value from class object
        FaspErrorCode GetValue(int index,double *value){
            if(index>=row){
                //! 1 is an array crossover error
                return 1;
            }
            *value=val[index];
            return 0;
        }
        //! get all the values from class object
        FaspErrorCode GetValues(double **array){
            *array=val;
            return 0;            
        }
        //! this->val = a * this->val
        FaspErrorCode Scale(double a){
            int j;
            for(j=0;j<row;j++){
                val[j]=a*val[j];
            }

            return 0;
        }
        //! value = this->val^{'} * b
        FaspErrorCode Dot(Vector b,double *value){
            if(b.row!=this->row){
                // 2 is the array length mismatch error
                return 2;
            }
            
            double dot=0;
            int j;
            for(j=0;j<this->row;++j){
                dot+=val[j]*b.val[j];
            }

            *value=dot;

            return 0;
        }
        //! this->val = vector + this->val
        FaspErrorCode Add(Vector b){
            if(b.row!=this->row){
                // 2 is the array length mismatch error
                return 2;
            }

            int j;
            for(j=0;j<this->row;++j){
                val[j]=val[j]+b.val[j];
            }

            return 0;
        }
        //! this->val = a * vector + this->val
        FaspErrorCode AddAmultVec(double a,Vector b){
            if(row!=b.row){
                //! 2 is the array length mismatch error
                return 2;
            }

            int j;
            for(j=0;j<row;j++){
                val[j]=val[j]+a*b.val[j];
            }

            return 0;
        }
        //! this->val = vector + a * this->val
        FaspErrorCode AmultAddVec(double a,Vector b){
            if(row!=b.row){
                //! 2 is the array length mismatch error
                return 2;
            }

            int j;
            for(j=0;j<row;j++){
                val[j]=a*val[j]+b.val[j];
            }

            return 0;
        }
        //! this->val = abs(this->val)
        FaspErrorCode Abs(){
            int j;
            for(j=0;j<row;j++){
                this->val[j]=fabs(this->val[j]);
            }
            return 0;
        }
        //! this->val[j]=this->val[j]*vector[j]
        FaspErrorCode PointWiseMult(Vector b){
            if(row!=b.row){
                // 2 is the array length mismatch error
                return 2;
            }
            int j;
            for(j=0;j<row;j++){
                this->val[j]=this->val[j]*b.val[j];
            }

            return 0;
        }
        //! max(this->val)
        FaspErrorCode Max(double *max){
            int j;
            *max=val[0];
            for(j=1;j<row;j++){
                if(*max<val[j]){
                    *max=val[j];
                }
            }

            return 0;
        }
        //! min(this->val)
        FaspErrorCode Min(double *min){
            int j;
            *min=val[0];
            for(j=1;j<row;j++){
                if(*min>val[j]){
                    *min=val[j];
                }
            }

            return 0;
        }
        //! Sum(this->val)
        FaspErrorCode Sum(double *sum){
            int j;
            *sum=0.0;
            for(j=0;j<row;j++){
                *sum+=val[j];
            }

            return 0;
        }
        //! Reciprocial(this->val)
        FaspErrorCode Reciprocial(){
            int j;
            for(j=0;j<row;j++){
                //! judge whether val[j] == 0
                if(fabs(val[j])<1.0E-44){
                    // 3 is the error divided by 0
                    return 3;
                }else{
                    val[j]=1.0/val[j];
                }
            }
        }
        //! Shift(this->val)
        FaspErrorCode Shift(double a){
            int j;
            for(j=0;j<row;j++){
                val[j]+=a;
            }

            return 0;
        }
        //! Norm1(this->val)
        FaspErrorCode Norm1(double *norm1){
            int j;
            *norm1=0.0;
            for(j=0;j<row;j++){
                *norm1+=fabs(this->val[j]);
            }

            return 0;
        }
        //! Norm2(this->val)
        FaspErrorCode Norm2(double *norm2){
            int j;
            double tmp;
            *norm2=0.0;
            for(j=0;j<row;j++){
                tmp=this->val[j];
                *norm2+=tmp*tmp;
            }
            *norm2=sqrt(*norm2);

            return 0;
        }
        //! NormInfinity(this->val)
        FaspErrorCode NormInfinity(double *norminfinity){
            int j;
            *norminfinity=0.0;
            for(j=0;j<row;j++){
                if(fabs(val[j])>*norminfinity){
                    *norminfinity=fabs(val[j]);
                }
            }

            return 0;
        }
        //! NormP(this->val)
        FaspErrorCode NormP(double *norm,double p){
            ////? need to judge that p < 0 ?
            int j;
            *norm=0.0;
            for(j=0;j<row;j++){
                *norm+=pow(fabs(val[j]),p);
            }
            *norm=pow(*norm,1/p);

            return 0;
        }
}; 
