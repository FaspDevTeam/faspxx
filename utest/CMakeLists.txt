#----------------- GoogleTest Dependent from FASP++ ---------------
if ("${utest}" STREQUAL "ON")
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    file(COPY "./data_for_test" DESTINATION ".") # 复制data目录到build/data目录

    add_executable(testAll unitGTestAll.cxx)
    target_link_libraries(testAll "${GTEST_LIBRARIES}" pthread faspxx)

else("${utest}" STREQUAL "OFF")
    message("No GoogleTest")
endif()