cmake_minimum_required(VERSION 3.22)
project(Graph-Versioning)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O2")
add_executable(Graph-Versioning main.cpp Graph.cpp LMG.cpp)
target_include_directories(Graph-Versioning PUBLIC "${PROJECT_BINARY_DIR}")
add_subdirectory(MST)



