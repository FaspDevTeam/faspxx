
#include "../src/Error.hxx"
#include <iostream>

using namespace std;


void test(int* tmp=NULL)
{
    if (tmp == NULL)
    {
        FASPXX_ASSERT(false, "This is a false test for assert.");
    }
    if (tmp == NULL)
    {
        FASPXX_WARNING("This is a WARNING!");
    }
    if (tmp == NULL)
    {
        FASPXX_ABORT("This is a abort test.");
    }

}



int main(int argc, char* argv[])
{
    test();
    return 0;
}