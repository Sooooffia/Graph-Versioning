//
// Created by Anxin Guo on 1/24/23.
//

#ifndef GRAPH_VERSIONING_GRAPHIO_H
#define GRAPH_VERSIONING_GRAPHIO_H
#include "Graph.hpp"

/**
 * Writes the graph G in PACE2016/PACE2017 format.
 * @param os: ofstream pointing to a file for writing.
 * @param G: IntGraph to write.
 */
void write_graph(ostream &os, const IntGraph &G) {
    auto edges = G.get_edges(false);
    os << "p tw " << G.size(false) << " " << edges.size() << endl;
    for (auto &[u,v,w]: edges) {
        os << u << " " << v << endl;
    }
}

/**
 * Reads a graph in the required form.
 * @param is: ifstream pointing to a file for reading.
 */
IntGraph read_graph(istream &is) {
    string s;
    vector<tuple<int, int, edge_variables>> edges;
    int u{}, v{}, storage{}, retrieval{};
    while (!is.eof()) {
        is >> u >> v >> storage >> retrieval;
        edges.emplace_back(u, v, edge_variables{storage, retrieval});
    }
    return IntGraph(edges);
}

//void write_graph_to_file(const string& s, const IntGraph &G) {
//    ofstream os;
//    os.open(s);
//    write_graph(os, G);
//}
//
//IntGraph read_graph_from_file(const string s) {
//    ifstream is;
//    is.open(s);
//    return read_graph(is);
//}

void write_graph_to_PACE(const string& name) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../meiji-tw-2017/" + name + "-PACE.gr";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);
    write_graph(output_file, G);
}


#endif //GRAPH_VERSIONING_GRAPHIO_H
