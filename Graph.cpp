//
// Created by Anxin Guo on 11/15/22.
//

#include "Graph.hpp"


// Helper functions //
void IntGraph::make_node(int v){
    nodes.insert(v);
    inNeighbors.insert({v,unordered_map<int,edge_variables>()});
    outNeighbors.insert({v,unordered_map<int,edge_variables>()});
}
void IntGraph::make_edge(int u, int v, const edge_variables& costs){
    inNeighbors[v][u] = costs;
    outNeighbors[u][v] = costs;
}

// Constructors
IntGraph::IntGraph() { make_node(0); }
IntGraph::IntGraph(int node_number) {
    make_node(0);
    while (n < node_number) {
        n++;
        make_node(n);
        make_edge(0,n,{0,0});
    }
}
IntGraph::IntGraph(const vector<tuple<int,int>>& vertices, const vector<tuple<int, int, edge_variables>>& edges) {
    make_node(0);
    for (auto v : vertices){
        int& vertex = get<0>(v);
        int& storage =  get<1>(v);
        make_node(vertex);
        n++;
        make_edge(0,vertex,{storage,0});
    }
    for (auto e : edges) {
        int& pred = get<0>(e);
        int& succ = get<1>(e);
        auto& costs = get<2>(e);
        if (pred == 0 || succ == 0){
            throw invalid_argument("Invalid initialization: auxiliary root involved in edges.");
        }
        make_edge(pred,succ,costs);
    }
}
IntGraph::IntGraph(int node_count, int p, bool equal_weights) {
    // lambda
    auto make_random_edge = [equal_weights, this](int u, int v) {
        if (equal_weights) {
            int weight = rand() % 100 + 1;
            make_edge(u, v, {weight, weight});
        } else {
            make_edge(u, v, { rand() % 100 + 1,  rand() % 100 + 1});
        }
    };
    make_node(0);
    while (n < node_count) {
        n++;
        make_node(n);
        make_edge(0,n,{rand() % 100 + 1,0});
    }
    for (int i = 1; i < n; i ++) {
        for (int j = i+1; j <= n; j++) {
            if (rand() < p * RAND_MAX)
                make_random_edge(i, j);
            if (rand() < p * RAND_MAX)
                make_random_edge(j, i);
        }
    }
}

// Accessors //
unordered_set<int> IntGraph::get_nodes(bool aux) const {
    if (aux) {
        return nodes;
    } else {
        unordered_set<int> ans = nodes;
        ans.erase(0);
        return ans;
    }
}
unordered_map<int, edge_variables> IntGraph::get_in_edges_of(int v,bool aux) const {
    unordered_map<int, edge_variables> output = inNeighbors.at(v);
    if (!aux){
        output.erase(0);
    }
    return output;
}
unordered_map<int, edge_variables> IntGraph::get_out_edges_of(int v) const {
    return outNeighbors.at(v);
}
vector<tuple<int, int, edge_variables>> IntGraph::get_edges(bool aux) const {
    vector<tuple<int, int, edge_variables>> output;
    auto valid_nodes = get_nodes(aux);

    for (auto u : valid_nodes){
        for (const auto& e : outNeighbors.at(u)){
            int v = get<0>(e);
            edge_variables costs = get<1>(e);
            output.emplace_back(u,v,costs);
        }
    }
    return output;
}
int IntGraph::size() const {
    return n;
}
vector<int> IntGraph::get_in_neighbors_of(int v, bool aux) const {
    vector<int> output;
    output.reserve(inNeighbors.at(v).size());
    for (const auto &u: inNeighbors.at(v)){
        if(aux || u.first != 0){
            output.push_back(u.first);
        }
    }
    return output;
}
vector<int> IntGraph::get_out_neighbors_of(int v) const {
    vector<int> output;
    output.reserve(outNeighbors.at(v).size());
    for (const auto &u: outNeighbors.at(v)){
        output.push_back(u.first);
    }
    return output;
}


// Modifiers
vector<int> IntGraph::add_node(int node_count) {
    int i = 1;
    vector<int> output;
    while (node_count != 0){
        if (nodes.find(i) == nodes.end()) {
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
vector<int> IntGraph::add_node_with_storage(const vector<tuple<int, int>>& vertices) {
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
    // TODO: check if duplicate
    if (u == v){
        throw invalid_argument("Self-loop error in add_or_modify_edge.");
    }
    if (nodes.find(u) == nodes.end() || nodes.find(v) == nodes.end()){
        throw invalid_argument("Nodes cannot be found in add_or_modify_edge.");
    }
    make_edge(u,v,costs);
}
void IntGraph::delete_edge(int u, int v) {
    auto cond1 = nodes.find(v) != nodes.end() && inNeighbors[v].find(u) != inNeighbors[v].end();
    auto cond2 = nodes.find(u) != nodes.end() && outNeighbors[u].find(v) != outNeighbors[u].end();
    if (cond1 != cond2) {
        throw logic_error("Error with symmetry for inputs in delete_edge");
    } else if (cond1) {
        inNeighbors[v].erase(u);
        outNeighbors[u].erase(v);
    }
}
void IntGraph::delete_node(int v) {
    if (nodes.find(v) == nodes.end()) {
        throw invalid_argument("Nodes cannot be found in delete_node.");
    }
    for (const auto& u : nodes) {
        delete_edge(u, v);
        delete_edge(v, u);
    }
    inNeighbors.erase(v);
    outNeighbors.erase(v);
    nodes.erase(v);
    n --;
}


