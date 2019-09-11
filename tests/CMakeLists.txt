


include_directories (
        "/home/fan/Downloads/googletest/googletest/include"
)

link_directories (
        "/home/fan/Downloads/googletest/googletest/build/lib"
)

link_libraries (
        "libgtest.a"
        "libgtest_main.a"
)


add_executable(TestAll tests_main.cpp)

add_executable(speedtest_p0 speedtest.cxx)
set_target_properties(speedtest_p0 PROPERTIES CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(speedtest_p1 speedtest.cxx)
set_target_properties(speedtest_p0 PROPERTIES CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")

add_executable(speedtest_p2 speedtest.cxx)
set_target_properties(speedtest_p0 PROPERTIES CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(speedtest_p3 speedtest.cxx)
set_target_properties(speedtest_p0 PROPERTIES CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
