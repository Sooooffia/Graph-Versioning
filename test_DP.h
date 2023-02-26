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

void recover_graph(IntGraph &ansG, const IntGraph &T, connection_type con) {
    const int &v = con.v, &child1 = con.child1, &child2 = con.child2;
    switch (con.val) {
        case 2:
            ansG.add_or_modify_edge(v, child1, T[v][child1], true);
            break;
        case 3:
            ansG.add_or_modify_edge(child1, v, T[child1][v], true);
            break;
        case 5:
            ansG.add_or_modify_edge(v, child2, T[v][child2], true);
            break;
        case 6:
            ansG.add_or_modify_edge(v, child1, T[v][child1], true);
            break;
        case 7:
            ansG.add_or_modify_edge(v, child2, T[v][child2], true);
            ansG.add_or_modify_edge(v, child1, T[v][child1], true);
            break;
        case 8:
            ansG.add_or_modify_edge(child1, v, T[child1][v], true);
            break;
        case 9:
            ansG.add_or_modify_edge(child2, v, T[child2][v], true);
            break;
        case 10:
            ansG.add_or_modify_edge(child1, v, T[child1][v], true);
            ansG.add_or_modify_edge(v, child2, T[v][child2], true);
            break;
        case 11:
            ansG.add_or_modify_edge(child2, v, T[child2][v], true);
            ansG.add_or_modify_edge(v, child1, T[v][child1], true);
            break;
    }
    const auto nodes = ansG.get_nodes(false);
    if (nodes.find(v) == nodes.end() or ansG.get_in_edges_of(v, true).empty()) {
        ansG.add_or_modify_edge(0, v, T[0][v], true);
    }
    if (child1 > 0)
        recover_graph(ansG, T, *(con.con1));
    if (child2 > 0)
        recover_graph(ansG, T, *(con.con2));
}

void test_DP_on_git_graph(const string &name, double epsilon, bool use_storage=false) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-DP-output.csv";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);
    cout << G.size(false) << " " << G.get_edges(false).size() << endl;

    auto start_DP = high_resolution_clock::now();
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
    cout << bidirectional_T.size(false) << " " << bidirectional_T.get_edges(false).size() << endl;

    auto M = MST(bidirectional_T);
    long long S_min = M.get_total_storage_cost(), R_of_MST = M.get_total_retrieval_cost();

    if (use_storage) {
        auto [Arbb, H, OPT, OPT_k, DP] = DP_bidirectional_s(bidirectional_T, r, epsilon, S_min * 2, R_of_MST);
        auto ans = OPT[r];
        auto end_DP = high_resolution_clock::now();
        cout << "DP finished" << endl;

//        IntGraph first_graph;
//        recover_graph(first_graph, H, ans.begin()->second.con); //simulate recovering graph
//        cout << first_graph.get_edges(true).size() << " " << first_graph.get_nodes(false).size();

        output_file << ",,,time used: " << duration_cast<milliseconds>(end_DP - start_DP).count() << endl;
        for (const auto &p: ans) {
            output_file << p.second.storage << "," << p.second.retrieval << "," << p.first << "," << endl;
        }
    }
    else {
        auto [Arbb, H, OPT, DP] = DP_bidirectional(bidirectional_T, r, epsilon, S_min * 2, R_of_MST);
        auto ans = OPT[r];
        auto end_DP = high_resolution_clock::now();

        IntGraph first_graph;
        recover_graph(first_graph, H, ans.begin()->second.con); //simulate recovering graph
//        cout << first_graph.get_edges(true).size() << " " << first_graph.get_nodes(false).size();

        output_file << ",,,time used: " << duration_cast<milliseconds>(end_DP - start_DP).count() << endl;
        for (const auto &p: ans) {
            output_file << p.second.storage << "," << p.second.retrieval << "," << p.first << "," << endl;
        }
    }




}


#endif //GRAPH_VERSIONING_TEST_DP_H
