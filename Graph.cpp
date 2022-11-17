//
// Created by Anxin Guo on 11/15/22.
//

#include "Graph.h"

IntGraph::IntGraph(int node_number) {
    n = node_number;
}

int IntGraph::add_node(int node_count) {
    return 0;
}

void IntGraph::add_edge(int u, int v) {

}

void IntGraph::delete_node(int node) {

}

unordered_map<int, edge_variables> IntGraph::in_edges(int) {
    return unordered_map<int, edge_variables>();
}

unordered_map<int, edge_variables> IntGraph::out_edges(int) {
    return unordered_map<int, edge_variables>();
}

IntGraph generate_random_graph(int v_size, int e_size) {
    IntGraph G(1);
    // TODO: check how to generate random graphs
    return G;
}