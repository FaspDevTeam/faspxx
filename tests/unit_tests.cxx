#define CATCH_CONFIG_RUNNER
#include "catch.hxx"

int main(int argc, char *argv[])
{
    // There must be exactly one instance.
    Catch::Session session;

    // For floating point comparisons, print 8 digits for single precision
    // values, and 16 digits for double precision values.
    Catch::StringMaker<float>::precision = 8;
    Catch::StringMaker<double>::precision = 16;

    // Apply provided command line arguments.
    int r = session.applyCommandLine(argc, argv);
    if (r != 0)
    {
        return r;
    }

    int result = session.run();

    return result;
}
