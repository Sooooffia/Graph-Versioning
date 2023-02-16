#ifndef GRAPH_VERSIONING_DP_H
#define GRAPH_VERSIONING_DP_H
#include "Graph.hpp"

struct DP_type {
    int storage, retrieval, gamma;
};

IntGraph make_binary(const IntGraph &G, int r);

/**
 * Runs the DP on a binary arborescene.
 * @return min_rec: int, the retrieval cost of the resulting version graph.
 */
//int DP_arborescence(IntGraph &G, int r, double l, int S, int R_of_MST);

/// Trying geometric discretization.
map<int, edge_variables, std::greater<>> DP_arb_modified(const IntGraph &H, int r, double epsilon, int S, int R_of_MST);

map<int, DP_type, std::greater<>> DP_bidirectional(const IntGraph &H, int r, double epsilon, int S, int R_of_MST);

#endif //GRAPH_VERSIONING_DP_H
