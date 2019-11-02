/**
 * generate MTX format's data for five points stencil of possion equation
 */

#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>

// left boundary
double left(double x,double y){
    return 0;
}

// right boundary
double right(double x,double y){
    return sin(y);
}

// lower boundary
double lower(double x,double y){
    return 0;
}

// upper boundary
double upper(double x,double y){
    return sin(x);
}

// \Delta u = -f
double f(double x,double y){
    return (x*x+y*y)*sin(x*y);
}

// exact solution
double exact_solu(double x,double y){
    return sin(x*y);
}

/*
 * locate position
 */
int locate(int row,int column,int dimen){
    return (row-1)*(dimen-1)+column-1;
}

void Rhs(int dimen,double **ptr,double h){

    *ptr=new double[(dimen-1)*(dimen-1)];

    for(int j=0;j<(dimen-1)*(dimen-1);j++)
        (*ptr)[j]=0;

    for(int k=2;k<=dimen-2;k++){
        for(int j=2;j<=dimen-2;j++)
            (*ptr)[locate(k,j,dimen)]=h*h*f(k*h,j*h);
    }

    // left boundary
    for(int j=2;j<=dimen-2;j++)
        (*ptr)[locate(1,j,dimen)]=left(0,j*h)+h*h*f(1*h,j*h);

    // right boundary
    for(int j=2;j<=dimen-2;j++)
        (*ptr)[locate(dimen-1,j,dimen)]=right(1.0,j*h)+h*h*f((dimen-1)*h,j*h);

    // lower boundary
    for(int j=2;j<=dimen-2;j++)
        (*ptr)[locate(j,1,dimen)]=lower(j*h,0)+h*h*f(j*h,1*h);

    // upper boundary
    for(int j=2;j<=dimen-2;j++)
        (*ptr)[locate(j,dimen-1,dimen)]=upper(j*h,1.0)+h*h*f(j*h,(dimen-1)*h);

    // left lower corner
    (*ptr)[locate(1,1,dimen)]=left(0,h)+lower(h,0)+h*h*f(1*h,1*h);
    // left upper corner
    (*ptr)[locate(1,dimen-1,dimen)]=left(0,h*(dimen-1))+upper(h,1.0)+h*h*f(1*h,
            (dimen-1)*h);
    // right lower corner
    (*ptr)[locate(dimen-1,1,dimen)]=lower((dimen-1)*h,0)+right(1.0,h)+h*h*f(
            (dimen-1)*h,1*h);
    // right upper corner
    (*ptr)[locate(dimen-1,dimen-1,dimen)]=upper((dimen-1)*h,1.0)
            +right(1.0,(dimen-1)*h)+h*h*f((dimen-1)*h,(dimen-1)*h);

}