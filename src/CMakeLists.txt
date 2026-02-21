cmake_minimum_required(VERSION 3.16...3.29)

project(stl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR
    "In-source builds not allowed.\n"
    "Create a build directory and run CMake from there, e.g.:\n"
    "  cmake -S . -B build\n"
    "  cmake --build build\n")
endif()

option(STL_BUILD_TESTS      "Build unit tests" OFF)
option(STL_BUILD_BENCHMARKS "Build benchmarks" ON)
option(STL_ENABLE_ASAN      "Enable AddressSanitizer (GCC/Clang)" OFF)
option(STL_ENABLE_UBSAN     "Enable UndefinedBehaviorSanitizer (GCC/Clang)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(NOT COMPILER_SUPPORTS_CXX17)
  message(FATAL_ERROR "The compiler does not support -std=c++17.")
endif()

add_library(stl_project_options INTERFACE)

if(MSVC)
  target_compile_options(stl_project_options INTERFACE /W4 /permissive-)
else()
  target_compile_options(stl_project_options INTERFACE
    -Wall -Wextra -Wpedantic
    -Wconversion -Wsign-conversion
    -Wshadow
  )
endif()

if(NOT MSVC)
  if(STL_ENABLE_ASAN)
    target_compile_options(stl_project_options INTERFACE -fsanitize=address)
    target_link_options(stl_project_options    INTERFACE -fsanitize=address)
  endif()

  if(STL_ENABLE_UBSAN)
    target_compile_options(stl_project_options INTERFACE -fsanitize=undefined)
    target_link_options(stl_project_options    INTERFACE -fsanitize=undefined)
  endif()
endif()

if(STL_BUILD_BENCHMARKS)
  add_executable(stl_bench_umap benchmark/benchmark_umap.cpp)
  target_link_libraries(stl_bench_umap PRIVATE stl_project_options)
endif()

if(STL_BUILD_TESTS)
  enable_testing()
  add_executable(stl_tests testing/test.cpp)
  target_link_libraries(stl_tests PRIVATE stl_project_options)
  add_test(NAME stl_tests COMMAND stl_tests)
endif()