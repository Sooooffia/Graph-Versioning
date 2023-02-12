//
// Created by Anxin Guo on 2/8/23.
//

#ifndef GRAPH_VERSIONING_SCRIPT_LMG_ALL_H
#define GRAPH_VERSIONING_SCRIPT_LMG_ALL_H
#include <cstring>
#include "GraphIO.h"
#include "Graph.hpp"
#include <chrono>

using std::string;
using namespace std::chrono;

void LMG_vs_LMGA_on_git_graph(string name) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-output.csv";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

//    graph_file.open(graph_name);
//    output_file.open(output_name);
    auto G = read_graph(graph_file);
    cout << G.is_legit() << endl;
    auto M = MST(G);
    int S_min = M.get_total_storage_cost();

    for (double beta = 1; beta < 2; beta += 0.05) {
        auto start_LMG = high_resolution_clock::now();
        auto sol_LMG = LMG(G, S_min*beta);
        auto end_LMG = high_resolution_clock::now();

        auto start_LMGA = high_resolution_clock::now();
        auto sol_LMGA = LMG_all(G, S_min*beta);
        auto end_LMGA = high_resolution_clock::now();

        output_file << sol_LMG.get_total_retrieval_cost() << " " << sol_LMGA.get_total_retrieval_cost() << " ";
        output_file << duration_cast<microseconds>(end_LMG-start_LMG).count() << " " << duration_cast<microseconds>(end_LMGA-start_LMGA).count();
        output_file << " " << S_min*beta << endl;
    }
    graph_file.close();
    output_file.close();
}

#endif //GRAPH_VERSIONING_SCRIPT_LMG_ALL_H
