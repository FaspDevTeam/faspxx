1. What is the difference between gtest and gmock?
ref: https://stackoverflow.com/questions/13696376/what-is-the-difference-between-gtest-and-gmock

Let say you want to write something to file. You have to test like system memory is full or not.
Will you make the system memory full to test this? No. Your friend Google mock will help you here.

2. 安装Google Test
cmake ..　-Dgtest_build_samples=ON　-DCMAKE_CXX_STANDARD=11 -DCMAKE_CXX_STANDARD_REQUIRED=ON
make
不需要make install

也可以如下修改CMakeLists.txt:

option(gtest_build_samples "Build gtest's sample programs." OFF) # 同时编译gtest的samples

set (CMAKE_CXX_STANDARD 11)           # 以c++11标准编译
set (CMAKE_CXX_STANDARD_REQUIRED ON)  # 以c++11标准编译

3.　编写测试的基本步骤:
// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.(注意区分二者)
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.

// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.

// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.

// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.

// Step 3. Call RUN_ALL_TESTS() in main().
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?

4. test fixture
// A test fixture is a place to hold objects and functions shared by
// all tests in a test case.  Using a test fixture avoids duplicating
// the test code necessary to initialize and cleanup those common
// objects for each test.  It is also useful for defining sub-routines
// that your tests need to invoke a lot.

// The tests share the test fixture in the sense of code sharing, not
// data sharing.  Each test is given its own fresh copy of the
// fixture.  You cannot expect the data modified by one test to be
// passed on to another test, which is a bad idea.

// The reason for this design is that tests should be independent and
// repeatable.  In particular, a test should not fail as the result of
// another test's failure.  If one test depends on info produced by
// another test, then the two tests should really be one big test.

// The macros for indicating the success/failure of a test
// (EXPECT_TRUE, FAIL, etc) need to know what the current test is
// (when Google Test prints the test result, it tells you which test
// each failure belongs to).  Technically, these macros invoke a
// member function of the Test class.  Therefore, you cannot use them
// in a global function.  That's why you should put test sub-routines
// in a test fixture.

5. sub-test-fixture
// This sample teaches how to reuse a test fixture in multiple test
// cases by deriving sub-fixtures from it.
//
// When you define a test fixture, you specify the name of the test
// case that will use this fixture.  Therefore, a test fixture can
// be used by only one test case.
//
// Sometimes, more than one test cases may want to use the same or
// slightly different test fixtures.  For example, you may want to
// make sure that all tests for a GUI library don't leak important
// system resources like fonts and brushes.  In Google Test, you do
// this by putting the shared logic in a super (as in "super class")
// test fixture, and then have each test case use a fixture derived
// from this super fixture.

6. 仔细阅读googletest/samples 下的 6,7,8,9,10.

7. 参考: https://www.cnblogs.com/coderzh/archive/2009/03/31/1426758.html
EXPECT系列和ASSERT系列的区别是：
    1. EXPECT_*  失败时，案例继续往下执行。
    2. ASSERT_* 失败时，直接在当前函数中返回，当前函数中ASSERT_*后面的语句将不会执行。

7.gtest系列之事件机制(section 3 in ref: https://blog.csdn.net/ONEDAY_789/article/details/76718463)
“事件” 本质是框架给你提供了一个机会, 让你能在这样的几个机会来执行你自己定制的代码, 来给测试用例准备/清理数据。gtest提供了多种事件机制，总结一下gtest的事件一共有三种：
1、TestSuite事件
需要写一个类，继承testing::Test，然后实现两个静态方法：SetUpTestCase方法在第一个TestCase之前执行；TearDownTestCase方法在最后一个TestCase之后执行。
2、TestCase事件
是挂在每个案例执行前后的，需要实现的是SetUp方法和TearDown方法。SetUp方法在每个TestCase之前执行；TearDown方法在每个TestCase之后执行。
3、全局事件
要实现全局事件，必须写一个类，继承testing::Environment类，实现里面的SetUp和TearDown方法。SetUp方法在所有案例执行前执行；TearDown方法在所有案例执行后执行。
例如全局事件可以按照下列方式来使用：
除了要继承testing::Environment类，还要定义一个该全局环境的一个对象并将该对象添加到全局环境测试中去。
