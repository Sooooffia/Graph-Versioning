//
// Created by Anxin Guo on 11/15/22.
//

#ifndef DATA_VERSIONING_C_GRAPH_H
#define DATA_VERSIONING_C_GRAPH_H

#include "vector"
#include "unordered_set"
#include "unordered_map"
#include "boost/graph/adjacency_list.hpp"

/**
 *
 */
class IntGraph {
public:
    // Variables //
    int n = 0;
    std::unordered_set<int> V;
    // we will have a set of predecessors and successors instead of edges.
    std::unordered_map<int, std::unordered_set<int>> pred;
    std::unordered_map<int, std::unordered_set<int>> succ;

    // Constructors //
    IntGraph() = default;
    /// @param node_number: number of nodes in the new empty graph.
    explicit IntGraph(int node_number) {
        n = node_number;
    }

    // Functions //

};

/**
 * @param v_size
 * @param e_size
 * @return a random graph
 */
IntGraph generate_random_graph(int v_size, int e_size) {
    IntGraph G(1);
    // TODO: check how to generate random graphs
    return G
}


#endif //DATA_VERSIONING_C_GRAPH_H
