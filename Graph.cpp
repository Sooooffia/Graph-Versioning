//
// Created by Anxin Guo on 11/15/22.
//

#include "Graph.h"

IntGraph::IntGraph(int node_number) {
    n = node_number;
}

// TODO: need to figure out how we want to add nodes
int IntGraph::add_node(int node_count) {
    return 0;
}

void IntGraph::add_or_modify_edge(int u, int v, edge_variables costs) {
    if (u == v){
        throw invalid_argument("Self-loop error in add_or_modify_edge.");
    }
    if (nodes.find(u) == nodes.end() || nodes.find(v) == nodes.end()){
        throw invalid_argument("Nodes cannot be found in add_or_modify_edge.");
    }
    inNeighbors[v].insert_or_assign(u,costs);
    outNeighbors[u].insert_or_assign(u,costs);
}

void IntGraph::delete_edge(int u, int v) {
    int ctr = 0;
    if (nodes.find(v) != nodes.end() && inNeighbors[v].find(u) != inNeighbors[v].end()){
        if (nodes.find(u) == nodes.end() || outNeighbors[u].find(v) == outNeighbors[u].end()) {
            throw logic_error("Error with symmetry for inputs in delete_edge");
        }
        inNeighbors[v].erase(u);
        outNeighbors[u].erase(v);
        m --;
    }
}

void IntGraph::delete_node(int v) {
    if (nodes.find(v) == nodes.end()){
        throw invalid_argument("Nodes cannot be found in delete_node.");
    }
    nodes.erase(v);
    for (int u : nodes){
        delete_edge(u,v);
        delete_edge(v,u);
    }
}

unordered_map<int, edge_variables> IntGraph::in_edges(int v) {
    return inNeighbors[v];
}

unordered_map<int, edge_variables> IntGraph::out_edges(int v) {
    return outNeighbors[v];
}

int IntGraph::edge_size() const {
    return m;
}

int IntGraph::size() const {
    return n;
}

unordered_set<int> IntGraph::in_neighbors(int) {
    return unordered_set<int>();
}

unordered_set<int> IntGraph::out_neighbors(int) {
    return unordered_set<int>();
}





IntGraph generate_random_graph(int v_size, int e_size) {
    IntGraph G(1);
    // TODO: check how to generate random graphs
    return G;
}