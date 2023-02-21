#include "DP.h"

/**
 * Builds a binary arborescence from the given arborescence.
 * @param G: an arb.
 * @param r: root of the arb.
 * @return H: IntGraph.
 */
IntGraph make_binary(const IntGraph &G, int r) {//TODO: not tested
    IntGraph H(G);
    int n = H.size(false);
    for (const auto &v : H.get_nodes(false)) {
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
            H.add_or_modify_edge(0, n, G[0][v]);

            H.add_or_modify_edge(n, succ, H[v][succ]);
            H.delete_edge(v, succ);
            out_neighbors = H.get_out_neighbors_of(v);
        }
    }
    return H;
}


double log_base_a_of_b(double a, double b) {
    return log2(b) / log2(a);
}

int geometrically_discretize(long long val, double epsilon) {
    return int(log_base_a_of_b(1+epsilon, val-1) + 2);
}

void update_DP(unordered_map<int, edge_variables> &t_map, int t, const edge_variables &new_val) {
    if (t_map.find(t) == t_map.end())
        t_map[t] = {INT32_MAX, INT32_MAX};

    if (t_map[t].storage > new_val.storage)
        t_map[t] = new_val;
}

void update_DP(unordered_map<int, DP_type> &t_map, int t, const DP_type &new_val) {
    if (new_val.storage < 0 or new_val.retrieval < 0)
        throw logic_error("Aghhhhhhh");
    if (t_map.find(t) == t_map.end())
        t_map[t] = {INT64_MAX, INT64_MAX};

    if (t_map[t].storage > new_val.storage)
        t_map[t] = new_val;
}


void update_OPT(map<int, edge_variables, std::greater<>> &t_map, int t, const edge_variables &new_val) {
    if (t_map.find(t) == t_map.end())
        t_map[t] = {INT32_MAX, INT32_MAX};

    if (t_map[t].storage > new_val.storage)
        t_map[t] = new_val;
}

void update_OPT(map<int, DP_type, std::greater<>> &t_map, int t, const DP_type &new_val) {
    if (t_map.find(t) == t_map.end())
        t_map[t] = {INT64_MAX, INT64_MAX};

    if (t_map[t].storage > new_val.storage)
        t_map[t] = new_val;
}

// TODO: geometric descretization doesn't add up, so we have to store the REAL retrieval cost as well.
map<int, edge_variables, std::greater<>> DP_arb_modified(const IntGraph &G, int r, double epsilon, int S, int R_of_MST) {
    IntGraph H = make_binary(G, r);

    // 1. Calculating parameters
    auto nodes = H.get_nodes_in_bfs_order(r);
    std::reverse(nodes.begin(), nodes.end());
    auto edges = H.get_edges(false);

    // 2. Base cases
    unordered_map<int, unordered_map<int, unordered_map<int, edge_variables>>> DP; ///<now we are storing DP[v][k][t] a pair (s,r) of the subsolution.
    map<int, map<int, edge_variables, std::greater<>>> OPT; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.

    // count number of possible descendents
    for (const auto &v : nodes) {
        if (H.get_out_neighbors_of(v).empty()) {
            DP[v][1][0] = OPT[v][0] = {H[0][v].storage, 0};
        }
    }

    for (const auto &v : nodes) {
        cout << "\nrecursion on node " << v << std::endl;
        auto out_edges = H.get_out_edges_of(v);
        // One child
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            int sto = out_edges.begin()->second.storage, ret = out_edges.begin()->second.retrieval;
            int matv = H[0][v].storage, mat = H[0][child].storage;

            // k = 1 case
            for (const auto &pt : OPT[child]) {
                int new_s = pt.second.storage + matv;
                if (new_s > S)
                    continue;
                update_DP(DP[v][1], pt.first, {new_s, pt.second.retrieval});
            }

            // k > 1 case
            for (const auto &pk : DP[child]) {
                for (const auto &pt : pk.second) {
                    edge_variables new_pair = {sto - mat + pt.second.storage + matv, pt.second.retrieval + ret * pk.first};
                    int new_k = pk.first + 1, new_t = geometrically_discretize(new_pair.retrieval, epsilon);
                    if (new_pair.storage > S)
                        continue;
                    update_DP(DP[v][new_k], new_t, new_pair);
                }
            }
        }

        // Two children
        else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            int sto1 = out_edges.begin()->second.storage, ret1 = out_edges.begin()->second.retrieval,
                    sto2 = (++out_edges.begin())->second.storage, ret2 = (++out_edges.begin())->second.retrieval;
            int matv = H[0][v].storage, mat1 = H[0][child1].storage, mat2 = H[0][child2].storage;
            cout << "node " << v << " has " << DP[child1].size() << " choices of k1, " << DP[child2].size() << " choices of k2.\n";
            cout << OPT[child1].size() << " choices of t1." << endl;
            cout << OPT[child2].size() << " choices of t2." << endl;

            // materialize both child (k=1)
            for (const auto &p1 : OPT[child1]) {
                for (const auto &p2 : OPT[child2]) {
                    edge_variables new_pair = {p1.second.storage + p2.second.storage + matv, p1.second.retrieval + p1.second.retrieval};
                    int new_t = geometrically_discretize(new_pair.retrieval, epsilon);
                    if (new_pair.storage > S)
                        continue;
                    update_DP(DP[v][1], new_t, new_pair);
                }
            }

            // only materialize child2
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pk2 : DP[child2]) {
                    for (const auto &pt2 : pk2.second) {
                        edge_variables new_pair = {pt1.second.storage + pt2.second.storage + sto2 - mat2 + matv,
                                                   pt1.second.retrieval + pt2.second.retrieval + ret2 * pk2.first};
                        int new_t = geometrically_discretize(new_pair.retrieval, epsilon), new_k = pk2.first + 1;
                        if (new_pair.storage > S)
                            continue;
                        update_DP(DP[v][new_k], new_t, new_pair);
                    }
                }
            }

            // only materialize child1
            for (const auto &pt2 : OPT[child2]) {
                for (const auto &pk1 : DP[child1]) {
                    for (const auto &pt1 : pk1.second) {
                        edge_variables new_pair = {pt2.second.storage + pt1.second.storage + sto1 - mat1 + matv,
                                                   pt2.second.retrieval + pt1.second.retrieval + ret1 * pk1.first};
                        int new_t = geometrically_discretize(new_pair.retrieval, epsilon), new_k = pk1.first + 1;
                        if (new_pair.storage > S)
                            continue;
                        update_DP(DP[v][new_k], new_t, new_pair);
                    }
                }
            }

            // materialize neither
            for (const auto &pk1 : DP[child1]) {
                for (const auto &pt1 : pk1.second) {
                    for (const auto& pk2 : DP[child2]) {
                        for (const auto &pt2 : pk2.second) {
                            edge_variables new_pair = {pt1.second.storage + pt2.second.storage + sto2 - mat2 + sto1 - mat1 + matv,
                                                       pt1.second.retrieval + pt2.second.retrieval + ret1 * pk1.first + ret2 * pk2.first};
                            int new_t = geometrically_discretize(new_pair.retrieval, epsilon), new_k = pk1.first + pk2.first + 1;
                            if (new_pair.storage > S)
                                continue;
                            update_DP(DP[v][new_k], new_t, new_pair);
                        }
                    }
                }
            }
        }
        // calculate OPT
        for (const auto &pk : DP[v]) {
            for (const auto &pt : pk.second) {
                update_OPT(OPT[v], pt.first, pt.second);
            }
        }
    }
    return OPT[r];
}

/**
 * @param G : A bidirectional tree.
 * @param r : the designated root.
 * @return pair {H, Arb} : H is a bidirectional tree, while Arb is an arborescence specifying the ancestry of nodes.
 */
pair<IntGraph, IntGraph> make_binary_bidirectional(const IntGraph &G, int r) {//TODO: test this
    IntGraph Arb = MST_with_designated_root(G, r); ///<This is for defining parent-child hierarchy.
    IntGraph H(G);
    int n = Arb.size(false);
    for (auto v : Arb.get_nodes(false)) {
        auto out_neighbors = Arb.get_out_neighbors_of(v);
        while (out_neighbors.size() > 2) { // start removing child of v.
            n++;
            if (v != r) {
                auto pred = Arb.get_in_neighbors_of(v, false)[0]; // the parent of v

                Arb.add_or_modify_edge(pred, n, H[pred][v], true);
                H.add_or_modify_edge(pred, n, H[pred][v], true);
                H.add_or_modify_edge(n, pred, H[v][pred]);

                Arb.delete_edge(pred, v);
                H.delete_edge(pred, v);
                H.delete_edge(v, pred);
            }

            auto succ = Arb.get_out_neighbors_of(v)[0];
            Arb.add_or_modify_edge(n, v, {0, 0}, true);
            H.add_or_modify_edge(n, v, {0, 0}, true);
            H.add_or_modify_edge(v, n, {0, 0}, true);

            Arb.add_or_modify_edge(0, n, H[0][v]);
            H.add_or_modify_edge(0, n, H[0][v]);

            Arb.add_or_modify_edge(n, succ, H[v][succ]);
            H.add_or_modify_edge(n, succ, H[v][succ]);
            H.add_or_modify_edge(succ, n, H[succ][v]);

            Arb.delete_edge(v, succ);
            H.delete_edge(v, succ);
            H.delete_edge(succ, v);
            out_neighbors = Arb.get_out_neighbors_of(v);
        }
    }
    return {H, Arb};
}

map<int, DP_type, std::greater<>> DP_bidirectional(const IntGraph &G, int r, double epsilon, long long S, long long R_of_MST) {
    auto [H, Arb] = make_binary_bidirectional(G, r);

    // 1. Calculating parameters
    auto nodes = Arb.get_nodes_in_bfs_order(r);
    std::reverse(nodes.begin(), nodes.end());
    auto edges = H.get_edges(false);

    // 2. Base cases
    unordered_map<int, unordered_map<int, unordered_map<int, unordered_map<int, DP_type>>>> DP; ///< [v][k][gamma][t]
    map<int, map<int, DP_type, std::greater<>>> OPT; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.
    unordered_map<int, unordered_map<int, unordered_map<int, DP_type>>> OPT_fixing_k; // OPT[v][gamma][t]

    // count number of possible descendents
    for (const auto &v : nodes) {
        if (Arb.get_out_neighbors_of(v).empty()) {
            DP[v][1][0][0] = OPT[v][0] = OPT_fixing_k[v][0][0] = {H[0][v].storage, 0, 0};
        }
    }

    for (const auto &v : nodes) {
        cout << "\nrecursion on node " << v << endl;
        auto out_edges = Arb.get_out_edges_of(v);
        /// One child
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            long long sto_to = H[v][child].storage, sto_from = H[child][v].storage; /// to and from CHILD
            long long ret_to = H[v][child].retrieval, ret_from = H[child][v].retrieval;
            long long matv = H[0][v].storage, mat = H[0][child].storage;

            // 1. Independent.
            for (const auto &pt : OPT[child]) {
                long long new_s = pt.second.storage + matv;
                if (new_s > S)
                    continue;
                update_DP(DP[v][1][0], pt.first, {new_s, pt.second.retrieval, 0});
            }

            // 2. v to child
            for (const auto &pk : DP[child]) {
                if (pk.second.find(0) == pk.second.end())
                    continue;
                for (const auto &pt : pk.second.at(0)) {
                    DP_type new_DP_var = {sto_to - mat + pt.second.storage + matv,
                                          pt.second.retrieval + ret_to * (long long)pk.first,
                                          0};
                    int new_k = pk.first + 1, new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                    if (new_DP_var.storage > S)
                        continue;
                    update_DP(DP[v][new_k][0], new_t, new_DP_var);
                }
            }

            // 3. child to v
            for (const auto &pg : OPT_fixing_k[child]) {
                for (const auto &pt : pg.second) {
                    DP_type new_DP_var = {sto_from + pt.second.storage,
                                          pt.second.retrieval + ret_from + pt.second.gamma,
                                          ret_from + pt.second.gamma};
                    int new_g = geometrically_discretize(new_DP_var.gamma, epsilon), new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                    if (new_DP_var.storage > S)
                        continue;
                    update_DP(DP[v][1][new_g], new_t, new_DP_var);
                }
            }
        }/// EndFor of onechild

        /// Two children
        else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            long long sto_to1 = H[v][child1].storage, sto_from1 = H[child1][v].storage;
            long long sto_to2 = H[v][child2].storage, sto_from2 = H[child2][v].storage;
            long long ret_to1 = H[v][child1].retrieval, ret_from1 = H[child1][v].retrieval;
            long long ret_to2 = H[v][child2].retrieval, ret_from2 = H[child2][v].retrieval;
            long long matv = H[0][v].storage, mat1 = H[0][child1].storage, mat2 = H[0][child2].storage;
            cout << "node " << v << " has " << DP[child1].size() << " choices of k1, " << DP[child2].size() << " choices of k2.\n";
            cout << OPT[child1].size() << " choices of t1." << endl;
            cout << OPT[child2].size() << " choices of t2." << endl;

            // 1. Completely independent
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pt2 : OPT[child2]) {
                    DP_type new_DP_var = {pt1.second.storage + pt2.second.storage + matv,
                                          pt1.second.retrieval + pt2.second.retrieval,
                                          0};
                    if (new_DP_var.storage > S)
                        continue;
                    update_DP(DP[v][1][0], geometrically_discretize(new_DP_var.retrieval, epsilon), new_DP_var);
                }
            }

            // 2. Only edge is (v, child2)
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pk2 : DP[child2]) {
                    if (pk2.second.find(0) == pk2.second.end())
                        continue;
                    for (const auto &pt2 : pk2.second.at(0)) {
                        DP_type new_DP_var = {sto_to2 - mat2 + pt2.second.storage + pt1.second.storage + matv,
                                              pt2.second.retrieval + ret_to2 * pk2.first + pt1.second.retrieval,
                                              0};
                        int new_k = pk2.first + 1, new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                        if (new_DP_var.storage > S)
                            continue;
                        update_DP(DP[v][new_k][0], new_t, new_DP_var);
                    }
                }
            }

            // 3. Only edge is (v, child1)
            for (const auto &pt2 : OPT[child2]) {
                for (const auto &pk1 : DP[child1]) {
                    if (pk1.second.find(0) == pk1.second.end())
                        continue;
                    for (const auto &pt1 : pk1.second.at(0)) {
                        DP_type new_DP_var = {sto_to1 - mat1 + pt1.second.storage + pt2.second.storage + matv,
                                              pt1.second.retrieval + ret_to1 * (long long)pk1.first + pt2.second.retrieval,
                                              0};
                        int new_k = pk1.first + 1, new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                        if (new_DP_var.storage > S)
                            continue;
                        update_DP(DP[v][new_k][0], new_t, new_DP_var);
                    }
                }
            }

            // 4. Both (v, child1) and (v, child2)
            for (const auto &pk2 : DP[child2]) {
                if (pk2.second.find(0) == pk2.second.end())
                    continue;
                for (const auto &pt2: pk2.second.at(0)) {
                    for (const auto &pk1: DP[child1]) {
                        if (pk1.second.find(0) == pk1.second.end())
                            continue;
                        for (const auto &pt1: pk1.second.at(0)) {
                            DP_type new_DP_var = {sto_to1 - mat1 + sto_to2 - mat2 + pt1.second.storage + pt2.second.storage + matv,
                                                  pt1.second.retrieval + ret_to1 * (long long)pk1.first + pt2.second.retrieval + ret_to2 * pk2.first,
                                                  0};
                            int new_k = pk1.first + pk2.first + 1, new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                            if (new_DP_var.storage > S)
                                continue;
                            update_DP(DP[v][new_k][0], new_t, new_DP_var);
                        }
                    }
                }
            }

            // 5. Only edge is (child1, v)
            for (const auto &pt2 : OPT[child2]) {
                for (const auto &pg1 : OPT_fixing_k[child1]) {
                    for (const auto &pt1 : pg1.second) {
                        DP_type new_DP_var = {sto_from1 + pt1.second.storage + pt2.second.storage,
                                              pt1.second.retrieval + pt2.second.retrieval + pt1.second.gamma + ret_from1,
                                              pt1.second.gamma + ret_from1};
                        int new_g = geometrically_discretize(new_DP_var.gamma, epsilon), new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                        if (new_DP_var.storage > S)
                            continue;
                        update_DP(DP[v][1][new_g], new_t, new_DP_var);
                    }
                }
            }

            // 6. Only edge is (child2, v)
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pg2 : OPT_fixing_k[child2]) {
                    for (const auto &pt2 : pg2.second) {
                        DP_type new_DP_var = {sto_from2 + pt2.second.storage + pt1.second.storage,
                                              pt2.second.retrieval + pt1.second.retrieval + pt2.second.gamma + ret_from2,
                                              pt2.second.gamma + ret_from2};
                        int new_g = geometrically_discretize(new_DP_var.gamma, epsilon), new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                        if (new_DP_var.storage > S)
                            continue;
                        update_DP(DP[v][1][new_g], new_t, new_DP_var);
                    }
                }
            }

            // 7. (child1, v) (v, child2)
            for (const auto &pk2 : DP[child2]) {
                if (pk2.second.find(0) == pk2.second.end())
                    continue;
                for (const auto &pt2: pk2.second.at(0)) {
                    for (const auto &pg1: OPT_fixing_k[child1]) {
                        for (const auto &pt1: pg1.second) {
                            DP_type new_DP_var = {sto_to2 - mat2 + sto_from1 + pt1.second.storage + pt2.second.storage,
                                                  pt1.second.retrieval + (ret_from1 + pt1.second.gamma + ret_to2) * (long long)pk2.first
                                                  + pt2.second.retrieval + ret_from1 + pt1.second.gamma,
                                                  ret_from1 + pt1.second.gamma};
                            int new_k = pk2.first + 1;
                            int new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                            int new_g = geometrically_discretize(new_DP_var.gamma,epsilon);
                            if (new_DP_var.storage > S)
                                continue;
                            update_DP(DP[v][new_k][new_g], new_t, new_DP_var);
                        }
                    }
                }
            }

            // 7. (child2, v) (v, child1)
            for (const auto &pk1 : DP[child1]) {
                if (pk1.second.find(0) == pk1.second.end())
                    continue;
                for (const auto &pt1: pk1.second.at(0)) {
                    for (const auto &pg2: OPT_fixing_k[child2]) {
                        for (const auto &pt2: pg2.second) {
                            DP_type new_DP_var = {sto_to1 - mat1 + sto_from2 + pt2.second.storage + pt1.second.storage,
                                                  pt2.second.retrieval + (ret_from2 + pt2.second.gamma + ret_to1) * (long long)pk1.first
                                                  + pt1.second.retrieval + ret_from2 + pt2.second.gamma,
                                                  ret_from2 + pt2.second.gamma};
                            int new_k = pk1.first + 1;
                            int new_t = geometrically_discretize(new_DP_var.retrieval, epsilon);
                            int new_g = geometrically_discretize(new_DP_var.gamma,epsilon);
                            if (new_DP_var.storage > S)
                                continue;
                            update_DP(DP[v][new_k][new_g], new_t, new_DP_var);
                        }
                    }
                }
            }
            DP.erase(child1);
            DP.erase(child2);
            OPT.erase(child1);
            OPT.erase(child2);
            OPT_fixing_k.erase(child1);
            OPT_fixing_k.erase(child2);
        }///EndFor two children

        // calculate OPT
        for (const auto &pk : DP[v]) {
            for (const auto &pg : pk.second)  {
                for (const auto &pt : pg.second) {
                    update_OPT(OPT[v], pt.first, pt.second);
                }
            }
        }

        // calculate OPT_fixing_k
        for (const auto &pk : DP[v]) {
            for (const auto &pg : pk.second)  {
                for (const auto &pt : pg.second) {
                    update_DP(OPT_fixing_k[v][pg.first], pt.first, pt.second);
                }
            }
        }
    }
    return OPT[r];
}
