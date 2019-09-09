/**
 * a test file
 */

#include <iostream>
#include <vector>

using namespace std;

int main(){
    vector<double> v1(10);
    for(int j=0;j<10;j++){
        v1[j]=(j+1)/10.0;
    }
    for(int j=0;j<10;j++){
        cout<<v1[j]<<"  ";
    }
    cout<<endl;
    vector<double> v2(v1.begin()+6,v1.end());
    for(int j=0;j<4;j++){
        cout<<v2[j]<<"  ";
    }
    cout<<endl;
    return 0;
}