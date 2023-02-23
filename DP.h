#ifndef GRAPH_VERSIONING_DP_H
#define GRAPH_VERSIONING_DP_H
#include "Graph.hpp"

struct connection_type {
    int v, val;
    int child1;
    const connection_type *con1;
    int child2;
    const connection_type *con2;
};
struct DP_type {
    long long storage, retrieval, gamma;
    connection_type con;
};

IntGraph make_binary(const IntGraph &G, int r);

/**
 * Runs the DP on a binary arborescene.
 * @return min_rec: int, the retrieval cost of the resulting version graph.
 */
//int DP_arborescence(IntGraph &G, int r, double l, int S, int R_of_MST);

/// Trying geometric discretization.
map<int, edge_variables, std::greater<>> DP_arb_modified(const IntGraph &H, int r, double epsilon, int S, int R_of_MST);

tuple<IntGraph, IntGraph, unordered_map<int, map<int, DP_type, std::greater<>>>, unordered_map<int, unordered_map<int, unordered_map<int, unordered_map<int, DP_type>>>>>
        DP_bidirectional(const IntGraph &G, int r, double epsilon, long long S, long long R_of_MST);

tuple<IntGraph, IntGraph, unordered_map<int, map<int, DP_type>>, unordered_map<int, unordered_map<int, unordered_map<int, DP_type>>>, unordered_map<int, unordered_map<int, unordered_map<int, unordered_map<int, DP_type>>>>>
        DP_bidirectional_s(const IntGraph &G, int r, double epsilon, long long S, long long R_of_MST);

#endif //GRAPH_VERSIONING_DP_H
