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

IntGraph recover_graph(const IntGraph &Arb, const IntGraph &T, const unordered_map<int, int> &change_type) {
    IntGraph ret;
    for (const auto &p : change_type) {
        const int &v = p.first;

        auto out_edges = Arb.get_out_edges_of(v);
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            switch (p.second) {
                case 2:
                    ret.add_or_modify_edge(v, child, T[v][child], true);
                    break;
                case 3:
                    ret.add_or_modify_edge(child, v, T[child][v], true);
                    break;
                case 1:
                    ret.add_or_modify_edge(0, v, T[0][v], true);
                    break;
            }
        } else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            switch (p.second) {
                case 5:
                    ret.add_or_modify_edge(v, child2, T[v][child2], true);
                    break;
                case 6:
                    ret.add_or_modify_edge(v, child1, T[v][child1], true);
                    break;
                case 7:
                    ret.add_or_modify_edge(v, child2, T[v][child2], true);
                    ret.add_or_modify_edge(v, child1, T[v][child1], true);
                    break;
                case 8:
                    ret.add_or_modify_edge(child1, v, T[child1][v], true);
                    break;
                case 9:
                    ret.add_or_modify_edge(child2, v, T[child2][v], true);
                    break;
                case 10:
                    ret.add_or_modify_edge(child1, v, T[child1][v], true);
                    ret.add_or_modify_edge(v, child2, T[v][child2], true);
                    break;
                case 11:
                    ret.add_or_modify_edge(child2, v, T[child2][v], true);
                    ret.add_or_modify_edge(v, child1, T[v][child1], true);
                    break;
                case 4:
                    ret.add_or_modify_edge(0, v, T[0][v], true);
                    break;
            }
        } else if (out_edges.empty()) {
            ret.add_or_modify_edge(0, v, T[0][v], true);
        }
    }
        return ret;
}

void test_DP_on_git_graph(const string &name, double epsilon) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-DP-output.csv";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);

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

    auto M = MST(bidirectional_T);
    long long S_min = M.get_total_storage_cost(), R_of_MST = M.get_total_retrieval_cost();

    auto start_DP = high_resolution_clock::now();
    auto [Arbb,H, ans] = DP_bidirectional_s(bidirectional_T, r, epsilon, S_min * 2, R_of_MST);
    auto end_DP = high_resolution_clock::now();

    cout << "DP finished" << endl;
    IntGraph first_graph = recover_graph(Arbb, H, ans.begin()->second.change_type);
    cout << first_graph.get_edges(true).size() << " " << first_graph.get_nodes(false).size();
    output_file << "time used: " << duration_cast<milliseconds>(end_DP - start_DP).count() << ",first storage:" << first_graph.get_total_storage_cost()
        << ",first retrieval:" << first_graph.get_total_retrieval_cost() << endl;
    for (const auto &p : ans) {//TODO: check the rounding here as well.
        output_file << p.second.storage << "," << p.second.retrieval << "," << p.first << "," << endl;
    }
}


#endif //GRAPH_VERSIONING_TEST_DP_H
