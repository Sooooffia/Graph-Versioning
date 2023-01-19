//
// Created by Anxin Guo on 1/10/23.
//

#ifndef GRAPH_VERSIONING_LMG_H
#define GRAPH_VERSIONING_LMG_H
#include "Graph.hpp"
#include "MST/gabow.h"

/// Initialization helper: calculating dependency and retrieval for the first time.
void initialize_dependency_and_retrieval(IntGraph& H, unordered_map<int, int>& dependency_count,
                                         unordered_map<int, int>& retrieval_cost);

/**
 * @param G: IntGraph, Version Graph
 * @param S: int, Storage budget
 * @returns H: an arborescence.
 */
IntGraph LMG(const IntGraph& G, int S);

#endif //GRAPH_VERSIONING_LMG_H
