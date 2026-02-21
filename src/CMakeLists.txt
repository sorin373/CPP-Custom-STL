cmake_minimum_required(VERSION 3.16)

project(stl VERSION 1.0.0 LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
        "In-source builds are not allowed.\n"
        "Please run: cmake -S . -B build"
    )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    # testing/test.cpp
    # benchmark/benchmark_vector.cpp
    # benchmark/benchmark_umap.cpp
    benchmark/benchmark_forward_list.cpp
)