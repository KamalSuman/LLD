cmake_minimum_required(VERSION 3.23)

project(
    machine_coding_lld
    VERSION 0.1.0
    DESCRIPTION "C++ low-level design practice"
    LANGUAGES CXX
)

option(LLD_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

add_library(lld_project_options INTERFACE)
target_compile_features(lld_project_options INTERFACE cxx_std_20)

if(MSVC)
    target_compile_options(lld_project_options INTERFACE /W4 /permissive-)
    if(LLD_WARNINGS_AS_ERRORS)
        target_compile_options(lld_project_options INTERFACE /WX)
    endif()
else()
    target_compile_options(
        lld_project_options
        INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
    )
    if(LLD_WARNINGS_AS_ERRORS)
        target_compile_options(lld_project_options INTERFACE -Werror)
    endif()
endif()

# Keep MinGW-built practice executables runnable outside the compiler's bin directory.
if(MINGW)
    target_link_options(lld_project_options INTERFACE -static)
endif()

include(CTest)

add_subdirectory(problems/hello_lld)
add_subdirectory(problems/parking_lot)
