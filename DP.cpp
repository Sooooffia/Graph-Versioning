#include "DP.h"
using std::map;

/**
 * Builds a binary arborescence from the given arborescence.
 * @param G: an arb.
 * @param r: root of the arb.
 * @return H: IntGraph.
 */
IntGraph make_binary(const IntGraph &G, int r) {//TODO: not tested
    IntGraph H(G);
    int n = H.size(false);
    for (auto v : H.get_nodes(false)) {
        auto out_neighbors = H.get_out_neighbors_of(v);
        while (out_neighbors.size() > 2) { // start removing child of v.
            n++;
            if (v != r) {
                auto pred = H.get_in_neighbors_of(v, false)[0]; // the parent of v
                H.add_or_modify_edge(pred, n, H[pred][v], true);
                H.delete_edge(pred, v);
            }

            auto succ = H.get_out_neighbors_of(v)[0];
            H.add_or_modify_edge(n, v, {0,0}, true);
            H.add_or_modify_edge(0, n, {0,0});

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
    int ind = 0;
    while (ind < stack.size()) {
        desc_count[stack[ind]] = 1;
        auto children = G.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            stack.push_back(child);
        }
        ind++;
    }
    ind = stack.size() - 1;
    while (ind >= 0) {
        auto children = G.get_out_neighbors_of(stack[ind]);
        for (int child : children) {
            desc_count[stack[ind]] += desc_count[child];
        }
        ind--;
    }
    return desc_count;
}

int DP_arborescence(IntGraph &G, int r, double l, int S, int R_of_MST) {
    G = make_binary(G, r);
    // 1. Calculating parameters
    auto nodes = G.get_nodes_in_topo_order(r);
    auto edges = G.get_edges(false);
    int n = nodes.size();

    int T = R_of_MST / l;
    for (auto &[u,v,w]: G.get_edges(false)) {
        w.retrieval /= l;
        w.retrieval += 1;
    }

    // 2. Base cases
    vector<vector<vector<int>>> DP(n+1, vector<vector<int>>(n+1, vector<int>(T, 100000001))); // Too large?
    vector<vector<int>> OPT(n+1, vector<int>(T, 100000001)); // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.
    std::reverse(nodes.begin(), nodes.end());

    // count number of possible descendents
    unordered_map<int, int> desc_count = get_descendants_of_arb(G, r);
    std::cout << "desc_count: " << desc_count << std::endl;
    for (auto v : nodes) {
        if (desc_count[v] == 1) {
            for (int t=0; t < T; t++) {
                DP[v][1][t] = G[0][v].storage;
                OPT[v][t] = G[0][v].storage;
            }
        }
    }

    // 3. Recursion TODO: debug the mat and sto things
    std::cout << "nodes = " << nodes;
    for (auto v : nodes) {
        std::cout << "recursion on node " << v << std::endl;
        auto out_edges = G.get_out_edges_of(v);
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            int sto = out_edges.begin()->second.storage, ret = out_edges.begin()->second.retrieval;
            int matv = G[0][v].storage, mat = G[0][child].storage;

            // k = 1 case
            for (auto t = 0; t < T; t++) {
                DP[v][1][t] = matv + OPT[child][t];
            }
            // k > 1 case
            for (auto k = 2; k <= desc_count[v]; k++) {
                    for (auto t = ret * (k-1); t < T; t++) {
                    DP[v][k][t] = std::min(DP[v][k][t], DP[child][k-1][t - ret * (k-1)] + sto - mat);
                }
            }
        }
        else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            int sto1 = out_edges.begin()->second.storage, ret1 = out_edges.begin()->second.retrieval,
                sto2 = (++out_edges.begin())->second.storage, ret2 = (++out_edges.begin())->second.retrieval;
            int matv = G[0][v].storage, mat1 = G[0][child1].storage, mat2 = G[0][child2].storage;

            // k = 1 case
            for (auto t = 0; t < T; t++) {
                for (auto t1 = 0; t1 <= t; t1++)
                    DP[v][1][t] = std::min(DP[v][1][t], OPT[child1][t1] + OPT[child2][t-t1]);
                DP[v][1][t] += matv;
            }
            // k > 1 case
            for (auto k = 2; k <= desc_count[v]; k++) {
                // only materialize child1
                for (auto t = ret2 * (k-1); t <= T; t++) {
                    for (auto t1 = 0; t1 <= t; t1++) {
                        DP[v][k][t] = std::min(DP[v][k][t],
                                               OPT[child1][t1] + DP[child2][k - 1][t - t1 - ret2 * (k - 1)] + sto2 - mat2 + matv);
                    }
                }
                // only materialize child2
                for (auto t = ret1 * (k-1); t <= T; t++) {
                    for (auto t2 = 0; t2 <= t; t2++) {
                        DP[v][k][t] = std::min(DP[v][k][t],
                                               DP[child1][k - 1][t - t2 - ret1 * (k - 1)] + OPT[child2][t2] + sto1 - mat1 + matv);
                    }
                }
                // materialize both
                // TODO: this is a very costly step. optimize.
                for (auto k1 = 1; k1 < k; k1++) {
                    int k2 = k - 1 - k1, min_ret = std::min(ret1, ret2);
                    for (auto t = min_ret * (k - 1); t <= T; t++) {
                        for (auto t1 = k1 * ret1; t1 <= t - k2 * ret2; t1++) {
                            int t2 = t - t1;
                            DP[v][k][t] = std::min(DP[v][k][t],
                                                   DP[child1][k1][t1] + DP[child2][k2][t2] + sto1 + sto2 - mat1 - mat2 +
                                                   matv);
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
    std::cout << OPT[r] << std::endl;
    for (int t = T; t >= 0; t--) {
        if (OPT[r][t] > S)
            return (t+1) * l;
    }
}

int DP_arb_modified(IntGraph &G, int r, double l, int S, int R_of_MST) {
    G = make_binary(G, r);
    // 1. Calculating parameters
    auto nodes = G.get_nodes_in_topo_order(r);
    auto edges = G.get_edges(false);
    int n = nodes.size();

    int T = R_of_MST / l;
    for (auto &[u,v,w]: G.get_edges(false)) {
        w.retrieval /= l;
        w.retrieval += 1;
    }

    // 2. Base cases
    unordered_map<int, unordered_map<int, unordered_map<int, int>>> DP;
    map<int, map<int, int>> OPT; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.
    std::reverse(nodes.begin(), nodes.end());

    // count number of possible descendents
    unordered_map<int, int> desc_count = get_descendants_of_arb(G, r);
    std::cout << "desc_count: " << desc_count << std::endl;
    for (auto v : nodes) {
        if (desc_count[v] == 1) {
            DP[v][1][0] = G[0][v].storage;
            OPT[v][0] = G[0][v].storage;
        }
    }

    for (auto v : nodes) {
        std::cout << "\nrecursion on node " << v << std::endl;
        auto out_edges = G.get_out_edges_of(v);
        // One child
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            int sto = out_edges.begin()->second.storage, ret = out_edges.begin()->second.retrieval;
            int matv = G[0][v].storage, mat = G[0][child].storage;

            // k = 1 case
            for (auto pt : OPT[child]) {
                int news = pt.second + matv;
                if (news > S)
                    continue;
                DP[v][1][pt.first] = news;
            }

            // k > 1 case
            for (auto pk : DP[child]) {
                for (auto pt : pk.second) {
                    int newk = pk.first+1, newt = pt.first + ret * pk.first, news = sto - mat + pt.second + matv;
                    if (news > S)
                        continue;
                    if (DP[v][newk].find(newt) == DP[v][newk].end()) {
                        DP[v][newk][newt] = INT32_MAX;
                    }
                    DP[v][newk][newt] = std::min(DP[v][newk][newt], news);
                }
            }
        }

        // Two children
        else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            int sto1 = out_edges.begin()->second.storage, ret1 = out_edges.begin()->second.retrieval,
                    sto2 = (++out_edges.begin())->second.storage, ret2 = (++out_edges.begin())->second.retrieval;
            int matv = G[0][v].storage, mat1 = G[0][child1].storage, mat2 = G[0][child2].storage;
            std::cout << "node " << v << " has " << DP[child1].size() << " choices of k1, " << DP[child2].size() << " choices of k2.\n";
            std::cout << OPT[child1].size() << " choices of t1.\n";
            std::cout << OPT[child2].size() << " choices of t2.\n";

            // materialize neither child (k=1)
            for (const auto &p1 : OPT[child1]) {
                for (const auto &p2 : OPT[child2]) {
                    int newt = p1.first + p2.first, news =  p1.second + p2.second + matv;
                    if (news > S)
                        continue;
                    if (DP[v][1].find(newt) == DP[v][1].end())
                        DP[v][1][newt] = INT32_MAX;
                    DP[v][1][newt] = std::min(DP[v][1][newt], news);
                }
            }

            // only materialize child1
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pk2 : DP[child2]) {
                    for (const auto &pt2 : pk2.second) {
                        int newk = pk2.first+1, newt = pt1.first + pt2.first + ret2 * pk2.first;
                        int news = pt1.second + pt2.second + sto2 - mat2 + matv;
                        if (news > S)
                            continue;
                        if (DP[v][newk].find(newt) == DP[v][newk].end())
                            DP[v][newk][newt] = INT32_MAX;
                        DP[v][newk][newt] = std::min(DP[v][newk][newt], news);
                    }
                }
            }

            // only materialize child2
            for (const auto &pt2 : OPT[child2]) {
                for (const auto &pk1 : DP[child1]) {
                    for (const auto &pt1 : pk1.second) {
                        int newk = pk1.first+1, newt = pt2.first + pt1.first + ret1 * pk1.first;
                        int news = pt2.second + pt1.second + sto1 - mat1 + matv;
                        if (news > S)
                            continue;
                        if (DP[v][newk].find(newt) == DP[v][newk].end())
                            DP[v][newk][newt] = INT32_MAX;
                        DP[v][newk][newt] = std::min(DP[v][newk][newt], news);
                    }
                }
            }

            // materialize both
            for (const auto &pk1 : DP[child1]) {
                for (const auto &pt1 : pk1.second) {
                    for (const auto& pk2 : DP[child2]) {
                        for (const auto &pt2 : pk2.second) {
                            int newk = pk1.first + pk2.first + 1, newt = pt1.first + pt2.first + pk1.first*ret1 + pk2.first*ret2;
                            int news = pt2.second + pt1.second + sto1 - mat1 + sto2 - mat2 + matv;
                            if (news > S)
                                continue;
                            auto &thisDP = DP[v][newk];
                            if (thisDP.find(newt) == thisDP.end())
                                thisDP[newt] = INT32_MAX;
                            thisDP[newt] = std::min(thisDP[newt], news);
                        }
                    }
                }
            }
        }

        vector<int> k_list, t_list;
        // calculate OPT
        for (auto &pk : DP[v]) {
            for (auto &pt : pk.second) {
                if (OPT[v].find(pt.first) == OPT[v].end())
                    OPT[v][pt.first] = INT32_MAX;
                OPT[v][pt.first] = std::min(OPT[v][pt.first], pt.second);
            }
        }
    }
    std::cout << OPT[r] << std::endl;
    for (int t = T; t >= 0; t--) {
        if (OPT[r][t] > S)
            return (t+1) * l;
    }
}