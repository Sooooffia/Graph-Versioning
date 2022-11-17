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
    /**
     * n := number of nodes ** not counting the auxiliary root **
     * m := number of edges ** not counting edges from the auxiliary root **
     */
    int n = 0;
    unordered_set<int> nodes;
    /**
     * we will have a set of predecessors and successors instead of edges.
     */
    unordered_map<int, unordered_map<int, edge_variables>> inNeighbors;
    unordered_map<int, unordered_map<int, edge_variables>> outNeighbors;

    /**
     * Helper function that takes a node and makes it
     */
    void make_node(int v);
    void make_edge(int u, int v, edge_variables costs);

public:
    // Constructors //
    IntGraph();
    /**
     * @param node_count: number of nodes in the new empty graph.
     * all nodes storage cost initialized as 0.
     */
    explicit IntGraph(int node_count);
    /**
     * @param nodes: a list of nodes in a vector.
     * @param edges: a list of edges in tuple: predecessor, successor, weight struct.
     * construct an intGraph from lists of nodes and edges
     */
    IntGraph(const vector<tuple<int,int>>& vertices, const vector<tuple<int, int, edge_variables>>& edges);

    // Accessors //
    /// Number of nodes in graph
    int get_nodes_size() const;
    /// List of all nodes
    unordered_set<int> get_nodes();
    /// Access in-edge
    unordered_map<int, edge_variables> get_in_edges_of(int);
    /// Access out-edge
    unordered_map<int, edge_variables> get_out_edges_of(int);
    /// List of all edges
    vector<tuple<int,int,edge_variables>> get_edges();
    /// Access neighbors
    vector<int> get_in_neighbors_of(int);
    vector<int> get_out_neighbors_of(int);

    // Modifiers //
    /**
     * @param node_count: number of nodes to add.
     * @return a list of added nodes.
     * Automatically choose node names to add; storage cost set to 0 as default.
     */
    vector<int> add_node(int node_count = 1);
    /**
     * @param
     */
    vector<int> add_note_with_storage(vector<tuple<int,int>>);
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
