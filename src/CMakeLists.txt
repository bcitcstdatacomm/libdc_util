# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)

if(APPLE)
    add_definitions(-D_DARWIN_C_SOURCE)
endif()

find_program(LINT "clang-tidy")
IF(LINT)
    set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet")
ENDIF()

# Make a shared library
add_library(dc_util SHARED ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(dc_util PRIVATE ../include/dc_util)
target_include_directories(dc_util PRIVATE /usr/include)
target_include_directories(dc_util PRIVATE /usr/local/include)
target_link_directories(dc_util PRIVATE /usr/local/lib)

set_target_properties(dc_util PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
find_library(LIB_M m)

target_link_libraries(dc_util PUBLIC ${LIBDC_ERROR})
target_link_libraries(dc_util PUBLIC ${LIBDC_POSIX})

if (LIB_M)
    target_link_libraries(dc_util PRIVATE ${LIB_M})
endif ()

# All users of this library will need at least C17
target_compile_features(dc_util PUBLIC c_std_17)
target_compile_options(dc_util PRIVATE -g -fPIC)
target_compile_options(dc_util PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(dc_util PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(dc_util PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

install(TARGETS dc_util LIBRARY DESTINATION lib)
install(FILES ${HEADER_LIST} DESTINATION include/dc_util)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

add_custom_target(
        format
        COMMAND clang-format
        -i
        ${HEADER_LIST}
        ${SOURCE_LIST}
)
