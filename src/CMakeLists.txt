# Get the parent directory of the current source directory
# get_parent_directory(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Include the CMakeLists.txt from the parent directory to access the faspxx target
# include(${PARENT_DIR}/CMakeLists.txt)

# Set the source and header files
set(SRCS
    BiCGStab.cxx
    CG.cxx
    FGMRES.cxx
    GMRES.cxx
    Iter.cxx
    Krylov.cxx
    LOP.cxx
    MAT.cxx
    MATUtil.cxx
    MG.cxx
    Param.cxx
    ReadData.cxx
    RetCode.cxx
    SOL.cxx
    Timing.cxx
    Umfpack.cxx
    VEC.cxx
    VECUtil.cxx
)

set(HDRS
    BiCGStab.hxx
    CG.hxx
    Doxygen.hxx
    ErrorLog.hxx
    FGMRES.hxx
    GMRES.hxx
    Iter.hxx
    Krylov.hxx
    LOP.hxx
    MAT.hxx
    MATUtil.hxx
    MG.hxx
    Param.hxx
    ReadData.hxx
    RetCode.hxx
    SOL.cxx
    Timing.cxx
    Umfpack.hxx
    VEC.hxx
    VECUtil.hxx
)

# Add the source files to the faspxx target
target_sources(faspxx PRIVATE ${SRCS})

# Add the header files to the faspxx target
target_sources(faspxx PRIVATE ${HDRS})

# If needed, you can handle any additional steps related to these sources and headers
# For example, if you have specific compile options for these files, you could do:
# target_compile_options(faspxx PRIVATE some_compile_option)

# If there's a need to convert file paths (though usually not necessary in modern CMake)
# and the convert_filenames_to_full_paths function is available and needed:
# convert_filenames_to_full_paths(SRCS)
# convert_filenames_to_full_paths(HDRS)
# target_sources(faspxx PRIVATE ${SRCS} ${HDRS})