set(UNIT_TESTS_SRCS
    src/UnitTestsErrorLog.cxx
    src/UnitTestsJacobi.cxx
    src/UnitTestsMAT.cxx
    src/UnitTestsParam.cxx
    src/UnitTestsParVEC.cxx
    src/UnitTestsVEC.cxx
    )

# All serial unit tests are build into a single executable 'UnitTests'
add_executable(UnitTests UnitTests.cxx ${UNIT_TESTS_SRCS})
target_link_libraries(UnitTests faspxx)

# Copy data to the build directory
add_custom_command(TARGET UnitTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data data
        COMMENT "Copying the unit tests data directory ...")

# Create a test called "UnitTests' that runs the 'UnitTests' executable.
# The unit tests can be built and run seperately from the rest of the tests:
#    make UnitTests
#    ctest -R UnitTests [-V]
add_test(NAME UnitTests COMMAND UnitTests)
