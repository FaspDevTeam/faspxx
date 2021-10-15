/*! \file    UnitTests.cxx
 *  \brief   Main function for unit tests
 *  \author  Ronghong Fan
 *  \date    Oct/10/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#define CATCH_CONFIG_RUNNER

#include "catch.hxx"

int main(int argc, char* argv[])
{
    // There must be exactly one instance for session
    Catch::Session session;

    // For floating point comparisons, print
    //   -- 8  digits for single precision values
    //   -- 16 digits for double precision values
    Catch::StringMaker<float>::precision  = 8;
    Catch::StringMaker<double>::precision = 16;

    // Apply provided command line arguments
    int r = session.applyCommandLine(argc, argv);
    if (r != 0) return r;

    // Run unit tests
    int result = session.run();
    return result;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Ronghong Fan        Oct/10/2021      Create file                          */
/*----------------------------------------------------------------------------*/