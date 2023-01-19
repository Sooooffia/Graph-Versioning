//
// Created by Anxin Guo on 1/10/23.
//

#ifndef GRAPH_VERSIONING_LMG_H
#define GRAPH_VERSIONING_LMG_H
#include "Graph.hpp"
#include "MST/gabow.h"

/** @brief Initialization helper: calculating dependency and retrieval for the first time.
 * @param H: initial IntGraph.
 * @param dependency_count: int -> int. dependency_count[v] is the number of dependent nodes of v in H, including v itself.
 * @param retrieval_cost: int -> int. The retrieval cost of each v in H.
 */
void initialize_variables_LMG(IntGraph& H, unordered_map<int, int>& dependency_count,
                               unordered_map<int, int>& retrieval_cost);

/**
 * @param G: IntGraph, Version Graph
 * @param S: int, Storage budget
 * @returns H: IntGraph.
 */
IntGraph LMG(const IntGraph& G, int S);


/** @brief Initialization helper: calculating dependency and retrieval for the first time.
 * @param H: initial IntGraph.
 * @param dependency_count: int -> set<int>. dependency_count[v] is the list of dependent nodes of v in H, including v itself.
 * @param retrieval_cost: int -> int. The retrieval cost of each v in H.
 */
void initialize_variables_LMG_all(IntGraph& H, unordered_map<int, unordered_set<int>>& dependency_list,
                              unordered_map<int, int>& retrieval_cost);

/**
 * @param G: IntGraph, Version Graph
 * @param S: int, Storage budget
 * @returns H: IntGraph.
 */
IntGraph LMG_all(const IntGraph& G, int S);

#endif //GRAPH_VERSIONING_LMG_H
