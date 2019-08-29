/**
 * seek for the fastest operations among pointer, array and vector
 */
#include<iostream>
#include<vector>
#include<cstdlib>
#include<time.h>
#include<sys/time.h>
#include<cstring>

const int M16=16000000;
const int M32=32000000;
const int M64=64000000;

double get_wall_time(){  
    struct timeval time ;  
    if(gettimeofday(&time,NULL)){  
        return 0;  
    }  
    return (double)time.tv_sec+(double)time.tv_usec*0.000001;  
}

int main()
{
    double start1,end1,start2,end2,start3,end3;
    double sum;
    double add;
    int j;

    srand((unsigned)time(NULL));
    double test16_1[M16],test16_2[M16];
    double test32_1[M32],test32_2[M32];
    double test64_1[M64],test64_2[M64];

    for(j=0;j<M16;j++){
        test16_1[j]=rand()/(double)RAND_MAX;
        test16_2[j]=rand()/(double)RAND_MAX;
    }
    for(j=0;j<M32;j++){
        test32_1[j]=rand()/(double)RAND_MAX;
        test32_2[j]=rand()/(double)RAND_MAX;
    }
    for(j=0;j<M64;j++){
        test64_1[j]=rand()/(double)RAND_MAX;
        test64_2[j]=rand()/(double)RAND_MAX;
    }

    /*------------------------------------------------------------*/
    std::cout<<"memory allocation time : "<<std::endl;

    start1=get_wall_time();
    double array16_1[M16],array16_2[M16];
    end1=get_wall_time();
    start2=get_wall_time();
    double *p16_1,*p16_2;
    p16_1=new double[M16];
    p16_2=new double[M16];
    end2=get_wall_time();
    start3=get_wall_time();
    std::vector<double> vec16_1(M16),vec16_2(M16);
    end3=get_wall_time();
    
    std::cout<<"M16 array : "<<end1-start1<<std::endl;
    std::cout<<"M16 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M16 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    double array32_1[M32],array32_2[M32];
    end1=get_wall_time();
    start2=get_wall_time();
    double *p32_1,*p32_2;
    p32_1=new double[M32];
    p32_2=new double[M32];
    end2=get_wall_time();
    start3=get_wall_time();
    std::vector<double> vec32_1(M32),vec32_2(M32);
    end3=get_wall_time();

    std::cout<<"M32 array : "<<end1-start1<<std::endl;
    std::cout<<"M32 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M32 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    double array64_1[M64],array64_2[M64];
    end1=get_wall_time();
    start2=get_wall_time();
    double *p64_1,*p64_2;
    p64_1=new double[M64];
    p64_2=new double[M64];
    end2=get_wall_time();
    start3=get_wall_time();
    std::vector<double> vec64_1(M64),vec64_2(M64);
    end3=get_wall_time();

    std::cout<<"M64 array : "<<end1-start1<<std::endl;
    std::cout<<"M64 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M64 vector : "<<end3-start3<<std::endl;

    /*------------------------------------------------------------*/ 
    std::cout<<"assignment's time : "<<std::endl;
  
    start1=get_wall_time();
    memcpy(array16_1,test16_1,sizeof(test16_1));
    memcpy(array16_2,test16_2,sizeof(test16_2));
    end1=get_wall_time();
    start2=get_wall_time();
    memcpy(p16_1,test16_1,sizeof(test16_1));
    memcpy(p16_2,test16_2,sizeof(test16_2));
    end2=get_wall_time();
    start3=get_wall_time();
    vec16_1.assign(test16_1,test16_1+M16);
    vec16_2.assign(test16_2,test16_2+M16);
    end3=get_wall_time();

    std::cout<<"M16 array : "<<end1-start1<<std::endl;
    std::cout<<"M16 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M16 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    memcpy(array32_1,test32_1,sizeof(test32_1));
    memcpy(array32_2,test32_2,sizeof(test32_2));
    end1=get_wall_time();
    start2=get_wall_time();
    memcpy(p32_1,test32_1,sizeof(test32_1));
    memcpy(p32_2,test32_2,sizeof(test32_2));
    end2=get_wall_time();
    start3=get_wall_time();
    vec32_1.assign(test32_1,test32_1+M32);
    vec32_2.assign(test32_2,test32_2+M32);
    end3=get_wall_time();

    std::cout<<"M32 array : "<<end1-start1<<std::endl;
    std::cout<<"M32 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M32 vector : "<<end3-start3<<std::endl;
    
    start1=get_wall_time();
    memcpy(array64_1,test64_1,sizeof(test64_1));
    memcpy(array64_2,test64_2,sizeof(test64_2));
    end1=get_wall_time();
    start2=get_wall_time();
    memcpy(p64_1,test64_1,sizeof(test64_1));
    memcpy(p64_2,test64_2,sizeof(test64_2));
    end2=get_wall_time();
    start3=get_wall_time();
    vec64_1.assign(test64_1,test64_1+M64);
    vec64_2.assign(test64_2,test64_2+M64);
    end3=get_wall_time();

    std::cout<<"M64 array : "<<end1-start1<<std::endl;
    std::cout<<"M64 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M64 vector : "<<end3-start3<<std::endl;
    /*------------------------------------------------------------*/ 
    std::cout<<"add time : "<<std::endl;

    start1=get_wall_time();
    for(j=0;j<M16;j++){
        add=array16_1[j]+array16_2[j];
    }
    end1=get_wall_time();
    start2=get_wall_time();
    for(j=0;j<M16;j++){
        add=p16_1[j]+p16_2[j];
    }
    end2=get_wall_time();
    start3=get_wall_time();
    for(j=0;j<M16;j++){
        add=vec16_1[j]+vec16_2[j];
    }
    end3=get_wall_time();

    std::cout<<"M16 array : "<<end1-start1<<std::endl;
    std::cout<<"M16 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M16 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    for(j=0;j<M32;j++){
        add=array32_1[j]+array32_2[j];
    }
    end1=get_wall_time();
    start2=get_wall_time();
    for(j=0;j<M32;j++){
        add=p32_1[j]+p32_2[j];
    }
    end2=get_wall_time();
    start3=get_wall_time();
    for(j=0;j<M32;j++){
        add=vec32_1[j]+vec32_2[j];
    }
    end3=get_wall_time();

    std::cout<<"M32 array : "<<end1-start1<<std::endl;
    std::cout<<"M32 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M32 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    for(j=0;j<M64;j++){
        add=array64_1[j]+array64_2[j];
    }
    end1=get_wall_time();
    start2=get_wall_time();
    for(j=0;j<M64;j++){
        add=p64_1[j]+p64_2[j];
    }
    end2=get_wall_time();
    start3=get_wall_time();
    for(j=0;j<M64;j++){
        add=vec64_1[j]+vec64_2[j];
    }
    end3=get_wall_time();

    std::cout<<"M64 array : "<<end1-start1<<std::endl;
    std::cout<<"M64 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M64 vector : "<<end3-start3<<std::endl;
    /*------------------------------------------------------------*/ 
    std::cout<<"dot time : "<<std::endl;

    start1=get_wall_time();
    for(j=0;j<M16;j++){
        sum+=array16_1[j]*array16_2[j];
    }
    end1=get_wall_time();
    sum=0.0;
    start2=get_wall_time();
    for(j=0;j<M16;j++){
        sum+=p16_1[j]*p16_2[j];
    }
    end2=get_wall_time();
    sum=0.0;
    start3=get_wall_time();
    for(j=0;j<M16;j++){
        sum+=vec16_1[j]*vec16_2[j];
    }
    end3=get_wall_time();
    sum=0.0;

    std::cout<<"M16 array : "<<end1-start1<<std::endl;
    std::cout<<"M16 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M16 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    for(j=0;j<M32;j++){
        sum+=array32_1[j]*array32_2[j];
    }
    end1=get_wall_time();
    sum=0.0;
    start2=get_wall_time();
    for(j=0;j<M32;j++){
        sum+=p32_1[j]*p32_2[j];
    }
    end2=get_wall_time();
    sum=0.0;
    start3=get_wall_time();
    for(j=0;j<M32;j++){
        sum+=vec32_1[j]*vec32_2[j];
    }
    end3=get_wall_time();
    sum=0.0;

    std::cout<<"M32 array : "<<end1-start1<<std::endl;
    std::cout<<"M32 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M32 vector : "<<end3-start3<<std::endl;

    start1=get_wall_time();
    for(j=0;j<M64;j++){
        sum+=array64_1[j]*array64_2[j];
    }
    end1=get_wall_time();
    sum=0.0;
    start2=get_wall_time();
    for(j=0;j<M64;j++){
        sum+=p64_1[j]*p64_2[j];
    }
    end2=get_wall_time();
    sum=0.0;
    start3=get_wall_time();
    for(j=0;j<M64;j++){
        sum+=vec64_1[j]*vec64_2[j];
    }
    end3=get_wall_time();
    sum=0.0;

    std::cout<<"M64 array : "<<end1-start1<<std::endl;
    std::cout<<"M64 pointer : "<<end2-start2<<std::endl;
    std::cout<<"M64 vector : "<<end3-start3<<std::endl;

    delete[] p16_1;
    delete[] p16_2;
    delete[] p32_1;
    delete[] p32_2;
    delete[] p64_1;
    delete[] p64_2;

    return 0;
}
