set(TEST_HEADER_LIST
        tests.h
        )

set(TEST_SOURCE_LIST
        main.c
        )

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})
add_executable(libdc_util_test ${TEST_SOURCE_LIST} ${TEST_HEADER_LIST} ${SOURCE_LIST} ${HEADER_LIST})

target_compile_features(libdc_util_test PRIVATE c_std_17)

target_include_directories(libdc_util_test PRIVATE ../include)
target_include_directories(libdc_util_test PRIVATE /usr/local/include)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(libdc_util_test PRIVATE /opt/homebrew/include)
else ()
    target_include_directories(libdc_util_test PRIVATE /usr/include)
endif ()

find_library(LIBCGREEN cgreen REQUIRED)
find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_ENV dc_env REQUIRED)
find_library(LIBDC_C dc_c REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
find_library(LIBDC_UNIX dc_unix REQUIRED)
find_library(LIBBSD bsd)

target_link_libraries(libdc_util_test PRIVATE ${LIBCGREEN})
target_link_libraries(libdc_util_test PRIVATE ${LIBDC_ERROR})
target_link_libraries(libdc_util_test PRIVATE ${LIBDC_ENV})
target_link_libraries(libdc_util_test PRIVATE ${LIBDC_C})
target_link_libraries(libdc_util_test PRIVATE ${LIBDC_POSIX})
target_link_libraries(libdc_util_test PRIVATE ${LIBDC_UNIX})

if(LIBBSD)
    target_link_libraries(libdc_util_test PUBLIC ${LIBBSD})
endif()

add_test(NAME libdc_util_test COMMAND libdc_posix_test)

