#include "Graph.hpp"
#include <iostream>
#define CHECK(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }
using namespace std;

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
    os << "]" << std::endl;
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
    os << "}" << std::endl;
    return os;
}
// Outputting an unordered set like python dict
template <typename S, typename T>
ostream& operator<<(ostream& os, const unordered_map<S, T>& V)
{
    os << '{';
    for (auto it = V.begin(); it != V.end();) {
        os << it->first << ": " << it->second;
        if (++it != V.end())
            os << ", ";
    }
    os << "}" << std::endl;
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

void test_graph_construction() {
    // Construction graphs and accessing nodes and edges
    IntGraph G(10);
    CHECK(G.size() == 10)
    CHECK(G.get_in_edges_of(1,1).size() == 1)
    CHECK(G.get_out_neighbors_of(1).empty())
    CHECK(G.get_out_neighbors_of(0).size() == 10)
    CHECK(G.get_in_neighbors_of(0).empty())

    vector<tuple<int, int>> node_vector{{1,100}, {2,110}, {3,120}, {4,80}, {5,50}};
    IntGraph::edge_vec edge_vector{{1, 2, {10, 20}}
                                , {2, 3, {20, 10}}
                                , {4, 5, {40, 40}}};
    IntGraph G_prime(node_vector, edge_vector);
    cout << G_prime.get_edges();

    IntGraph G_rand(10, 0.5);
    cout << G_rand.get_edges();

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


