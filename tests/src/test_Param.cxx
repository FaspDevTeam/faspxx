#include <string>

#include "Param.hxx"

#include "../catch.hxx"


TEST_CASE("Param")
{
    std::cout << "TEST Param" << std::endl;

    // parameters in user program
    bool        bool_param   = false;
    INT         int_param    = 11;
    DBL         double_param = 3.14159;
    std::string char_param   = "user params";
    Output      output_lvl   = static_cast<Output>(4);

    // parameters in command line
    const int   _argc        = 11;
    const char* _argv[_argc] = {"program name",
                                "-bool_param",
                                "TruE",
                                "-int_param",
                                "22",
                                "-double_param",
                                "1.41414",
                                "-char_param",
                                "commandline_parameters",
                                "-faspxx_opts",
                                "./data_for_test/single_sol.param"};

    // parameters in file
    std::string params_file  = "./data_for_test/multiple_sol.param";
    bool        view_param   = false; // from file
    INT         level_param  = 0;     // from file
    DBL         resrel_param = 0.0;   // from file
    std::string vec_param    = "";    // from file

    REQUIRE(bool_param == false);
    REQUIRE(int_param == 11);
    REQUIRE(double_param == 3.14159);
    REQUIRE(char_param == "user params");
    REQUIRE(output_lvl == 4);
    REQUIRE(params_file == "./data_for_test/multiple_sol.param");
    REQUIRE(view_param == false);
    REQUIRE(level_param == 0);
    REQUIRE(resrel_param == 0.0);
    REQUIRE(vec_param == "");

    Parameters params(_argc, _argv);
    params.AddParam("-bool_param", "bool param help", &bool_param);
    params.AddParam("-int_param", "integer param help", &int_param);
    params.AddParam("-double_param", "double param help", &double_param);
    params.AddParam("-char_param", "char* param help", &char_param);
    params.AddParam("-faspxx_opts", "additional parameter file", &params_file, 2);

    params.AddParam("-view", "view from file", &view_param);
    params.AddParam("-level", "level from file", &level_param);
    params.AddParam("-resrel", "resrel from file", &resrel_param);
    params.AddParam("-vec", "vec from file", &vec_param);
    params.AddParam("-out_put", "output param help", &output_lvl);
    params.Parse();

    REQUIRE(bool_param == true);                     // modified by command line
    REQUIRE(int_param == 22);                        // modified by command line
    REQUIRE(double_param == 1.41414);                // modified by command line
    REQUIRE(char_param == "commandline_parameters"); // modified by command line
    REQUIRE(params_file ==
              "./data_for_test/single_sol.param"); // modified by command line

    REQUIRE(view_param == true);                  // modified from file
    REQUIRE(level_param == 4);                    // modified from file
    REQUIRE(std::abs(resrel_param - 1.234e-6) < 1E-14);     // modified from file
    REQUIRE(vec_param == "../data/ffffffffffff"); // modified from file
    REQUIRE(output_lvl == 6);                     // modified from file
}


