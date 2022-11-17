//
// Created by Anxin Guo on 11/15/22.
//

#ifndef DATA_VERSIONING_C_GRAPH_H
#define DATA_VERSIONING_C_GRAPH_H

#include "vector"
#include "unordered_set"
#include "unordered_map"
#include "map"
using namespace std;
/**
 *
 */

//template <typename T>
struct edge_variables{
    int storage = 0;
    int retrieval = 0;
};

class IntGraph {
private:
    // Variables //
    int n = 0, m = 0;
    unordered_set<int> nodes;
    // we will have a set of predecessors and successors instead of edges.
    unordered_map<int, unordered_map<int, edge_variables>> inNeighbors;
    unordered_map<int, unordered_map<int, edge_variables>> outNeighbors;

public:
    // Constructors //
    /// @param
    IntGraph() = default;
    /// @param node_count: number of nodes in the new empty graph.
    explicit IntGraph(int node_count);
    // TODO: add constructor that constructs the graph from lists of nodes and edges.

    // Accessors //
    /// Number of nodes in graph
    int size() const;
    /// Number of edges in graph
    int edge_size() const;
    /// Access in-edge
    unordered_map<int, edge_variables> in_edges(int);
    /// Access out-edge
    unordered_map<int, edge_variables> out_edges(int);
    /// List of all edges

    /// Access neighbors
    unordered_set<int> in_neighbors(int);
    unordered_set<int> out_neighbors(int);

    // Modifiers //
    /**
     * @param node_count: number of nodes to add.
     * @return last added node.
     * Automatically choose
     */
    int add_node(int node_count = 1);
    /**
     * @param node_count: the node to delete.
     */
    void delete_node(int node);

    /**
     * @param u pred
     * @param v succ
     * @param costs costs assigned to this directed edge
     * Adds directed (u,v) to graph. Throw exception if adds repeated edge or self-loop.
     * Costs are 0 by default.
     */
    void add_or_modify_edge(int u, int v, edge_variables costs = {0,0});

    /**
     * @param u pred
     * @param v succ
     * Deletes specified edge
     */
    void delete_edge(int u, int v);
};

/**
 * @param v_size
 * @param e_size
 * @return a random graph
 */
IntGraph generate_random_graph(int v_size, int e_size);


#endif //DATA_VERSIONING_C_GRAPH_H
