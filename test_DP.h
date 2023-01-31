#ifndef GRAPH_VERSIONING_TEST_DP_H
#define GRAPH_VERSIONING_TEST_DP_H

#include "tests.hpp"

void try_on_rg() {
    IntGraph G(100, 0.06);
    auto M = MST(G);
    int S_min = M.get_total_storage_cost();
    IntGraph Arb = MST_with_designated_root(G, 1);

    int ans = DP_arborescence(Arb, 1, 0.001, S_min * 1.2, M.get_total_retrieval_cost());

    cout << ans << endl;
}

#endif //GRAPH_VERSIONING_TEST_DP_H
