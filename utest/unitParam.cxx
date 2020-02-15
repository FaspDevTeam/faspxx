/** \file    unitMAT.cxx
 *  \brief   Unit test for MAT class implementations
 *  \author  Ronghong Fan
 *  \date    Oct/24/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include <string>
#include "Param.hxx"

/*---------------------------------*/
/*--     Beginning of TEST       --*/
/*---------------------------------*/

TEST(Parameters_Class, Parameters)
{
    // parameters in user program
    bool bool_param = false;
    int int_param = 11;
    double double_param = 3.14159;
    std::string char_param = "user params";
    Output output_lvl = static_cast<Output>(4);

    // parameters in command line
    const int _argc = 11;
    const char *_argv[_argc] = {"program name", "-bool_param", "TruE", "-int_param",
                                "22", "-double_param", "1.41414", "-char_param",
                                "commandline_parameters", "-faspxx_opts",
                                "./data_for_test/single_sol.param"};

    // parameters in file
    std::string params_file = "./data_for_test/multiple_sol.param";
    bool view_param = false; // from file
    int level_param = 0; // from file
    double resrel_param = 0.0; // from file
    std::string vec_param = ""; // from file

    EXPECT_EQ(bool_param,  false);
    EXPECT_EQ(int_param, 11);
    EXPECT_EQ(double_param, 3.14159);
    EXPECT_EQ(char_param, "user params");
    EXPECT_EQ(output_lvl, 4);
    EXPECT_EQ(params_file, "./data_for_test/multiple_sol.param");
    EXPECT_EQ(view_param, false);
    EXPECT_EQ(level_param, 0);
    EXPECT_EQ(resrel_param, 0.0);
    EXPECT_EQ(vec_param, "");

    Parameters params(_argc, _argv);
    params.AddParam("-bool_param", "bool param help", &bool_param);
    params.AddParam("-int_param", "INT param help", &int_param);
    params.AddParam("-double_param", "DBL param help", &double_param);
    params.AddParam("-char_param", "char* param help", &char_param);
    params.AddParam("-faspxx_opts", "additional parameter file", &params_file, 2);

    params.AddParam("-view", "view from file", &view_param);
    params.AddParam("-level", "level from file", &level_param);
    params.AddParam("-resrel", "resrel from file", &resrel_param);
    params.AddParam("-vec", "vec from file", &vec_param);
    params.AddParam("-out_put", "output param help", &output_lvl);
    params.Parse();

    EXPECT_EQ(bool_param, true); // modified by command line
    EXPECT_EQ(int_param, 22); // modified by command line
    EXPECT_EQ(double_param, 1.41414); // modified by command line
    EXPECT_EQ(char_param, "commandline_parameters"); // modified by command line
    EXPECT_EQ(params_file, "./data_for_test/single_sol.param"); // modified by command line

    EXPECT_EQ(view_param, true); // modified from file
    EXPECT_EQ(level_param, 4); // modified from file
    EXPECT_DOUBLE_EQ(resrel_param, 1.234e-6); // modified from file
    EXPECT_EQ(vec_param, "../data/ffffffffffff"); // modified from file
    EXPECT_EQ(output_lvl, 6); // modified from file
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/