//
// Created by Anxin Guo on 2/8/23.
//

#ifndef GRAPH_VERSIONING_TEST_LMG_ALL_H
#define GRAPH_VERSIONING_TEST_LMG_ALL_H
#include "GraphIO.h"
#include "Graph.hpp"
#include <chrono>
using namespace std::chrono;


void LMG_vs_LMGA_on_git_graph(const string& name) {
    cout << "\nStarting dataset " << name << endl;
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-LMGA-output.csv";
    ifstream graph_file(graph_name);

    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);
    cout << "Finished reading graph" << endl;

//    cout << "G has " << G.size(false) << " nodes and " << G.get_edges(false).size() << " edges" << endl;
//    long long mat_cost=0, sto_cost=0;
//    for (const auto &v : G.get_nodes(false))
//        mat_cost += G[0][v].storage;
//    auto edges = G.get_edges(false);
//    for (const auto &[u,v,w] : edges)
//        sto_cost += w.storage;
//    double mat_cost_avg = mat_cost / G.size(false), sto_cost_avg = sto_cost / edges.size();
//    cout << "Avg materialization cost: " << mat_cost_avg << endl;
//    cout << "Avg storage cost: " << sto_cost_avg << endl;

    auto M = MST(G);
    long long S_min = M.get_total_storage_cost(), R_max = M.get_total_retrieval_cost();
    cout << "MST storage = " << S_min << ", retrieval = " << R_max << endl;

    ofstream output_file(output_name);
    output_file << "storage constraint,LMG,LMGA,LMG time,LMGA time" << endl;
    for (double beta = 1; beta < 2; beta += 0.05) {
        cout << "Testing beta = " << beta << endl;
        long long constraint = beta * S_min;
        auto start_LMG = high_resolution_clock::now();
        auto sol_LMG = LMG(G, constraint);
        auto end_LMG = high_resolution_clock::now();

        auto start_LMGA = high_resolution_clock::now();
        auto sol_LMGA = LMG_all(G, constraint);
        auto end_LMGA = high_resolution_clock::now();

        output_file << constraint << "," << sol_LMG.get_total_retrieval_cost() << "," << sol_LMGA.get_total_retrieval_cost() << ",";
        output_file << duration_cast<milliseconds>(end_LMG-start_LMG).count() << "," << duration_cast<milliseconds>(end_LMGA-start_LMGA).count() << endl;
    }
    graph_file.close();
    output_file.close();
}

#endif //GRAPH_VERSIONING_TEST_LMG_ALL_H
