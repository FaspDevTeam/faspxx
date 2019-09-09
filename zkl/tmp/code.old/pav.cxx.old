/**
 * seek for the fastest operations among pointer, array and vector
 */
#include<iostream>
#include<vector>
#include<cstdlib>
#include<time.h>
#include<sys/time.h>

const int M16=16000000;
const int M32=32000000;

double get_wall_time(){  
    struct timeval time ;  
    if(gettimeofday(&time,NULL)){  
        return 0;  
    }  
    return (double)time.tv_sec+(double)time.tv_usec*0.000001;  
}

int main()
{
    double start,end;
    double sum;
    double add;
    int j;

    srand((unsigned)time(NULL));

    /*------------------------------------------------------------*/
    std::cout<<"16M array"<<std::endl;

    start=get_wall_time();
    double array16_1[M16],array16_2[M16];
    end=get_wall_time();
    std::cout<<"array memory allocated time : "<<end-start<<std::endl;

    start=get_wall_time();
    for(j=0;j<M16;j++){
        array16_1[j]=rand()/(double)RAND_MAX;
        array16_2[j]=rand()/(double)RAND_MAX;
    }
    end=get_wall_time();
    std::cout<<"array assignment's time : "<<end-start<<std::endl;

    //array add array
    start=get_wall_time();
    for(j=0;j<M16;j++){
        add=array16_1[j]+array16_2[j];
    }
    end=get_wall_time();
    std::cout<<"array add array time : "<<end-start<<std::endl;

    //array dot array
    start=get_wall_time();
    for(j=0;j<M16;j++){
        sum+=array16_1[j]*array16_2[j];
    }
    end=get_wall_time();
    std::cout<<"array dot array time : "<<end-start<<std::endl;
    sum=0.0;

    /*------------------------------------------------------------*/
    std::cout<<"32M array"<<std::endl;

    start=get_wall_time();
    double array32_1[M32],array32_2[M32];
    end=get_wall_time();
    std::cout<<"array memory allocated time : "<<end-start<<std::endl;

    start=get_wall_time();
    for(j=0;j<M32;j++){
        array32_1[j]=rand()/(double)RAND_MAX;
        array32_2[j]=rand()/(double)RAND_MAX;
    }
    end=get_wall_time();
    std::cout<<"array assignment's time : "<<end-start<<std::endl;

    //array add array
    start=get_wall_time();
    for(j=0;j<M32;j++){
        add=array32_1[j]+array32_2[j];
    }
    end=get_wall_time();
    std::cout<<"array add array time : "<<end-start<<std::endl;

    //array dot array
    start=get_wall_time();
    for(j=0;j<M32;j++){
        sum+=array32_1[j]*array32_2[j];
    }
    end=get_wall_time();
    std::cout<<"array dot array time : "<<end-start<<std::endl;
    sum=0.0;

    /*------------------------------------------------------------*/
    std::cout<<"16M pointer"<<std::endl;
    start=get_wall_time();
    double *p16_1,*p16_2;
    p16_1=new double[M16];
    p16_2=new double[M16];
    end=get_wall_time();
    std::cout<<"create pointers time : "<<end-start<<std::endl;

    start=get_wall_time();
    for(j=0;j<M16;j++){
        p16_1[j]=rand()/(double)RAND_MAX;
        p16_2[j]=rand()/(double)RAND_MAX;
    }
    end=get_wall_time();
    std::cout<<"pointers assignment's time : "<<end-start<<std::endl;

    // pointer add pointer
    start=get_wall_time();
    for(j=0;j<M16;j++){
        add=p16_1[j]+p16_2[j];
    }
    end=get_wall_time();
    std::cout<<"pointer add pointer time : "<<end-start<<std::endl;

    //pointer dot pointer
    start=get_wall_time();
    for(j=0;j<M16;j++){
        sum+=p16_1[j]*p16_2[j];
    }
    end=get_wall_time();
    std::cout<<"pointer dot pointer time : "<<end-start<<std::endl;
    sum=0.0;

    delete[] p16_1;
    delete[] p16_2;

    /*------------------------------------------------------------*/
    std::cout<<"32M pointer"<<std::endl;
    start=get_wall_time();
    double *p32_1,*p32_2;
    p32_1=new double[M32];
    p32_2=new double[M32];
    end=get_wall_time();
    std::cout<<"create pointers time : "<<end-start<<std::endl;

    start=get_wall_time();
    for(j=0;j<M32;j++){
        p32_1[j]=rand()/(double)RAND_MAX;
        p32_2[j]=rand()/(double)RAND_MAX;
    }
    end=get_wall_time();
    std::cout<<"pointers assignment's time : "<<end-start<<std::endl;

    // pointer add pointer
    start=get_wall_time();
    for(j=0;j<M32;j++){
        add=p32_1[j]+p32_2[j];
    }
    end=get_wall_time();
    std::cout<<"pointer add pointer time : "<<end-start<<std::endl;

    //pointer dot pointer
    start=get_wall_time();
    for(j=0;j<M32;j++){
        sum+=p32_1[j]*p32_2[j];
    }
    end=get_wall_time();
    std::cout<<"pointer dot pointer time : "<<end-start<<std::endl;
    sum=0.0;

    delete[] p32_1;
    delete[] p32_2;
    /*------------------------------------------------------------*/
    std::cout<<"16M vector"<<std::endl;
    start=get_wall_time();
    std::vector<double> vec16_1,vec16_2;
    end=get_wall_time();
    std::cout<<"create vectors time : "<<end-start<<std::endl;

    start=get_wall_time();
    for(j=0;j<M16;j++){
        vec16_1.push_back(rand()/(double)RAND_MAX);
        vec16_2.push_back(rand()/(double)RAND_MAX);
    }
    end=get_wall_time();
    std::cout<<"vector assignment's time : "<<end-start<<std::endl;

    // vector add vector
    start=get_wall_time();
    for(j=0;j<M16;j++){
        add=*(vec16_1.begin()+j)+*(vec16_2.begin()+j);
    }
    end=get_wall_time();
    std::cout<<"vector add vector time : "<<end-start<<std::endl;

    //vector dot vector
    start=get_wall_time();
    for(j=0;j<M16;j++){
        sum+=(*(vec16_1.begin()+j))*(*(vec16_2.begin()+j));
    }
    end=get_wall_time();
    std::cout<<"vector dot vector time : "<<end-start<<std::endl;
    sum=0.0;

    /*------------------------------------------------------------*/
    std::cout<<"32M vector"<<std::endl;
    start=get_wall_time();
    std::vector<double> vec32_1,vec32_2;
    end=get_wall_time();
    std::cout<<"create vectors time : "<<end-start<<std::endl;

    start=get_wall_time();
    for(j=0;j<M32;j++){
        vec32_1.push_back(rand()/(double)RAND_MAX);
        vec32_2.push_back(rand()/(double)RAND_MAX);
    }
    end=get_wall_time();
    std::cout<<"vector assignment's time : "<<end-start<<std::endl;

    // vector add vector
    start=get_wall_time();
    for(j=0;j<M32;j++){
        add=*(vec32_1.begin()+j)+*(vec32_2.begin()+j);
    }
    end=get_wall_time();
    std::cout<<"vector add vector time : "<<end-start<<std::endl;

    //vector dot vector
    start=get_wall_time();
    for(j=0;j<M32;j++){
        sum+=*(vec32_1.begin()+j) * *(vec32_2.begin()+j);
    }
    end=get_wall_time();
    std::cout<<"vector dot vector time : "<<end-start<<std::endl;
    sum=0.0;

    return 0;
}