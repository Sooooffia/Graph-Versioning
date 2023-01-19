add_library(versioning active_forest.h compressed_tree.h compressed_tree.inl gabow.h gabow.cpp active_forest.cpp ../LMG.h)
target_link_libraries(Graph-Versioning versioning)

#add_executable(gabow.cpp active_forest.cpp)