#ifndef GRAPH_VERSIONING_DP_H
#define GRAPH_VERSIONING_DP_H
#include "Graph.hpp"

/**
 * Runs the DP on a binary arborescene.
 * @return min_rec: int, the retrieval cost of the resulting version graph.
 */
int DP_arborescence(IntGraph &G, int r, double epsilon, int S, int R_max);

#endif //GRAPH_VERSIONING_DP_H
