//
// Created by Anxin Guo on 1/24/23.
//

#ifndef GRAPH_VERSIONING_GRAPHIO_H
#define GRAPH_VERSIONING_GRAPHIO_H
#include "Graph.hpp"

/**
 * Writes the graph G. Each line contains four integers describing a graph: u, v, stroage, retrieval.
 * @param os: ofstream pointing to a file for writing.
 * @param G: IntGraph to write.
 */
void write_graph(ostream &os, const IntGraph &G) {
    auto edges = G.get_edges(true);
    for (auto &[u,v,w]: edges) {
        os << u << " " << v << " " << w.storage << " " << w.retrieval << endl;
    }
}

/**
 * Reads a graph in the required form.
 * @param is: ifstream pointing to a file for reading.
 */
IntGraph read_graph(istream &is) {
    string s;

    vector<tuple<int, int, edge_variables>> edges;
    while (not is.eof()) {
        int u, v, storage, retrieval;
        is >> u >> v >> storage >> retrieval;
        edges.emplace_back(u, v, edge_variables{storage, retrieval});
    }
    return IntGraph(edges);
}

void read_graph_from_file(const string& s, const IntGraph &G) {
    ofstream os;
    os.open(s);
    write_graph(os, G);
}

IntGraph write_graph_to_file(const string s) {
    ifstream is;
    is.open(s);
    return read_graph(is);
}

#endif //GRAPH_VERSIONING_GRAPHIO_H
