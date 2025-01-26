find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_NCDSSS gnuradio-ncdsss)

FIND_PATH(
    GR_NCDSSS_INCLUDE_DIRS
    NAMES gnuradio/ncdsss/api.h
    HINTS $ENV{NCDSSS_DIR}/include
        ${PC_NCDSSS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_NCDSSS_LIBRARIES
    NAMES gnuradio-ncdsss
    HINTS $ENV{NCDSSS_DIR}/lib
        ${PC_NCDSSS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-ncdsssTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_NCDSSS DEFAULT_MSG GR_NCDSSS_LIBRARIES GR_NCDSSS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_NCDSSS_LIBRARIES GR_NCDSSS_INCLUDE_DIRS)
