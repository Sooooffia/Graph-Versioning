cmake_minimum_required(VERSION 3.22)
project(Graph-Versioning)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20 -O3")
add_executable(Graph-Versioning main.cpp Graph.cpp LMG.cpp OPT.cpp)
target_include_directories(Graph-Versioning PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(Graph-Versioning PUBLIC /Library/gurobi1000/macos_universal2/include)
add_subdirectory(MST)
OPTION (USE_OpenMP "Use OpenMP to enamble <omp.h>" ON)
#if(APPLE AND USE_OpenMP)
#    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
#        set(OpenMP_C "${CMAKE_C_COMPILER}")
#        set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
#        set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
#        set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
#        set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
#        set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
#    endif()
#    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#        set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
#        set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
#        set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
#        set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
#        set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
#        set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
#    endif()
#endif()

#cmake_minimum_required(VERSION 3.22)
#project(Graph-Versioning)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
#set (CMAKE_CXX_STANDARD 20)
#
#
#if(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
#    add_compile_options(-march=core-avx2)
#endif()
#if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    add_compile_options(-mcpu=apple-m1)
#    add_compile_options(-Wall -Wextra -Wshadow -Wfloat-equal -Wconversion -Wno-sign-compare)
#    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#        add_compile_options(
#                -fsanitize=address,undefined
#                -fno-omit-frame-pointer
#                -fno-sanitize-recover=all
#        )
#        add_link_options(
#                -fsanitize=address,undefined
#                -fno-omit-frame-pointer
#        )
#    endif()
#endif()
#add_executable(Graph-Versioning main.cpp Graph.cpp DP.h LMG.cpp OPT.h OPT.cpp)
#target_include_directories(Graph-Versioning PUBLIC "${PROJECT_BINARY_DIR}")


