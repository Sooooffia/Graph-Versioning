cmake_minimum_required(VERSION 3.22)
project(Graph-Versioning)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")
add_executable(Graph-Versioning main.cpp Graph.cpp LMG.cpp GraphIO.h DP.cpp test_DP.h script_LMG_all.h MST/dsu.cpp MST/gabow.cpp MST/active_forest.cpp)

#TODO: try find_library if everything else works
#find_library(Gurobi_c++ NAMES libgurobi_c++.a libgurobi100.dylib PATH /Library/gurobi1000/macos_universal2/src/build
#        /Library/gurobi1000/macos_universal2/lib)
#target_link_libraries(Graph-Versioning Gurobi_c++)

#if(NOT Gurobi_c++)
#    message(FATAL_ERROR "Failed to find Gurobi lib.  Try setting CMAKE_PREFIX_PATH")
#endif()
#target_link_libraries(Graph-Versioning ${Gurobi_c++})
# This is for Pat's mac.
#find_library(GUROBI_LIB NAMES "libgurobi_c++.a" "libgurobi95.dylib" HINTS "/Library/gurobi951/macos_universal2/src/build"
        #"/Library/gurobi951/macos_universal2/lib")
#target_link_libraries(Graph-Versioning /Library/gurobi951/macos_universal2/src/build)
#target_link_libraries(Graph-Versioning /Library/gurobi951/macos_universal2/src/build/libgurobi_c++.a)
#target_link_libraries(Graph-Versioning /Library/gurobi951/macos_universal2/lib/libgurobi95.dylib)
#target_include_directories(Graph-Versioning PUBLIC /Library/gurobi951/macos_universal2/include)

#TODO: This is for Bob's mac.
##find_library(GUROBI_LIB NAMES "libgurobi_c++.a" "libgurobi100.dylib" HINTS "/Library/gurobi1000/macos_universal2/src/build"
##        "/Library/gurobi1000/macos_universal2/lib")
##target_link_libraries(Graph-Versioning GUROBI_LIB)
#target_link_libraries(Graph-Versioning /Library/gurobi1000/macos_universal2/src/build/libgurobi_c++.a)
#target_link_libraries(Graph-Versioning /Library/gurobi1000/macos_universal2/lib/libgurobi100.dylib)
#target_include_directories(Graph-Versioning PUBLIC /Library/gurobi1000/macos_universal2/include)
#
#add_subdirectory(MST)
#OPTION (USE_OpenMP "Use OpenMP to enamble <omp.h>" ON)

#TODO: This is for Sofia's PC.
target_link_libraries(Graph-Versioning C:/gurobi1000/win64/src/build/gurobi_c++mt.lib)
target_link_libraries(Graph-Versioning C:/gurobi1000/win64/lib/gurobi_c++md2017.lib)
target_link_libraries(Graph-Versioning C:/gurobi1000/win64/lib/gurobi_c++mdd2017.lib)
target_link_libraries(Graph-Versioning C:/gurobi1000/win64/lib/gurobi_c++mt2017.lib)
target_link_libraries(Graph-Versioning C:/gurobi1000/win64/lib/gurobi_c++mtd2017.lib)
target_link_libraries(Graph-Versioning C:/gurobi1000/win64/lib/gurobi100.lib)
target_include_directories(Graph-Versioning PUBLIC C:/gurobi1000/win64/include)

#TODO: figure out what this does. (this is from the MST library)
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


