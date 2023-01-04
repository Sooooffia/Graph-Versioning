cmake_minimum_required(VERSION 3.23)
project(Graph-Versioning)
add_executable(Graph-Versioning main.cpp Graph.cpp)
set(CMAKE_CXX_STANDARD 20)
#add_library(Graph.cpp)
target_include_directories(Graph-Versioning PUBLIC "${PROJECT_BINARY_DIR}")
add_subdirectory(MST)



