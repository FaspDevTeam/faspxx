set(UNIT_TESTS_SRCS
    src/test_ErrorLog.cxx
    src/test_Jacobi.cxx
    src/test_MAT.cxx
    src/test_Param.cxx
    src/test_VEC.cxx
    )

# All serial unit tests are build into a single executable 'unit_tests'
add_executable(unit_tests unit_tests.cxx ${UNIT_TESTS_SRCS})
target_link_libraries(unit_tests faspxx)

# Copy data to the build directory
add_custom_command(TARGET unit_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data_for_test data_for_test
        COMMENT "Copying the unit tests data directory ...")

# Create a test called "unit_tests' that runs the 'unit_tests' executable.
# The unit tests can be built and run seperately from the rest of the tests:
#    make unit_tests
#    ctest -R unit_tests [-V]
add_test(NAME unit_tests COMMAND unit_tests)
