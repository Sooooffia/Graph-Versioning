# Graph-Versioning

This is a sourcecode for experiments related to graph versioning. Which is supplement to a paper titled **To Store or Not to Store: a graph theoretical approach for
Dataset Versioning**.

## Authors
Anxin (Bob) Guo, Jingwei (Sofia) Li, Pattara Sukprasert, Samir Khuller, Amol Deshpande, Koyel Mukherjee.

## Prerequisites
We need the following packages:
- Gurobi
- CMake
- Make

## Instructions

To reproduce the experiments with CLion (recommended): Open the project using CLion, modify CMakeLists.txt according to local environment, and build & run project. 

To reproduce the experiments using command line (to be tested):
- run `cmake -Bcmake-build-debug -H. ` in the directory to reload subdirectory. 
- run `cmake --build cmake-build-debug --target` to build the project. 
- the excutable can be found in the ./cmake-build-eebug subdirectory. 

Note: modify main.cpp in order to run different experiments. 
