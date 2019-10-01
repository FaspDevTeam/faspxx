/**
 * G/P Left-Looking Algorithm
 */

#include "MAT.hxx"
#include "RetCode.hxx"
#include <iostream>

using namespace std;
void func(){
    try{
        double *a=new double[10000000000000000];
        throw(FaspBadAlloc(__FILE__,__FUNCTION__,__LINE__));
    }catch(std::bad_alloc &ex){
        cout<<"Hello world!\n";
    }
    cout<<"Hello! \n";
}

int main()
{
    cout<<"begin!\n";
    try{
        func();
    }catch(FaspBadAlloc &ex){
        ex.LogExcep();
    }
    cout<<"end!\n";
    return 0;
}