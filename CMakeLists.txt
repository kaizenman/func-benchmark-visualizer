cmake_minimum_required(VERSION 3.18...3.22)

project(sample_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

include(FetchContent)
# fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

# google test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

# google benchmark
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.5.0
)
FetchContent_MakeAvailable(benchmark)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable Google Benchmark's tests" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "Do not download dependencies from the internet" FORCE)


set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/profiler/profiler.h
)

set(SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(JSON_FILE ${CMAKE_BINARY_DIR}/results.json)

include_directories(${benchmark_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${fmt_SOURCE_DIR}/include})

add_executable(sample_app ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(sample_app fmt)

add_executable(func_bench ${CMAKE_CURRENT_SOURCE_DIR}/src/bm/func_bench.cpp)
target_link_libraries(func_bench benchmark fmt)
add_custom_target(run_func_bench 
  COMMAND func_bench ${BM_LIST} --benchmark_format=json > ${JSON_FILE}
  DEPENDS func_bench
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(plot_func_bench
  COMMAND "python3 ${CMAKE_CURRENT_SOURCE_DIR}/visualize.py ${JSON_FILE}"
)
