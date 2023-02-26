//
// Created by Sofia Li on 2023/2/17.
//

#ifndef GRAPH_VERSIONING_TEST_PROB_6_H
#define GRAPH_VERSIONING_TEST_PROB_6_H
#include "Prob6.h"
#include "GraphIO.h"

void test_MP_vs_DP_on_git_graph(const string& name, int base_R = 1000) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-prob6-output.csv";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);
    auto M = MST(G);
    cout << M.get_total_storage_cost() << endl;

    int r = 1;
    IntGraph Arb = MST_with_designated_root(G, r);
    IntGraph bidirectional_T;
    for (const auto &[u,v,w] : Arb.get_edges(false)) {
        bidirectional_T.add_or_modify_edge(u, v, G[u][v], true);
        bidirectional_T.add_or_modify_edge(v, u, G[v][u]);
    }
    for (const auto &v : G.get_nodes(false)) {
        bidirectional_T.add_or_modify_edge(0, v, G[0][v], true);
    }

    output_file << "max retrieval constraint,MP solution,MP solution,MP time,DP time,DP solution" << endl;
    for (int theta = 0; theta < 20; theta ++) {
        long long R = base_R * theta;
        cout << "Testing R = " << R << endl;
        auto start_MP = high_resolution_clock::now();
        auto sol_MP = Modified_Prim(G, R);
        auto end_MP = high_resolution_clock::now();

        cout << "testing DP-BMR" << endl;
        auto start_DP = high_resolution_clock::now();
        auto sol_DP = DP_BMR(G, R);
        auto end_DP = high_resolution_clock::now();

        auto MP_sol_value = sol_MP.get_total_storage_cost();
        output_file << R << "," << MP_sol_value << "," << MP_sol_value << ",";
        output_file << duration_cast<milliseconds>(end_MP-start_MP).count() << "," << duration_cast<milliseconds>(end_DP-start_DP).count();
        output_file << "," << sol_DP << endl;
    }
    graph_file.close();
    output_file.close();
}

#endif //GRAPH_VERSIONING_TEST_PROB_6_H
