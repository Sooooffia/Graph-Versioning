#ifndef GRAPH_VERSIONING_TEST_DP_H
#define GRAPH_VERSIONING_TEST_DP_H

#include "tests.hpp"

void try_DP_on_rg() {
    IntGraph G(100, 0.2, false);
    auto M = MST(G);
    int S_min = M.get_total_storage_cost();
    IntGraph Arb = MST_with_designated_root(G, 1);

    int ans = DP_arborescence(Arb, 1, 300, S_min * 1.2, M.get_total_retrieval_cost());
//    int opt = prob3_ILP(Arb, S_min * 1.2).get_total_retrieval_cost();

    cout << ans << " ";
//    cout << opt << endl;
    cout << M.get_total_retrieval_cost() << endl;
}

#endif //GRAPH_VERSIONING_TEST_DP_H
