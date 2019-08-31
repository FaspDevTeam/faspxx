


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


add_executable(test_ex gtest_examples.cpp)

