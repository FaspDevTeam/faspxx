# CMake file for GoogleTest dependent from FASP++

if ("${utest}" STREQUAL "ON")

    # Find GTest from the default location
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Copy test files
    file(COPY "./data_for_test" DESTINATION ".")

    # Build unit tests
    add_executable(testAll unitGTestAll.cxx unitErrorLog.cxx)
    target_link_libraries(testAll "${GTEST_LIBRARIES}" pthread faspxx)

else("${utest}" STREQUAL "OFF")

    message("-- GoogleTest is disabled")

endif()