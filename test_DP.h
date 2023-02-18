#ifndef GRAPH_VERSIONING_TEST_DP_H
#define GRAPH_VERSIONING_TEST_DP_H

#include "tests.hpp"
#include "GraphIO.h"
#include <chrono>
using namespace std::chrono;

void test_make_binary() {
    IntGraph G(5);
    for (int i = 1; i <=5; i++)
        G.add_or_modify_edge(0, i, {i, i*10});
    G.add_or_modify_edge(1, 2, {12, 21});
    G.add_or_modify_edge(1,3, {13, 31});
    G.add_or_modify_edge(1,4, {14, 31});
    G.add_or_modify_edge(4,5, {45, 54});
    auto H = make_binary(G, 1);
    cout << H.get_edges(true);
}

//void try_DP_on_rg() {
//    IntGraph G(100, 0.2, false);
//    auto M = MST(G);
//    int S_min = M.get_total_storage_cost();
//    IntGraph Arb = MST_with_designated_root(G, 1);
//
//    int ans = DP_arborescence(Arb, 1, 20.0, S_min * 1.1, M.get_total_retrieval_cost() / 4);
//    int opt = prob3_ILP(Arb, S_min * 1.2).get_total_retrieval_cost();
//
//    cout << ans << " ";
//    cout << M.get_total_retrieval_cost() << endl;
//}

//void test_DP_on_git_graph(const string &name, double epsilon) {
//    string graph_name = "../Experiments/" + name + "-cpp.txt";
//    string output_name = "../Experiments/" + name + "-DP-output.csv";
//    ifstream graph_file(graph_name);
//    ofstream output_file(output_name);
//    if (!graph_file.is_open())
//        throw logic_error("failed to open file\n");
//
//    auto G = read_graph(graph_file);
//    auto M = MST(G);
//    int S_min = M.get_total_storage_cost(), R_of_MST = M.get_total_retrieval_cost();
//    int r = G.size(false);
//    IntGraph Arb = MST_with_designated_root(G, r);
//
//    auto ans = DP_arb_modified(Arb, r, epsilon, S_min * 2, R_of_MST);
//    for (const auto &p : ans) {//TODO: check the rounding here as well.
//        output_file << p.second.storage << " " << p.second.retrieval << " " << p.first << endl;
//    }
//}

void test_DP_on_git_graph(const string &name, double epsilon) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-DP-output.csv";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);
    auto M = MST(G);
    long long S_min = M.get_total_storage_cost(), R_of_MST = M.get_total_retrieval_cost();

    int r = 1;
    IntGraph Arb = MST_with_designated_root(G, r);
    IntGraph bidirectional_T;
    for (const auto &[u,v,w] : Arb.get_edges(false)) {
        bidirectional_T.add_or_modify_edge(u, v, w, true);
        bidirectional_T.add_or_modify_edge(v, u, G[v][u], true);
    }
    for (const auto &v : G.get_nodes(false)) {
        bidirectional_T.add_or_modify_edge(0, v, G[0][v], true);
    }

    auto start_DP = high_resolution_clock::now();
    auto ans = DP_bidirectional(bidirectional_T, r, epsilon, S_min * 2, R_of_MST);
    auto end_DP = high_resolution_clock::now();

    for (const auto &p : ans) {//TODO: check the rounding here as well.
        output_file << p.second.storage << "," << p.second.retrieval << "," << p.first << "," << endl;
    }
    output_file << "time used: " << duration_cast<milliseconds>(end_DP - start_DP).count() << endl;
}


#endif //GRAPH_VERSIONING_TEST_DP_H
