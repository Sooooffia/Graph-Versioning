#ifndef DATA_VERSIONING_C_GRAPH_H
#define DATA_VERSIONING_C_GRAPH_H

#include "vector"
#include "unordered_set"
#include "unordered_map"
#include "map"
#include "MST/gabow.h"
#include "MST/tarjan_pq.h"
#include <iostream>
#include <cmath>
#include <cstring>

using std::unordered_set;
using std::set;
using std::unordered_map;
using std::vector;
using std::tuple;
using std::invalid_argument;
using std::logic_error;
using std::istream;
using std::ostream;
using std::cout;
using std::map;
using std::get;
using std::endl;
using std::pair;
using std::priority_queue;
using std::string;

struct edge_variables{
    long storage = 0;
    long retrieval = 0;
};

inline bool operator==(const edge_variables& a, const edge_variables& b) {
    return a.storage == b.storage && a.retrieval == b.retrieval;
}
ostream& operator<<(ostream& os, const edge_variables& w);

class IntGraph {
private:
    // Variables //
    int n = 0; ///< @brief Number of nodes (not counting the auxiliary root)
    //TODO: add this m to every constructor and methods
    int m = 0; ///< @brief Number of edges (not counting the auxiliary edges)
    unordered_set<int> nodes;
    /*
     * we will have a set of predecessors and successors instead of edges.
     */
    unordered_map<int, unordered_map<int, edge_variables>> inNeighbors;
    unordered_map<int, unordered_map<int, edge_variables>> outNeighbors;

    // Helpers //
    /**
     * Helper function that makes a node. Should make sure that such node doesn't exist before using.
     */
    void make_node(int v);
    void make_edge(int u, int v, const edge_variables& costs);

public:
    using edge_vec = vector<tuple<int, int, edge_variables>>;
    // Constructors //
    /// @brief Default constructor. Emtpy graph. (only auxiliary node 0.)
    IntGraph();
    /**
     * @brief Initialize empty graph with specified node number 1-n.
     * @param node_count: number of nodes in the new empty graph.
     * @Detail All nodes storage cost initialized as 0.
     */
    explicit IntGraph(int node_count);
    /**
     * @brief Initialize graph with specified nodes and edges.
     * @param nodes: a vector of <int,int>: node, storage. Cannot contain auxiliary root.
     * @param edges: a vector of edges in tuple<int,int,edge_var>: predecessor, successor, weight struct. Cannot contain auxiliary root.
     */
    IntGraph(const vector<tuple<int,int>>& nodes, const vector<tuple<int, int, edge_variables>>& edges);
    /**
     * @brief Initialize graph with specified edges.
     * @param edges: a vector of edges in tuple<int,int,edge_var>: predecessor, successor, weight struct. Contains auxiliary root.
     */
    explicit IntGraph(const vector<tuple<int, int, edge_variables>>& edges);
    /**
     * @brief Initialize random graph.
     * @param node_count: int, number of nodes.
     * @param p: (0,1), independent probability of each edge being created.
     * @param equal_weights: bool, whether the two costs are equal.
     * @Detail Directed. Both costs of the edge are the same integer random variable from Unif[1,100].
     */
     // TODO: add a string indicator for different types of graphs
     IntGraph(int node_count, float p, bool equal_weights = true);

    /**
     * @brief Initialize subgraph of graph by specifying the .
     * @param node_count: int, number of nodes.
     * @param p: (0,1), independent probability of each edge being created.
     * @param equal_weights: bool, whether the two costs are equal.
     * @Detail Directed. Both costs of the edge are the same integer random variable from Unif[1,100].
     */


    // Accessors //
    [[nodiscard]] int size(bool aux) const;///< @return Number of nodes in graph.
    [[nodiscard]] unordered_set<int> get_nodes(bool aux) const;///< @return A vector of all nodes. aux=true if wants 0.
    [[nodiscard]] vector<tuple<int, int>> get_nodes_and_storage(bool aux) const;///< @return a vector of tuple<int, int>, node number and storage cost.
    [[nodiscard]] unordered_map<int, edge_variables> get_in_edges_of(int v, bool aux) const;///< @param v: node. @return A map u -> weight(u,v), for all existing (u,v).
    [[nodiscard]] const unordered_map<int, edge_variables> &get_out_edges_of(int) const;///< @param u: node. @return A map v -> weight(u,v), for all existing (u,v).
    [[nodiscard]] vector<tuple<int,int,edge_variables>> get_edges(bool aux) const;///< @return A vector of all edges, aux: whether to include auxilary root
    [[nodiscard]] vector<int> get_in_neighbors_of(int, bool aux) const;///< @return A vector of predecessors
    [[nodiscard]] vector<int> get_out_neighbors_of(int) const;///< @return A vector of successors
    unordered_map<int, edge_variables> operator[](int) const;
    [[nodiscard]] long long get_total_storage_cost() const;
    [[nodiscard]] long long get_max_storage_cost() const;
    [[nodiscard]] long long get_total_retrieval_cost() const;
    [[nodiscard]] long long get_max_retrieval_cost() const;
    [[nodiscard]] vector<int> get_nodes_in_bfs_order(int r) const;///< Not including 0. Assuming there is no cycle!
    /**
     * @note The graph must be an arborescence rooted at 0!
     * @param dependency_count: int -> int. dependency_count[v] is the number of dependent nodes of v in H, including v itself.
     * @param retrieval_cost: int -> int. The retrieval cost of each v in H.
     */
    void get_dependency_count_and_retrieval_cost(unordered_map<int, int>& dependency_count,
                                  unordered_map<int, long long>& retrieval_cost) const;
    /**
     * @note The graph must be an arborescence rooted at 0!
     * @param dependency_count: int -> set<int>. dependency_count[v] is the set of dependent nodes of v in H, including v itself.
     * @param retrieval_cost: int -> int. The retrieval cost of each v in H.
     */
    void get_dependency_list_and_retrieval_cost(unordered_map<int, unordered_set<int>>& dependency_list,
                                      unordered_map<int, long long>& retrieval_cost) const;
    bool is_valid_solution() const;
    bool is_arborescence(int r) const;

    // Modifiers //
    /**
     * @param node_count: number of nodes to add.
     * @return a list of added nodes.
     * Automatically choose node names to add; storage cost set to 0 as default.
     * Add a single node by default.
     */
    vector<int> add_node(int node_count = 1);
    /**
     * @param vertex: a list of pair of nodes and respective storage costs.
     */
    vector<int> add_node_with_storage(const vector<tuple<int,int>>& vertex);
    /**
     * @param node_count: the node to delete.
     */
    void delete_node(int);

    /**
     * @param u pred
     * @param v succ
     * @param costs costs assigned to this directed edge
     * @param new_node marks whether we can potentially create new nodes with this call.
     * Adds directed (u,v) to graph. Throw exception if adds repeated edge or self-loop.
     * Costs are 0 by default.
     */
    void add_or_modify_edge(int u, int v, edge_variables costs = {0,0}, bool new_node=false);

    /**
     * @param u pred
     * @param v succ
     * Deletes specified edge
     */
    void delete_edge(int u, int v);
};

/**
 * @param G: a connected IntGraph.
 * @returns A minimum spanning arborescence of G.
 */
IntGraph MST(const IntGraph& G);

/**
 * @param G: an IntGraph where r can reach all nodes except 0.
 * @param r: the root to calculate MST from.
 * @return IntGraph H, where H\{0} is a minimum spanning arborescence of G\{0}, rooted at r.
 * This should be useful for heuristics. (extracting arborescence from
 */
IntGraph MST_with_designated_root(const IntGraph &G, int r);

/**
 * @param G : directed graph.
 * @return A bidirectional tree T. The edges of T form a MST for the undirected graph G', where G'[u][v] = min{G[u][v], G[v][u]}
 */
IntGraph Symmetric_MST_with_min_weight(const IntGraph &G, int r);

// Outputting a vector like python list
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& V)
{
    os << '[';
    for (auto it = V.begin(); it != V.end(); ) {
        os << *it;
        if (++it != V.end())
            os << ", ";
    }
    os << "]";
    return os;
}
// Outputting an unordered set like python set
template <typename T>
ostream& operator<<(ostream& os, const unordered_set<T>& V)
{
    os << '{';
    for (auto it = V.begin(); it != V.end();) {
        os << *it;
        if (++it != V.end())
            os << ", ";
    }
    os << "}";
    return os;
}
// Outputting an unordered map like python dict
template <typename S, typename T>
ostream& operator<<(ostream& os, const unordered_map<S, T>& V)
{
    os << '{';
    for (auto it = V.begin(); it != V.end();) {
        os << it->first << ": " << it->second;
        if (++it != V.end())
            os << ", ";
    }
    os << "}";
    return os;
}
// Outputting a map like python dict
template <typename S, typename T>
ostream& operator<<(ostream& os, const std::map<S, T>& V)
{
    os << '{';
    for (auto it = V.begin(); it != V.end();) {
        os << it->first << ": " << it->second;
        if (++it != V.end())
            os << ", ";
    }
    os << "}";
    return os;
}
// Outputting a tuple like python tuple
template<class Tuple, size_t N>
struct TuplePrinter {
    static void print(ostream& os, const Tuple& t)
    {
        TuplePrinter<Tuple, N-1>::print(os, t);
        os << ", " << get<N-1>(t);
    }
};
template<class Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(ostream& os, const Tuple& t)
    {
        os << get<0>(t);
    }
};
template<class... Args>
ostream& operator<<(ostream& os, const std::tuple<Args...>& t)
{
    os << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(os, t);
    os << ")";
    return os;
}

#endif //DATA_VERSIONING_C_GRAPH_H
