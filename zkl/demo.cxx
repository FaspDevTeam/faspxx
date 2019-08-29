/**
 * <vector> functions test
 */

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    std::vector<double> v;
    v.reserve(2);
    v[0]=1.2;
    v[1]=2.3;
    for(int j=0;j<2;j++){
        cout<<v[j]<<endl;
    }

    return 0;
}