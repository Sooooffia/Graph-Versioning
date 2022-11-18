#include "Graph.hpp"

#define CHECK(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

void test_graph_construction() {
    // Construction graphs and accessing nodes and edges
    IntGraph G(10);
    CHECK(G.size() == 10)
    CHECK(G.get_in_edges_of(1,1).size() == 1)
    CHECK(G.get_out_neighbors_of(1).empty())
    CHECK(G.get_out_neighbors_of(0).size() == 10)
    CHECK(G.get_in_neighbors_of(0).empty())
    vector<int> node_vector{1, 2, 3, 4, 5};
    // TODO: add list of edges

    // Adding and deleting nodes
    auto v = G.add_node(1);
    CHECK(G.size() == 11)
    CHECK(v[0] == 11);
    CHECK(G.get_out_edges_of(0).size() == 11)
    G.delete_node(1);
    CHECK(G.get_out_edges_of(0).size() == 10)
    CHECK(G.size() == 10)
    unordered_set<int> S{2,3,4,5,6,7,8,9,10,11};
    CHECK(G.get_nodes() == S)

    // Adding and deleting edges
    G.add_or_modify_edge(2, 3, {10, 20});
    G.add_or_modify_edge(2, 3, {30, 40});
    edge_variables weight = {30, 40};
    CHECK(G.get_out_edges_of(2).at(3) == weight)
    G.add_or_modify_edge(10, 3, {10, 20});
    CHECK(G.get_in_edges_of(3).size() == 2)
    auto E = G.get_edges();
    v = {2, 10};
    CHECK(G.get_in_neighbors_of(3) == v)
    G.delete_edge(3, 10);
    CHECK(G.get_in_edges_of(3).size() == 2)
    G.delete_edge(10, 3);
    CHECK(G.get_in_edges_of(3).size() == 1)

    // get nodes and get edges
}


