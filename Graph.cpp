//
// Created by Anxin Guo on 11/15/22.
//

#include "Graph.h"


// Helper functions
void IntGraph::make_node(int v){
    nodes.insert(v);
    inNeighbors.insert({v,unordered_map<int,edge_variables>()});
    outNeighbors.insert({v,unordered_map<int,edge_variables>()});
}
void IntGraph::make_edge(int u, int v, edge_variables costs){
    inNeighbors[v].insert_or_assign(u,costs);
    outNeighbors[u].insert_or_assign(u,costs);
}

// Constructors

IntGraph::IntGraph() {
    make_node(0);
}
IntGraph::IntGraph(int node_number) {
    make_node(0);
    for (int i = 1; i <= node_number; i ++){
        make_node(i);
        make_edge(0,i,{0,0});
    }
}
IntGraph::IntGraph(const vector<tuple<int,int>>& vertices, const vector<tuple<int, int, edge_variables>>& edges) {
    make_node(0);
    for (auto v : vertices){
        int vertex = get<0>(v);
        int storage =  get<1>(v);
        make_node(vertex);
        n++;
        make_edge(0,vertex,{storage,0});
    }
    for (auto e : edges) {
        int pred = get<0>(e);
        int succ = get<1>(e);
        edge_variables costs = get<2>(e);
        if (pred == 0 || succ == 0){
            throw invalid_argument("Invalid initialization: auxiliary root involved in edges.");
        }
        make_edge(pred,succ,costs);
    }
}

// Accessors
unordered_set<int> IntGraph::get_nodes() {
    return nodes;
}
unordered_map<int, edge_variables> IntGraph::get_in_edges_of(int v) {
    return inNeighbors[v];
}
unordered_map<int, edge_variables> IntGraph::get_out_edges_of(int v) {
    return outNeighbors[v];
}
vector<tuple<int, int, edge_variables>> IntGraph::get_edges() {
    vector<tuple<int, int, edge_variables>> output;
    for (auto u : nodes){
        for (auto e : outNeighbors[u]){
            int v = get<0>(e);
            edge_variables costs = get<1>(e);
            output.emplace_back(u,v,costs);
        }
    }
    return output;
}
int IntGraph::get_nodes_size() const {
    return n;
}
vector<int> IntGraph::get_in_neighbors_of(int v) {
    vector<int> output;
    for (auto u: inNeighbors[v]){
        output.push_back(u.first);
    }
    return output;
}
vector<int> IntGraph::get_out_neighbors_of(int v) {
    vector<int> output;
    for (auto u: outNeighbors[v]){
        output.push_back(u.first);
    }
    return output;
}


// Modifiers
vector<int> IntGraph::add_node(int node_count) {
    int i = 1;
    vector<int> output;
    while (node_count != 0){
        if (nodes.find(i) == nodes.end()){
            make_node(i);
            make_edge(0,i,{0,0});
            output.push_back(i);
            n ++;
            node_count --;
        }
        i++;
    }
    return output;
}
vector<int> IntGraph::add_note_with_storage(vector<tuple<int, int>> vertices) {
    vector<int> output;
    for (auto vertex : vertices){
        int v = get<0>(vertex);
        int storage = get<1>(vertex);
        make_node(v);
        make_edge(0,v,{storage,0});
        output.push_back(v);
    }
    return output;
}
void IntGraph::add_or_modify_edge(int u, int v, edge_variables costs) {
    if (u == v){
        throw invalid_argument("Self-loop error in add_or_modify_edge.");
    }
    if (nodes.find(u) == nodes.end() || nodes.find(v) == nodes.end()){
        throw invalid_argument("Nodes cannot be found in add_or_modify_edge.");
    }
    make_edge(u,v,costs);
}
void IntGraph::delete_edge(int u, int v) {
    int ctr = 0;
    if (nodes.find(v) != nodes.end() && inNeighbors[v].find(u) != inNeighbors[v].end()){
        if (nodes.find(u) == nodes.end() || outNeighbors[u].find(v) == outNeighbors[u].end()) {
            throw logic_error("Error with symmetry for inputs in delete_edge");
        }
        inNeighbors[v].erase(u);
        outNeighbors[u].erase(v);
    }
}
void IntGraph::delete_node(int v) {
    if (nodes.find(v) == nodes.end()){
        throw invalid_argument("Nodes cannot be found in delete_node.");
    }
    nodes.erase(v);
    n --;
    for (int u : nodes){
        delete_edge(u,v);
        delete_edge(v,u);
    }
}




IntGraph generate_random_graph(int v_size, int e_size) {
    IntGraph G(1);
    // TODO: generate random graphs
    return G;
}