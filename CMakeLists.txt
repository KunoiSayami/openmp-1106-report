cmake_minimum_required(VERSION 3.17)
project(openmp_1105)

set(CMAKE_CXX_STANDARD 14)

add_executable(openmp_1105 main.cpp)

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    target_link_libraries(openmp_1105 PUBLIC OpenMP::OpenMP_CXX)
endif()