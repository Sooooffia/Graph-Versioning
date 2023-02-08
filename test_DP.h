#ifndef GRAPH_VERSIONING_TEST_DP_H
#define GRAPH_VERSIONING_TEST_DP_H

#include "tests.hpp"

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

void try_DP_on_rg() {
    IntGraph G(100, 0.2, false);
    auto M = MST(G);
    int S_min = M.get_total_storage_cost();
    IntGraph Arb = MST_with_designated_root(G, 1);

    int ans = DP_arb_modified(Arb, 1, 20.0, S_min * 1.1, M.get_total_retrieval_cost() / 4);
//    int opt = prob3_ILP(Arb, S_min * 1.2).get_total_retrieval_cost();

    cout << ans << " ";
    cout << M.get_total_retrieval_cost() << endl;
}

#endif //GRAPH_VERSIONING_TEST_DP_H
