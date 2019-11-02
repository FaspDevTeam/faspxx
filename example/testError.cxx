
#include <iostream>
#include "ErrorLog.hxx"

using namespace std;

void test(int* tmp=NULL)
{
    if (tmp == NULL)
    {
        FASPXX_ASSERT(false, "This is an assert test!");
    }
    if (tmp == NULL)
    {
        FASPXX_WARNING("Something might be wrong!");
    }
    if (tmp == NULL)
    {
        FASPXX_ABORT("This is an abort test!");
    }
}

int main(int argc, char* argv[])
{
    test();
    return 0;
}