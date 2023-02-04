#include "DP.h"

/**
 * Builds a binary arborescence from the given arborescence.
 * @param G: an arb.
 * @param r: root of the arb.
 * @return H: IntGraph.
 */
IntGraph make_binary(const IntGraph &G, int r) {//TODO: not tested
    IntGraph H(G);
    int n = G.size(false);
    for (auto v : G.get_nodes(false)) {
        auto out_neighbors = H.get_out_neighbors_of(v);
        while (out_neighbors.size() > 2) { // start removing child of v.
            n++;
            if (v != r) {
                auto pred = H.get_in_neighbors_of(v, false)[0]; // the parent of v
                H.add_or_modify_edge(pred, n, H[pred][v], true);
                H.delete_edge(pred, v);
            }

            auto succ = H.get_out_neighbors_of(v)[0];
            H.add_or_modify_edge(n, v, {0,0});
            H.add_or_modify_edge(0, n, H[0][v]);

            H.add_or_modify_edge(n, succ, H[v][succ]);
            H.delete_edge(v, succ);
            out_neighbors = H.get_out_neighbors_of(v);
        }
    }
    return H;
}

unordered_map<int, int> get_descendants_of_arb(const IntGraph &G, int r) {
    unordered_map<int, int> desc_count;
    vector<int> stack{r};
    size_t ind = 0;
    while (ind < stack.size()) {
        auto children = G.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            stack.push_back(child);
        }
        ind++;
    }
    ind = stack.size() - 1;
    while (ind > 0) {
        auto &desc = desc_count[stack[ind]];
        auto children = G.get_out_neighbors_of(stack[ind]);
        for (int child : children) {
            desc += desc_count[child];
        }
        ind--;
    }
    return desc_count;
}

int DP_arborescence(IntGraph &G, int r, double epsilon, int S, int R_max) {
    G = make_binary(G, r);
    // 1. Calculating parameters
    auto nodes = G.get_nodes_in_topo_order(false);
    auto edges = G.get_edges(false);
    int n = nodes.size();

    int T = std::min(R_max, int(1/epsilon));
    double l = R_max / T;
    for (auto &[u,v,w]: G.get_edges(false)) {
        w.retrieval /= l;
    }

    // 2. Base cases
    vector<vector<vector<int>>> DP(n+1, vector<vector<int>>(n+1, vector<int>(T, INT32_MAX))); // Too large?
    int OPT[n+1][T]; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.
    std::reverse(nodes.begin(), nodes.end());
    for (auto v : nodes) {
        if (G.get_out_neighbors_of(v).empty()) {
            for (int t=0; t < T; t++) {
                DP[v][1][t] = 0;
                OPT[v][t] = 0;
            }
        }
    }
    // count number of possible descendents
    unordered_map<int, int> desc_count = get_descendants_of_arb(G, r);

    // 3. Recursion
    for (auto v : nodes) {
        auto out_edges = G.get_out_edges_of(v);
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            int sto = out_edges.begin()->second.storage, ret = out_edges.begin()->second.retrieval;
            int mat = G[0][child].storage;

            // k = 1 case
            for (auto t = 0; t < T; t++) {
                DP[v][1][t] = mat + OPT[child][t];
            }
            // k > 1 case
            for (auto k = 2; k <= desc_count[v]; k++) {
                    for (auto t = ret * (k-1); t < T; t++) {
                    DP[v][k][t] = std::min(DP[v][k][t], DP[child][k-1][t - ret * (k-1)] + sto);//TODO: optimize ret * k and 3d arrays
                }
            }
        }
        else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            int sto1 = out_edges.begin()->second.storage, ret1 = out_edges.begin()->second.retrieval,
                sto2 = (++out_edges.begin())->second.storage, ret2 = (++out_edges.begin())->second.retrieval;
            int mat1 = G[0][child1].storage, mat2 = G[0][child2].storage;

            // k = 1 case
            for (auto t = 0; t < T; t++) {
                for (auto t1 = 0; t1 <= t; t1++)
                    DP[v][1][t] = std::min(DP[v][1][t], OPT[child1][t1] + OPT[child2][t-t1]);
            }
            // k > 1 case
            for (auto k = 2; k <= desc_count[v]; k++) {
                // only materialize child1
                for (auto t = ret2 * (k-1); t <= T; t++) {
                    for (auto t1 = 0; t1 <= t; t1++) {
                        DP[v][k][t] = std::min(DP[v][k][t],
                                               OPT[child1][t1] + DP[child2][k - 1][t - t1 - ret2 * (k - 1)] + sto2);
                    }
                }
                // only materialize child2
                for (auto t = ret1 * (k-1); t <= T; t++) {
                    for (auto t2 = 0; t2 <= t; t2++) {
                        DP[v][k][t] = std::min(DP[v][k][t],
                                               DP[child1][k - 1][t - t2 - ret1 * (k - 1)] + OPT[child2][t2] + sto1);
                    }
                }
                // materialize both
                // TODO: this is a very costly step. optimize.
                for (auto k1 = 1; k1 < k; k1++) {
                    int k2 = k-1-k1, min_ret = std::min(ret1, ret2);
                    for (auto t = min_ret*(k-1); t <= T; t++) {
                        for (auto t1 = k1 * ret1; t1 <= t - k2 * ret2; t1++) {
                            int t2 = t - t1;
                            DP[v][k][t] = std::min(DP[v][k][t], DP[child1][k1][t1] + DP[child2][k2][t2] + sto1 + sto2);
                        }
                    }
                }
            }
        }

        for (auto t = 0; t <= T; t++) {
            for (auto k = 1; k <= desc_count[v]; k++)
                OPT[v][t] = std::min(OPT[v][t], DP[v][k][t]);
        }
    }
    for (int t = T; t >= 0; t--) {
        if (OPT[r][t] > S)
            return t+1;
    }
}