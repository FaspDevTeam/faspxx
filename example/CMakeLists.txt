set(EXAMPLES_SRCS "")

list(APPEND EXAMPLES_SRCS
    TestCG.cxx
    TestGMRES.cxx
    TestInverse.cxx
    TestJacobi.cxx
    TestKrylov.cxx
    TestMatFree.cxx
    TestMG.cxx
    TestPCG.cxx
    TestReadData.cxx
    TestSolver.cxx
    TestVecSpeed.cxx
    TestWeightedJacobi.cxx
    )

# Add one executable per .cxx file
add_all_examples(EXAMPLES_SRCS)
