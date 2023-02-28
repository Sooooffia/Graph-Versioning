#include "LMG.h"
#include <limits>
using std::map;
using std::pair;
using std::cout;


IntGraph LMG(const IntGraph& G, long long S) {
    IntGraph H = MST(G); // The current graph that LMG works on. Initially it's the MST.
    long long storage_surplus = S - H.get_total_storage_cost();
    if (storage_surplus < 0) {
        throw invalid_argument("LMG: storage budget infeasible.");
    }

    /// Initializing dependency, retrieval, and active nodes
    unordered_map<int, unordered_set<int>> dependency_set;
    unordered_map<int, long long> retrieval_cost;

    H.get_dependency_list_and_retrieval_cost(dependency_set, retrieval_cost);


    unordered_set<int> active_nodes; // The set of nodes that are not materialized, and that our budget allows for materialization.
    for (auto v : H.get_nodes(false)) {
        auto pred = H.get_in_neighbors_of(v, true)[0];
        if (pred != 0 && G[0][v].storage - H[pred][v].storage <= storage_surplus) {
            active_nodes.insert(v);
        }
    }

    /// Iteratively materialize nodes while modifying dependency and retrieval accordingly.
    while (!active_nodes.empty()) {
//        cout << active_nodes.size() << " nodes left" << endl;
        // 1. Calculate rho for all active nodes and find maximum
        unordered_map<int, double> rho; // For each v, rho[v] is the value over cost of materializing v.
        for (auto v : active_nodes) {
            int pred = H.get_in_neighbors_of(v, true)[0];
            // The storage budget SHOULD allow for materializing v.
            if (G[0][v].storage <= G[pred][v].storage) {
                rho[v] = std::numeric_limits<double>::infinity();
            } else {
                rho[v] = double(dependency_set[v].size()) * retrieval_cost[v] / (G[0][v].storage - G[pred][v].storage);
            }
        }
        auto iter = std::max_element(rho.begin(), rho.end(),
                                     [](const auto &x, const auto &y) {
                                         return x.second < y.second;
                                     }); // this is argmax in c++...
        int candidate = iter->first; // vertex to materialize
        if (iter->second <= 0)// No possible improvements
            break;

        // 2. Modify dependency
        const auto & dependency_reduction = dependency_set[candidate];
        auto retrieval_reduction = retrieval_cost[candidate];
        int cur = H.get_in_neighbors_of(candidate, true)[0];
        while (cur != 0) {
            for (const auto &node : dependency_reduction)
                dependency_set[cur].erase(node);
            cur = H.get_in_neighbors_of(cur, true)[0];
        }

        // 3. Modify retrieval
        for (auto child : dependency_reduction) {
            retrieval_cost[child] -= retrieval_reduction;
        }

        // 4. Update the graph and active nodes
        int pred = H.get_in_neighbors_of(candidate, true)[0];
        storage_surplus -= G[0][candidate].storage - G[pred][candidate].storage;
        H.delete_edge(pred, candidate);
        H.add_or_modify_edge(0, candidate, G[0][candidate]);
        active_nodes.erase(candidate);
        for (auto v : H.get_nodes(false)) {
            if (active_nodes.find(v) == active_nodes.end())
                continue;
            if (G[0][v].storage - H[pred][v].storage > storage_surplus) {
                active_nodes.erase(v);
            }
        }
    }
    return H;
}
IntGraph LMG_all(const IntGraph& G, long long S) {
    IntGraph H = MST(G); // The current graph that LMG works on. Initially it's the MST.
    long long storage_surplus = S - H.get_total_storage_cost();
    if (storage_surplus < 0) {
        throw invalid_argument("LMG: storage budget infeasible.");
    }

    /// Initializing dependency, retrieval, and active nodes
    unordered_map<int, unordered_set<int>> dependency_set;
    unordered_map<int, long long> retrieval_cost;
    H.get_dependency_list_and_retrieval_cost(dependency_set, retrieval_cost);

    unordered_set<int> active_edges; // The set of edges that our budget allows for materialization.
    auto edges = G.get_edges(true);
    for (auto i = 0; i < edges.size(); i++) {
        const auto &[u, v, w] = edges[i];
        auto pred = H.get_in_neighbors_of(v, true)[0];
        int storage_increase = w.storage - H[pred][v].storage;
        if (pred != u and storage_increase <= storage_surplus) {
            active_edges.insert(i);
        }
    }

    /// Iteratively materialize nodes while modifying dependency and retrieval accordingly.
    while (!active_edges.empty()) {
//        cout << active_edges.size() << " edges left " << endl;
        // 1. Calculate rho for all active nodes and find maximum
        unordered_map<int, double> rho; // For each i, rho[i] is the value over cost of replacing some edge with edge i.
        for (auto i : active_edges) {
            auto &[u, v, w] = edges[i];
            auto pred = H.get_in_neighbors_of(v, true)[0];
            int retrieval_reduction = retrieval_cost[v] - (retrieval_cost[u] + w.retrieval),
                storage_increase = w.storage - G[pred][v].storage;
            // The storage budget SHOULD allow for adding (u,v).
            if (retrieval_reduction <= 0 or dependency_set[v].find(u) != dependency_set[v].end() ) { // if (u,v) forms a cycle or retrieval does not decrease
                rho[i] = INT32_MIN;
            } else if (storage_increase <= 0) {
                rho[i] = std::numeric_limits<double>::infinity();
            } else {
                rho[i] = double(dependency_set[v].size()) * retrieval_reduction
                         / storage_increase;
            }
        }
        auto iter = std::max_element(rho.begin(), rho.end(),
                                     [](const auto &x, const auto &y) {
                                         return x.second < y.second;
                                     }); // this is argmax in c++...
        const auto &[u, v, w] = edges[iter->first]; // vertex to materialize
        if (iter->second <= 0)// No possible improvements
            break;

        // 2. Modify dependency
        const auto &dependency_reduction = dependency_set[v];
        int retrieval_reduction = retrieval_cost[v] - (retrieval_cost[u] + w.retrieval);
        if (retrieval_reduction <= 0) { // No possible improvements
            break;
        }

        int cur = H.get_in_neighbors_of(v, true)[0];
        while (cur != 0) {
            for (const auto &node : dependency_reduction)
                dependency_set[cur].erase(node);
            cur = H.get_in_neighbors_of(cur, true)[0];
        }
        cur = u; // adding dependency to u's ancestors
        while (cur != 0) {
            dependency_set[cur].insert(dependency_reduction.begin(), dependency_reduction.end());
            cur = H.get_in_neighbors_of(cur, true)[0];
        }

        // 3. Modify retrieval
        for (auto child : dependency_reduction) {
            retrieval_cost[child] -= retrieval_reduction;
        }

        // 4. Update the graph and active nodes

        int pred = H.get_in_neighbors_of(v, true)[0];
        storage_surplus -= w.storage - H[pred][v].storage;
        H.delete_edge(pred, v);
        H.add_or_modify_edge(u, v, w);
        active_edges.erase(iter->first);
        for (auto i = 0; i < edges.size(); i++) {
            if (active_edges.find(i) == active_edges.end())
                continue;
            const auto &[u, v, w] = edges[i];
            auto pred = H.get_in_neighbors_of(v, true)[0];
            if (w.storage - H[pred][v].storage > storage_surplus) {
                active_edges.erase(i);
            }
        }
    }
    return H;
}