/**
 * a test file
 */

#include "VEC.old.hxx"
#include <iostream>
#include "Timing.hxx"

using namespace std;

int main()
{
    const int len=100000000;
    GetWallTime timer;
    GetCycleNum cycle;

    DBL *p1=new DBL[len];
    DBL *p2=new DBL[len];

    VEC v1(len),v2(len);

    srand(1);
    for(INT j=0;j<len;j++)
        v1[j]=rand()/(double)RAND_MAX;

    srand(2);
    for(INT j=0;j<len;j++)
        v2[j]=rand()/(double)RAND_MAX;

    VEC tmp1,tmp2;

    DBL cpucycle;
    DBL dot;

    tmp1=v1;
    tmp2=v2;

    timer.start();
    cycle.start();
    tmp1.GetArray(len,p1);
    cpucycle=cycle.stop();
    cout<<"GetArray time 1 : "<<timer.stop()<<endl;
    cout<<"GetArray cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.GetArray2(len,p1);
    cpucycle=cycle.stop();
    cout<<"GetArray time 2 : "<<timer.stop()<<endl;
    cout<<"GetArray cycle 2 "<<cpucycle<<endl;

    delete[] p1;
    delete[] p2;

    timer.start();
    cycle.start();
    tmp1.Scale(2.87);
    cpucycle=cycle.stop();
    cout<<"Scale time 1 : "<<timer.stop()<<endl;
    cout<<"Scale cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Scale2(2.87);
    cpucycle=cycle.stop();
    cout<<"Scale time 2 : "<<timer.stop()<<endl;
    cout<<"Scale cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.PointwiseMult(v2);
    cpucycle=cycle.stop();
    cout<<"PointwiseMult time 1 : "<<timer.stop()<<endl;
    cout<<"PointwiseMult cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.PointwiseMult2(v2);
    cpucycle=cycle.stop();
    cout<<"PointwiseMult time 2 : "<<timer.stop()<<endl;
    cout<<"PointwiseMult cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Reciprocal();
    cpucycle=cycle.stop();
    cout<<"Reciprocal time 1 : "<<timer.stop()<<endl;
    cout<<"Reciprocal cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Reciprocal2();
    cpucycle=cycle.stop();
    cout<<"Reciprocal time 2 : "<<timer.stop()<<endl;
    cout<<"Reciprocal cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.PointwiseDivide(v2);
    cpucycle=cycle.stop();
    cout<<"PointwiseDivide time 1 : "<<timer.stop()<<endl;
    cout<<"PointwiseDivide cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.PointwiseDivide2(v2);
    cpucycle=cycle.stop();
    cout<<"PointwiseDivide2 time 2 : "<<timer.stop()<<endl;
    cout<<"PointwiseDivide2 cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Shift(3.14);
    cpucycle=cycle.stop();
    cout<<"Shift time 1 : "<<timer.stop()<<endl;
    cout<<"Shift cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Shift2(3.14);
    cpucycle=cycle.stop();
    cout<<"Shift time 2 : "<<timer.stop()<<endl;
    cout<<"Shift cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Abs();
    cpucycle=cycle.stop();
    cout<<"Abs time 1 : "<<timer.stop()<<endl;
    cout<<"Abs cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Abs2();
    cpucycle=cycle.stop();
    cout<<"Abs time 2 : "<<timer.stop()<<endl;
    cout<<"Abs cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Add(3.14,2.87,v2);
    cpucycle=cycle.stop();
    cout<<"Add3 time 1 : "<<timer.stop()<<endl;
    cout<<"Add3 cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Add2(3.14,2.87,v2);
    cpucycle=cycle.stop();
    cout<<"Add3 time 2 : "<<timer.stop()<<endl;
    cout<<"Add3 cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Add(3.14,v1,2.87,v2);
    cpucycle=cycle.stop();
    cout<<"Add4 time 1 : "<<timer.stop()<<endl;
    cout<<"Add4 cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Add2(3.14,v1,2.87,v2);
    cpucycle=cycle.stop();
    cout<<"Add4 time 2 : "<<timer.stop()<<endl;
    cout<<"Add4 cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Max();
    cpucycle=cycle.stop();
    cout<<"Max time 1 : "<<timer.stop()<<endl;
    cout<<"Max cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Max2();
    cpucycle=cycle.stop();
    cout<<"Max time 2 : "<<timer.stop()<<endl;
    cout<<"Max cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Min();
    cpucycle=cycle.stop();
    cout<<"Min time 1 : "<<timer.stop()<<endl;
    cout<<"Min cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Min2();
    cpucycle=cycle.stop();
    cout<<"Min time 2 : "<<timer.stop()<<endl;
    cout<<"Min cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.Norm2();
    cpucycle=cycle.stop();
    cout<<"Norm2 time 1 : "<<timer.stop()<<endl;
    cout<<"Norm2 cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.Norm22();
    cpucycle=cycle.stop();
    cout<<"Norm time 2 : "<<timer.stop()<<endl;
    cout<<"Norm cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp1.NormInf();
    cpucycle=cycle.stop();
    cout<<"NormInf time 1 : "<<timer.stop()<<endl;
    cout<<"NormInf cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    tmp2.NormInf2();
    cpucycle=cycle.stop();
    cout<<"NormInf time 2 : "<<timer.stop()<<endl;
    cout<<"NormInf cycle 2 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    dot=tmp1.Dot(v2);
    cpucycle=cycle.stop();
    cout<<"Dot time 1 : "<<timer.stop()<<endl;
    cout<<"Dot cycle 1 "<<cpucycle<<endl;

    timer.start();
    cycle.start();
    dot=tmp2.Dot(v2);
    cpucycle=cycle.stop();
    cout<<"Dot time 2 : "<<timer.stop()<<endl;
    cout<<"Dot cycle 2 "<<cpucycle<<endl;

    return 0;
}