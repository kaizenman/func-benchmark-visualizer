cmake_minimum_required(VERSION 3.18...3.22)

project(sample_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

include(FetchContent)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.5.0
)
FetchContent_MakeAvailable(benchmark)

set(SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)


include_directories(${benchmark_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(sample_app ${SOURCE_FILES})
add_executable(func_bench ${CMAKE_CURRENT_SOURCE_DIR}/src/bm/func_bench.cpp)
target_link_libraries(func_bench benchmark)
