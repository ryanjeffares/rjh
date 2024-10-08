cmake_minimum_required(VERSION 3.28)
project(rjh)

set(CMAKE_CXX_STANDARD 23)
set(BENCHMARK_ENABLE_TESTING OFF)

include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
        GIT_TAG         v3.7.1
        GIT_SHALLOW     ON
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
        benchmark
        GIT_REPOSITORY  https://github.com/google/benchmark.git
        GIT_TAG         v1.9.0
        GIT_SHALLOW     ON
)
FetchContent_MakeAvailable(benchmark)

set(RJH_OPTIONS -Wall -Wextra -Wpedantic -Werror)

add_library(rjh STATIC src/rjh.cpp)
target_include_directories(rjh PRIVATE include)
target_compile_options(rjh PRIVATE ${RJH_OPTIONS})

add_executable(rjh_test test/rjh_unordered_map_test.cpp test/rjh_unordered_set_test.cpp)
target_include_directories(rjh_test PRIVATE include)
target_link_libraries(rjh_test PRIVATE rjh Catch2::Catch2WithMain)
target_compile_options(rjh_test PRIVATE ${RJH_OPTIONS} -fsanitize=address,undefined)

add_executable(rjh_benchmark benchmark/rjh_benchmark.cpp)
target_include_directories(rjh_benchmark PRIVATE include)
target_link_libraries(rjh_benchmark PRIVATE rjh benchmark::benchmark_main)