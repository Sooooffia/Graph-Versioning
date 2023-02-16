//
// Created by Sofia Li on 2023/2/4.
//

#ifndef GRAPH_VERSIONING_TEST_OPT_H
#define GRAPH_VERSIONING_TEST_OPT_H
#include "Graph.hpp"
#include "tests.hpp"
#include "OPT.h"


//void test_prob3_ILP() {
//    IntGraph G(101, 0.1, true);
//    auto M = MST(G);
//    int S = int(M.get_total_storage_cost() * 1.2);
//    IntGraph H = prob3_ILP(G, S);
//    cout << "M's storage and retrieval: " << M.get_total_storage_cost() << " and " << M.get_total_retrieval_cost() << endl;
//    cout << "H's storage and retrieval: " << H.get_total_storage_cost() << " and " << H.get_total_retrieval_cost() << endl;
//}
//
//void test_LMG_with_OPT() {
//    IntGraph G(101, 0.1, true);
//    auto M = MST(G);
//    int S = int(M.get_total_storage_cost() * 1.2);
//    IntGraph H = prob3_ILP(G, S);
//    IntGraph H1 = LMG(G, S);
//    IntGraph H2 = LMG_all(G, S);
//    cout << "MST's storage and retrieval: " << M.get_total_storage_cost() << " and " << M.get_total_retrieval_cost() << endl;
//    cout << "OPT's storage and retrieval: " << H.get_total_storage_cost() << " and " << H.get_total_retrieval_cost() << endl;
//    cout << "LMG's storage and retrieval: " << H1.get_total_storage_cost() << " and " << H1.get_total_retrieval_cost() << endl;
//    cout << "LMG_all's storage and retrieval: " << H2.get_total_storage_cost() << " and " << H2.get_total_retrieval_cost() << endl;
//}

void test_OPT_on_git_grpah(const string &name) {
    string graph_name = "../Experiments/" + name + "-cpp.txt";
    string output_name = "../Experiments/" + name + "-OPT-output.csv";
    ifstream graph_file(graph_name);
    ofstream output_file(output_name);
    if (!graph_file.is_open())
        throw logic_error("failed to open file\n");

    auto G = read_graph(graph_file);
    auto M = MST(G);
    int S_min = M.get_total_storage_cost();

    for (double beta = 1; beta < 2; beta += 0.05) {
        auto opt = prob3_ILP(G, S_min * beta);
        output_file << opt << " ";
        output_file << S_min*beta << endl;
    }
    graph_file.close();
    output_file.close();
}


#endif //GRAPH_VERSIONING_TEST_OPT_H
