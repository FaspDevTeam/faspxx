/**
 * a test file about toupper and tolower
 */

#include <iostream>
#include <cstring>

using namespace std;

int main(int argc,char *args[]){
    char *a="aaaa";
    char *B="BBBB";
    char *tmp=new char[strlen(a)];

    cout<<"a : "<<a<<endl;
    cout<<"B : "<<B<<endl;

    //a[0]=(char)(a[0]-'a'+'A');
    //cout<<"a[0] : "<<a[0]<<endl;

    for(int j=0;j<strlen(a);++j)
        tmp[j]=(char)(a[j]-'a'+'A');
    cout<<tmp<<endl;
#if 0
    for(int j=0;j<strlen(B);++j)
        B[j]=(char)(B[j]-'A'+'a');
    cout<<B<<endl;
#endif
    return 0;
}