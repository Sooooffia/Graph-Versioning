#include "Graph.hpp"
#include "LMG.h"
#include <iostream>
#include "OPT.h"
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
    CHECK(G.size(false) == 10)
    CHECK(G.get_in_edges_of(1,1).size() == 1)
    CHECK(G.get_out_neighbors_of(1).empty())
    CHECK(G.get_out_neighbors_of(0).size() == 10)
    CHECK(G.get_in_neighbors_of(0, false).empty())

    vector<tuple<int, int>> node_vector{{1,100}, {2,110}, {3,120}, {4,80}, {5,50}};
    IntGraph::edge_vec edge_vector{{1, 2, {10, 20}}
                                , {2, 3, {20, 10}}
                                , {4, 5, {40, 40}}};
    IntGraph G_prime(node_vector, edge_vector);
    cout << G_prime.get_edges(false);

    IntGraph G_rand(10, 0.5);
    cout << G_rand.get_edges(false);

    // Adding and deleting nodes
    auto v = G.add_node(1);
    CHECK(G.size(false) == 11)
    CHECK(v[0] == 11);
    CHECK(G.get_out_edges_of(0).size() == 11)
    G.delete_node(1);
    CHECK(G.get_out_edges_of(0).size() == 10)
    CHECK(G.size(false) == 10)
    unordered_set<int> S{2,3,4,5,6,7,8,9,10,11};
    CHECK(G.get_nodes(false) == S)

    // Adding and deleting edges
    G.add_or_modify_edge(2, 3, {10, 20});
    G.add_or_modify_edge(2, 3, {30, 40});
    edge_variables weight = {30, 40};
    CHECK(G.get_out_edges_of(2).at(3) == weight)
    G.add_or_modify_edge(10, 3, {10, 20});
    CHECK(G.get_in_edges_of(3, false).size() == 2)
    auto E = G.get_edges(false);
    v = {2, 10};
    CHECK(G.get_in_neighbors_of(3, false) == v)
    G.delete_edge(3, 10);
    CHECK(G.get_in_edges_of(3, false).size() == 2)
    G.delete_edge(10, 3);
    CHECK(G.get_in_edges_of(3, false).size() == 1)
}

void test_LMG() {
//    IntGraph G({{4, 79}, {3, 24}, {2, 50}, {1, 8}},
//               {{2,1,{45, 45}}, {3,4,{41,41}}
//               , {3,2,{22,22}}, {4,3,{4,4}}, {4,1,{66,66}}});
//    cout << "G's edges: \n";
//    for(auto [u,v,w] : G.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    cout << "MST's edges: \n";
//    for(auto [u,v,w] : MST(G).get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H1 = LMG(G, 161); // should be enough to materialize all
//    cout << "H1's edges: \n";
//    for(auto [u,v,w] : H1.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H2 = LMG(G, 95); // should be just enough for MST
//    cout << "H2's edges: \n";
//    for(auto [u,v,w] : H2.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H3 = LMG(G, 123); // should be enough to replace (3,2) with materialization of 2
//    cout << "H3's edges: \n";
//    for(auto [u,v,w] : H3.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H4 = LMG(G, 133); // should be enough to replace (3,4) with materialization of 4
//    cout << "H4's edges: \n";
//    for(auto [u,v,w] : H4.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }

    IntGraph G(1002, 0.2, true);
    auto M = MST(G);
    int S = int(M.get_total_storage_cost() * 1.2);
    auto H = LMG(G, S);
    cout << S << endl << H.get_total_storage_cost() << endl;
}

void test_LMG_all() {
//    IntGraph G({{4, 79}, {3, 24}, {2, 50}, {1, 8}},
//               {{2,1,{45, 45}}, {3,4,{41,41}}
//                       , {3,2,{22,22}}, {4,3,{4,4}}, {4,1,{66,66}}});
//    cout << "G's edges: \n";
//    for(auto [u,v,w] : G.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    cout << "MST's edges: \n";
//    for(auto [u,v,w] : MST(G).get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H1 = LMG_all(G, 161); // should be enough to materialize all
//    cout << "H1's edges: \n";
//    for(auto [u,v,w] : H1.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H2 = LMG_all(G, 95); // should be just enough for MST
//    cout << "H2's edges: \n";
//    for(auto [u,v,w] : H2.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H3 = LMG_all(G, 123); // should be enough to replace (3,2) with materialization of 2
//    cout << "H3's edges: \n";
//    for(auto [u,v,w] : H3.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
//
//    auto H4 = LMG_all(G, 133); // should be enough to replace (3,4) with materialization of 4
//    cout << "H4's edges: \n";
//    for(auto [u,v,w] : H4.get_edges(true)){
//        std::cout << u << ' ' << v << ' ' << w << '\n';
//    }
    IntGraph G(1001, 0.1, true);
    auto M = MST(G);
    int S = int(M.get_total_storage_cost() * 1.2);
    auto H = LMG_all(G, S);
    cout << S << endl << H.get_total_storage_cost();
}

void test_prob3_ILP() {
    IntGraph G(101, 0.1, true);
    auto M = MST(G);
    int S = int(M.get_total_storage_cost() * 1.2);
    IntGraph H = prob3_ILP(G, S);

}