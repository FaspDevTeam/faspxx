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
    ParVEC.cxx
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
    ParVEC.hxx
    ReadData.hxx
    RetCode.hxx
    SOL.hxx
    Timing.hxx
    Umfpack.hxx
    VEC.hxx
    VECUtil.hxx
    )

convert_filenames_to_full_paths(SRCS)
convert_filenames_to_full_paths(HDRS)

set(SOURCES ${SOURCES} ${SRCS} PARENT_SCOPE)
set(HEADERS ${HEADERS} ${HDRS} PARENT_SCOPE)