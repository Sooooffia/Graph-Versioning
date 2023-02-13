#ifndef GRAPH_VERSIONING_DP_H
#define GRAPH_VERSIONING_DP_H
#include "Graph.hpp"

IntGraph make_binary(const IntGraph &G, int r);

/**
 * Runs the DP on a binary arborescene.
 * @return min_rec: int, the retrieval cost of the resulting version graph.
 */
int DP_arborescence(IntGraph &G, int r, double l, int S, int R_of_MST);

/// Trying geometric discretization.
map<int, edge_variables, std::greater<int>> DP_arb_modified(IntGraph &G, int r, double epsilon, int S, int R_of_MST);

#endif //GRAPH_VERSIONING_DP_H
