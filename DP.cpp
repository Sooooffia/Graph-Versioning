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

//unordered_map<int, int> get_descendants_of_arb(const IntGraph &G, int r) {
//    unordered_map<int, int> desc_count;
//    vector<int> stack{r};
//    int ind = 0;
//    while (ind < stack.size()) {
//        desc_count[stack[ind]] = 1;
//        auto children = G.get_out_neighbors_of(stack[ind]);
//        for (auto child : children) {
//            stack.push_back(child);
//        }
//        ind++;
//    }
//    ind = stack.size() - 1;
//    while (ind >= 0) {
//        auto children = G.get_out_neighbors_of(stack[ind]);
//        for (int child : children) {
//            desc_count[stack[ind]] += desc_count[child];
//        }
//        ind--;
//    }
//    return desc_count;
//}
//
//int DP_arborescence(IntGraph &G, int r, double l, int S, int R_of_MST) {
//    G = make_binary(G, r);
//    // 1. Calculating parameters
//    auto nodes = G.get_nodes_in_bfs_order(r);
//    auto edges = G.get_edges(false);
//    int n = nodes.size();
//
//    int T = R_of_MST / l;
//    for (auto &[u,v,w]: G.get_edges(false)) {
//        w.retrieval /= l;
//        w.retrieval += 1;
//    }
//
//    // 2. Base cases
//    unordered_map<int, unordered_map<int, unordered_map<int, int>>> DP;
//    map<int, map<int, int>> OPT; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.
//    std::reverse(nodes.begin(), nodes.end());
//
//    // count number of possible descendents
//    unordered_map<int, int> desc_count = get_descendants_of_arb(G, r);
//    std::cout << "desc_count: " << desc_count << std::endl;
//    for (auto v : nodes) {
//        if (desc_count[v] == 1) {
//            DP[v][1][0] = G[0][v].storage;
//            OPT[v][0] = G[0][v].storage;
//        }
//    }
//
//    for (auto v : nodes) {
//        std::cout << "\nrecursion on node " << v << std::endl;
//        auto out_edges = G.get_out_edges_of(v);
//        // One child
//        if (out_edges.size() == 1) {
//            int child = out_edges.begin()->first;
//            int sto = out_edges.begin()->second.storage, ret = out_edges.begin()->second.retrieval;
//            int matv = G[0][v].storage, mat = G[0][child].storage;
//
//            // k = 1 case
//            for (auto pt : OPT[child]) {
//                int news = pt.second + matv;
//                if (news > S)
//                    continue;
//                DP[v][1][pt.first] = news;
//            }
//
//            // k > 1 case
//            for (auto pk : DP[child]) {
//                for (auto pt : pk.second) {
//                    int newk = pk.first+1, newt = pt.first + ret * pk.first, news = sto - mat + pt.second + matv;
//                    if (news > S)
//                        continue;
//                    if (DP[v][newk].find(newt) == DP[v][newk].end()) {
//                        DP[v][newk][newt] = INT32_MAX;
//                    }
//                    DP[v][newk][newt] = std::min(DP[v][newk][newt], news);
//                }
//            }
//        }
//
//            // Two children
//        else if (out_edges.size() == 2) {
//            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
//            int sto1 = out_edges.begin()->second.storage, ret1 = out_edges.begin()->second.retrieval,
//                    sto2 = (++out_edges.begin())->second.storage, ret2 = (++out_edges.begin())->second.retrieval;
//            int matv = G[0][v].storage, mat1 = G[0][child1].storage, mat2 = G[0][child2].storage;
//            std::cout << "node " << v << " has " << DP[child1].size() << " choices of k1, " << DP[child2].size() << " choices of k2.\n";
//            std::cout << OPT[child1].size() << " choices of t1.\n";
//            std::cout << OPT[child2].size() << " choices of t2.\n";
//
//            // materialize neither child (k=1)
//            for (const auto &p1 : OPT[child1]) {
//                for (const auto &p2 : OPT[child2]) {
//                    int newt = p1.first + p2.first, news =  p1.second + p2.second + matv;
//                    if (news > S)
//                        continue;
//                    if (DP[v][1].find(newt) == DP[v][1].end())
//                        DP[v][1][newt] = INT32_MAX;
//                    DP[v][1][newt] = std::min(DP[v][1][newt], news);
//                }
//            }
//
//            // only materialize child1
//            for (const auto &pt1 : OPT[child1]) {
//                for (const auto &pk2 : DP[child2]) {
//                    for (const auto &pt2 : pk2.second) {
//                        int newk = pk2.first+1, newt = pt1.first + pt2.first + ret2 * pk2.first;
//                        int news = pt1.second + pt2.second + sto2 - mat2 + matv;
//                        if (news > S)
//                            continue;
//                        if (DP[v][newk].find(newt) == DP[v][newk].end())
//                            DP[v][newk][newt] = INT32_MAX;
//                        DP[v][newk][newt] = std::min(DP[v][newk][newt], news);
//                    }
//                }
//            }
//
//            // only materialize child2
//            for (const auto &pt2 : OPT[child2]) {
//                for (const auto &pk1 : DP[child1]) {
//                    for (const auto &pt1 : pk1.second) {
//                        int newk = pk1.first+1, newt = pt2.first + pt1.first + ret1 * pk1.first;
//                        int news = pt2.second + pt1.second + sto1 - mat1 + matv;
//                        if (news > S)
//                            continue;
//                        if (DP[v][newk].find(newt) == DP[v][newk].end())
//                            DP[v][newk][newt] = INT32_MAX;
//                        DP[v][newk][newt] = std::min(DP[v][newk][newt], news);
//                    }
//                }
//            }
//
//            // materialize both
//            for (const auto &pk1 : DP[child1]) {
//                for (const auto &pt1 : pk1.second) {
//                    for (const auto& pk2 : DP[child2]) {
//                        for (const auto &pt2 : pk2.second) {
//                            int newk = pk1.first + pk2.first + 1, newt = pt1.first + pt2.first + pk1.first*ret1 + pk2.first*ret2;
//                            int news = pt2.second + pt1.second + sto1 - mat1 + sto2 - mat2 + matv;
//                            if (news > S)
//                                continue;
//                            auto &thisDP = DP[v][newk];
//                            if (thisDP.find(newt) == thisDP.end())
//                                thisDP[newt] = INT32_MAX;
//                            thisDP[newt] = std::min(thisDP[newt], news);
//                        }
//                    }
//                }
//            }
//        }
//
//        // calculate OPT
//        for (auto &pk : DP[v]) {
//            for (auto &pt : pk.second) {
//                if (OPT[v].find(pt.first) == OPT[v].end())
//                    OPT[v][pt.first] = INT32_MAX;
//                OPT[v][pt.first] = std::min(OPT[v][pt.first], pt.second);
//            }
//        }
//    }
//    std::cout << OPT[r] << std::endl;
//    for (int t = T; t >= 0; t--) {
//        if (OPT[r][t] > S)
//            return (t+1) * l;
//    }
//}

double log_base_a_of_b(double a, double b) {
    return log2(b) / log2(a);
}

int getmetrically_discretize(int val, double epsilon) {
    return int(log_base_a_of_b(1+epsilon, val-1) + 2);
}

void update_DP(unordered_map<int, edge_variables> &t_map, int t, const edge_variables &new_val) {
    if (t_map.find(t) == t_map.end())
        t_map[t] = {INT32_MAX, INT32_MAX};

    if (t_map[t].storage > new_val.storage)
        t_map[t] = new_val;
}

void update_OPT(map<int, edge_variables, std::greater<>> &t_map, int t, const edge_variables &new_val) {
    if (t_map.find(t) == t_map.end())
        t_map[t] = {INT32_MAX, INT32_MAX};

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

    int T = log_base_a_of_b(1+epsilon, R_of_MST);

    // 2. Base cases
    unordered_map<int, unordered_map<int, unordered_map<int, edge_variables>>> DP; ///<now we are storing a pair (s,r) of the subsolution.
    map<int, map<int, edge_variables, std::greater<>>> OPT; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.

    // count number of possible descendents
    for (const auto &v : nodes) {
        if (H.get_out_neighbors_of(v).size() == 0) {
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
            for (auto pt : OPT[child]) {
                int new_s = pt.second.storage + matv;
                if (new_s > S)
                    continue;
                DP[v][1][pt.first] = {new_s, pt.second.retrieval};
            }

            // k > 1 case
            for (const auto &pk : DP[child]) {
                for (const auto &pt : pk.second) {
                    edge_variables new_pair = {sto - mat + pt.second.storage + matv, pt.second.retrieval + ret * pk.first};
                    int new_k = pk.first + 1, new_t = getmetrically_discretize(new_pair.retrieval, epsilon);
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

            // materialize neither child (k=1)
            for (const auto &p1 : OPT[child1]) {
                for (const auto &p2 : OPT[child2]) {
                    edge_variables new_pair = {p1.second.storage + p2.second.storage + matv, p1.second.retrieval + p1.second.retrieval};
                    int new_t = getmetrically_discretize(new_pair.retrieval, epsilon);
                    if (new_pair.storage > S)
                        continue;
                    update_DP(DP[v][1], new_t, new_pair);
                }
            }

            // only materialize child1
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pk2 : DP[child2]) {
                    for (const auto &pt2 : pk2.second) {
                        edge_variables new_pair = {pt1.second.storage + pt2.second.storage + sto2 - mat2 + matv,
                                                   pt1.second.retrieval + pt2.second.retrieval + ret2 * pk2.first};
                        int new_t = getmetrically_discretize(new_pair.retrieval, epsilon), new_k = pk2.first + 1;
                        if (new_pair.storage > S)
                            continue;
                        update_DP(DP[v][new_k], new_t, new_pair);
                    }
                }
            }

            // only materialize child2
            for (const auto &pt2 : OPT[child2]) {
                for (const auto &pk1 : DP[child1]) {
                    for (const auto &pt1 : pk1.second) {
                        edge_variables new_pair = {pt2.second.storage + pt1.second.storage + sto1 - mat1 + matv,
                                                   pt2.second.retrieval + pt1.second.retrieval + ret1 * pk1.first};
                        int new_t = getmetrically_discretize(new_pair.retrieval, epsilon), new_k = pk1.first + 1;
                        if (new_pair.storage > S)
                            continue;
                        update_DP(DP[v][new_k], new_t, new_pair);
                    }
                }
            }

            // materialize both
            for (const auto &pk1 : DP[child1]) {
                for (const auto &pt1 : pk1.second) {
                    for (const auto& pk2 : DP[child2]) {
                        for (const auto &pt2 : pk2.second) {
                            edge_variables new_pair = {pt1.second.storage + pt2.second.storage + sto2 - mat2 + sto1 - mat1 + matv,
                                                       pt1.second.retrieval + pt2.second.retrieval + ret1 * pk1.first + ret2 * pk2.first};
                            int new_t = getmetrically_discretize(new_pair.retrieval, epsilon), new_k = pk1.first + pk2.first + 1;
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
                H.add_or_modify_edge(n, pred, H[v][pred], true);
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

map<int, edge_variables, std::greater<>> DP_bidirectional(const IntGraph &G, int r, double epsilon, int S, int R_of_MST) {
    auto [H, Arb] = make_binary_bidirectional(G, r);
    //TODO: change this

    // 1. Calculating parameters
    auto nodes = H.get_nodes_in_bfs_order(r);
    std::reverse(nodes.begin(), nodes.end());
    auto edges = H.get_edges(false);

    // 2. Base cases
    unordered_map<int, unordered_map<int, unordered_map<int, unordered_map<int, edge_variables>>>> DP; ///<now we are storing a pair (s,r) of the subsolution.
    map<int, map<int, edge_variables, std::greater<>>> OPT; // OPT[v][t] is the optimal storage cost in the subtree induced by v, given t retrieval constraint.

    // count number of possible descendents
    for (const auto &v : nodes) {
        if (Arb.get_out_neighbors_of(v).empty()) {
            DP[v][1][0][0] = OPT[v][0] = {H[0][v].storage, 0};
        }
    }

    for (const auto &v : nodes) {
        cout << "\nrecursion on node " << v << endl;
        auto out_edges = Arb.get_out_edges_of(v);
        /// One child
        if (out_edges.size() == 1) {
            int child = out_edges.begin()->first;
            int sto = out_edges.begin()->second.storage, ret = out_edges.begin()->second.retrieval;
            int matv = H[0][v].storage, mat = H[0][child].storage;

            // k = 1 case
            for (const auto &pt : OPT[child]) {
                int new_s = pt.second.storage + matv;
                if (new_s > S)
                    continue;
                DP[v][1][pt.first] = {new_s, pt.second.retrieval};
            }

            // k > 1 case
            for (const auto &pk : DP[child]) {
                for (const auto &pt : pk.second) {
                    edge_variables new_pair = {sto - mat + pt.second.storage + matv, pt.second.retrieval + ret * pk.first};
                    int new_k = pk.first + 1, new_t = getmetrically_discretize(new_pair.retrieval, epsilon);
                    if (new_pair.storage > S)
                        continue;
                    update_DP(DP[v][new_k], new_t, new_pair);
                }
            }
        }/// EndFor of onechild

        /// Two children
        else if (out_edges.size() == 2) {
            int child1 = out_edges.begin()->first, child2 = (++out_edges.begin())->first;
            int sto1 = out_edges.begin()->second.storage, ret1 = out_edges.begin()->second.retrieval,
                    sto2 = (++out_edges.begin())->second.storage, ret2 = (++out_edges.begin())->second.retrieval;
            int matv = H[0][v].storage, mat1 = H[0][child1].storage, mat2 = H[0][child2].storage;
            cout << "node " << v << " has " << DP[child1].size() << " choices of k1, " << DP[child2].size() << " choices of k2.\n";
            cout << OPT[child1].size() << " choices of t1." << endl;
            cout << OPT[child2].size() << " choices of t2." << endl;

            // materialize neither child (k=1)
            for (const auto &p1 : OPT[child1]) {
                for (const auto &p2 : OPT[child2]) {
                    edge_variables new_pair = {p1.second.storage + p2.second.storage + matv, p1.second.retrieval + p1.second.retrieval};
                    int new_t = getmetrically_discretize(new_pair.retrieval, epsilon);
                    if (new_pair.storage > S)
                        continue;
                    update_DP(DP[v][1], new_t, new_pair);
                }
            }

            // only materialize child1
            for (const auto &pt1 : OPT[child1]) {
                for (const auto &pk2 : DP[child2]) {
                    for (const auto &pt2 : pk2.second) {
                        edge_variables new_pair = {pt1.second.storage + pt2.second.storage + sto2 - mat2 + matv,
                                                   pt1.second.retrieval + pt2.second.retrieval + ret2 * pk2.first};
                        int new_t = getmetrically_discretize(new_pair.retrieval, epsilon), new_k = pk2.first + 1;
                        if (new_pair.storage > S)
                            continue;
                        update_DP(DP[v][new_k], new_t, new_pair);
                    }
                }
            }

            // only materialize child2
            for (const auto &pt2 : OPT[child2]) {
                for (const auto &pk1 : DP[child1]) {
                    for (const auto &pt1 : pk1.second) {
                        edge_variables new_pair = {pt2.second.storage + pt1.second.storage + sto1 - mat1 + matv,
                                                   pt2.second.retrieval + pt1.second.retrieval + ret1 * pk1.first};
                        int new_t = getmetrically_discretize(new_pair.retrieval, epsilon), new_k = pk1.first + 1;
                        if (new_pair.storage > S)
                            continue;
                        update_DP(DP[v][new_k], new_t, new_pair);
                    }
                }
            }

            // materialize both
            for (const auto &pk1 : DP[child1]) {
                for (const auto &pt1 : pk1.second) {
                    for (const auto& pk2 : DP[child2]) {
                        for (const auto &pt2 : pk2.second) {
                            edge_variables new_pair = {pt1.second.storage + pt2.second.storage + sto2 - mat2 + sto1 - mat1 + matv,
                                                       pt1.second.retrieval + pt2.second.retrieval + ret1 * pk1.first + ret2 * pk2.first};
                            int new_t = getmetrically_discretize(new_pair.retrieval, epsilon), new_k = pk1.first + pk2.first + 1;
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
