#include<iostream>
#include<cstdlib>
#include<time.h>

int main()
{
    int j;
    srand((unsigned)time(NULL));
    double *p;
    p=new double(10);

    for(j=0;j<10;j++){
        p[j]=rand()/(double)RAND_MAX;
    }

    for(j=0;j<10;j++){
        std::cout<<p[j]<<std::endl;
    }

    delete[] p;
    return 0;
}