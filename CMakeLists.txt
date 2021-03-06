# https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/CMakeLists.txt

cmake_minimum_required(VERSION 3.21)

# Project name and a few useful settings. Other commands can pick up the results
project(libdc_util
        VERSION 0.1.0.0
        HOMEPAGE_URL "https://github.com/bcitcstdatacomm/libdc_util"
        DESCRIPTION ""
        LANGUAGES C)

set(HEADER_LIST
        "${libdc_util_SOURCE_DIR}/include/dc_util/bits.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/dump.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/filesystem.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/networking.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/path.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/streams.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/strings.h"
        "${libdc_util_SOURCE_DIR}/include/dc_util/types.h"
        )

set(SOURCE_LIST
        "${libdc_util_SOURCE_DIR}/src/bits.c"
        "${libdc_util_SOURCE_DIR}/src/dump.c"
        "${libdc_util_SOURCE_DIR}/src/filesystem.c"
        "${libdc_util_SOURCE_DIR}/src/networking.c"
        "${libdc_util_SOURCE_DIR}/src/path.c"
        "${libdc_util_SOURCE_DIR}/src/streams.c"
        "${libdc_util_SOURCE_DIR}/src/strings.c"
        "${libdc_util_SOURCE_DIR}/src/types.c"
        )

### Require out-of-source builds
# this still creates a CMakeFiles directory and CMakeCache.txt- can we delete them?
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif ()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

    # Let's ensure -std=c11 instead of -std=g11
    set(CMAKE_C_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if (Doxygen_FOUND)
        message(STATUS "Doxygen found, building docs")
        add_subdirectory(docs)
    else ()
        message(STATUS "Doxygen not found, not building docs")
    endif ()
endif ()

# The compiled library code is here
add_subdirectory(src)

# Testing only available if this is the main app
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()
