#----------------- GoogleTest Dependent from FASP++ ---------------
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(testAll unitGTestAll.cxx)
target_link_libraries(testAll "${GTEST_LIBRARIES}" pthread faspxx)
