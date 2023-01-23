//
// Created by Anxin Guo on 1/10/23.
//

#ifndef GRAPH_VERSIONING_LMG_H
#define GRAPH_VERSIONING_LMG_H
#include "Graph.hpp"
#include "MST/gabow.h"

/**
 * @param G: IntGraph, Version Graph
 * @param S: int, Storage budget
 * @returns H: IntGraph.
 */
IntGraph LMG(const IntGraph& G, int S);

/**
 * @param G: IntGraph, Version Graph
 * @param S: int, Storage budget
 * @returns H: IntGraph.
 */
IntGraph LMG_all(const IntGraph& G, int S);

#endif //GRAPH_VERSIONING_LMG_H
