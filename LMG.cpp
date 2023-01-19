#include "LMG.h"
using std::map;
using std::pair;

/// Initialization helper: calculating dependency and retrieval for the first time.
void initialize_variables_LMG(IntGraph& H, unordered_map<int, int>& dependency_count,
                               unordered_map<int, int>& retrieval_cost) {
    for (auto v : H.get_nodes(false)) {
        dependency_count[v] = 1;
    }
    // 1. Push element into stack in bfs order, and calculate their retrieval cost along the way.
    vector<int> stack{0};
    retrieval_cost[0] = dependency_count[0] = 0;
    size_t ind = 0;
    while (ind < stack.size()) {
        auto children = H.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            stack.push_back(child);
            retrieval_cost[child] = retrieval_cost[stack[ind]] + H[stack[ind]][child].retrieval;
        }
        ind++;
    }

    // 2. Go backwards in stack and note the number of descendents
    ind = stack.size() - 1;
    while (ind > 0) {
        auto &dep = dependency_count[stack[ind]];
        auto children = H.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            dep += dependency_count[child];
        }
        ind--;
    }
}

IntGraph LMG(const IntGraph& G, int S) {
    IntGraph H = MST(G); // The current graph that LMG works on. Initially it's the MST.
    int storage_surplus = S - H.get_total_storage_cost();
    if (storage_surplus < 0) {
        throw invalid_argument("LMG: storage budget infeasible.");
    }

    /// Initializing dependency, retrieval, and active nodes
    unordered_map<int, int> dependency_count;
    unordered_map<int, int> retrieval_cost;
    initialize_variables_LMG(H, dependency_count, retrieval_cost);


    unordered_set<int> active_nodes; // The set of nodes that are not materialized, and that our budget allows for materialization.
    for (auto v : H.get_nodes(false)) {
        auto pred = H.get_in_neighbors_of(v, true)[0];
        if (pred != 0 && G[0][v].storage - H[pred][v].storage <= storage_surplus) {
            active_nodes.insert(v);
        }
    }

    /// Iteratively materialize nodes while modifying dependency and retrieval accordingly.
    while (!active_nodes.empty()) {
        // 1. Calculate rho for all active nodes and find maximum
        map<int, double> rho; // For each v, rho[v] is the value over cost of materializing v.
        for (auto v : active_nodes) {
            int pred = H.get_in_neighbors_of(v, true)[0];
            // The storage budget SHOULD allow for materializing v.
            if (G[0][v].storage <= G[pred][v].storage) {
                rho[v] = INFINITY;
            } else {
                rho[v] = (double) dependency_count[v] * retrieval_cost[v] / (G[0][v].storage - G[pred][v].storage);
            }
        }
        auto iter = std::max_element(rho.begin(), rho.end(),
                                     [](const auto &x, const auto &y) {
                                         return x.second < y.second;
                                     }); // this is argmax in c++...
        int candidate = iter->first; // vertex to materialize

        // 2. Modify dependency
        int dependency_reduction = dependency_count[candidate], retrieval_reduction = retrieval_cost[candidate];
        int cur = H.get_in_neighbors_of(candidate, true)[0];
        while (cur != 0) {
            dependency_count[cur] -= dependency_reduction;
            cur = H.get_in_neighbors_of(cur, true)[0];
        }

        // 3. Modify retrieval
        vector<int> stack{candidate};
        size_t ind = 0;
        while (ind != stack.size()) {
            retrieval_cost[stack[ind]] -= retrieval_reduction;
            auto children = H.get_out_neighbors_of(stack[ind]);
            stack.insert(stack.end(), children.begin(), children.end());
            ind++;
        }
        for (auto v : stack) {
            retrieval_cost[v] -= retrieval_reduction;
        }

        // 4. Update the graph and active nodes
        int pred = H.get_in_neighbors_of(candidate, true)[0];
        H.delete_edge(pred, candidate);
        H.add_or_modify_edge(0, candidate, G[0][candidate]);
        active_nodes.erase(candidate);
        for (auto v : H.get_nodes(false))
            if (G[0][v].storage > storage_surplus) {
                active_nodes.erase(v);
            }
    }
    return H;
}

void initialize_variables_LMG_all(IntGraph& H, unordered_map<int, unordered_set<int>>& dependency_list,
                                  unordered_map<int, int>& retrieval_cost) {
    for (auto v : H.get_nodes(false)) {
        dependency_list[v].insert(v);
    }
    // 1. Push element into stack in bfs order, and calculate their retrieval cost along the way.
    vector<int> stack{0};
    retrieval_cost[0] = 0;
    // TODO: dependency of 0 is everyone? Necessary?
    size_t ind = 0;
    while (ind < stack.size()) {
        auto children = H.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            stack.push_back(child);
            retrieval_cost[child] = retrieval_cost[stack[ind]] + H[stack[ind]][child].retrieval;
        }
        ind++;
    }

    // 2. Go backwards in stack and note the number of descendents
    ind = stack.size() - 1;
    while (ind > 0) {
        auto &dep = dependency_list[stack[ind]];
        auto children = H.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            dep.insert(dependency_list[child].begin(), dependency_list[child].end());
        }
        ind--;
    }
}

IntGraph LMG_all(const IntGraph& G, int S) {
    IntGraph H = MST(G); // The current graph that LMG works on. Initially it's the MST.
    int storage_surplus = S - H.get_total_storage_cost();
    if (storage_surplus < 0) {
        throw invalid_argument("LMG: storage budget infeasible.");
    }

    /// Initializing dependency, retrieval, and active nodes
    unordered_map<int, unordered_set<int>> dependency_set;
    unordered_map<int, int> retrieval_cost;
    initialize_variables_LMG_all(H, dependency_set, retrieval_cost);

    // TODO: change this
    unordered_set<int> active_edges; // The set of nodes that are not materialized, and that our budget allows for materialization.
    auto edges = G.get_edges(true);
    for (auto i = 0; i < edges.size(); i++) {
        auto &v = get<1>(edges[i]);
        auto pred_w = H[H.get_in_neighbors_of(v, true)[0]][v];
        if (G[0][v].storage - pred_w.storage <= storage_surplus) {
            active_edges.insert(i);
        }
    }

    /// Iteratively materialize nodes while modifying dependency and retrieval accordingly.
    while (!active_edges.empty()) {
        // 1. Calculate rho for all active nodes and find maximum
        map<int, double> rho; // For each i, rho[i] is the value over cost of replacing some edge with edge i.
        for (auto i : active_edges) {
            auto &[u, v, w] = edges[i];
            auto pred = H.get_in_neighbors_of(v, true)[0];
            // The storage budget SHOULD allow for materializing v.
            if (dependency_set[v].find(u) != dependency_set[v].end()) { // if u is dependent on v, meaning (u,v) forms a cycle
                rho[v] = -1;
            } else if (G[0][v].storage <= G[pred][v].storage) {
                rho[v] = INFINITY;
            } else {
                rho[v] = (double) dependency_set[v].size() * (retrieval_cost[v] - (retrieval_cost[u] + w.retrieval))
                         / (G[0][v].storage - G[pred][v].storage);
            }
        }
        auto iter = std::max_element(rho.begin(), rho.end(),
                                     [](const auto &x, const auto &y) {
                                         return x.second < y.second;
                                     }); // this is argmax in c++...
        auto &[u, v, w] = edges[iter->first]; // vertex to materialize

        // 2. Modify dependency
        const auto &dependency_reduction = dependency_set[v];
        int retrieval_reduction = retrieval_cost[v] - (retrieval_cost[u] + w.retrieval);
        if (retrieval_reduction < 0) {
            throw logic_error("Unwise step");
        }

        int cur = H.get_in_neighbors_of(v, true)[0];
        while (cur != 0) {
            for (auto node : dependency_reduction)
                dependency_set[cur].erase(node);
            cur = H.get_in_neighbors_of(cur, true)[0];
        }
        cur = u; // additional step of adding dependency to u's ancestors
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
        H.delete_edge(pred, v);
        H.add_or_modify_edge(u, v, w);
        active_edges.erase(iter->first);
        for (auto i =0; i < edges.size(); i++) {
            auto &v = get<1>(edges[i]);
            auto pred_w = H[H.get_in_neighbors_of(v, true)[0]][v];
            if (G[0][v].storage - pred_w.storage <= storage_surplus) {
                active_edges.erase(i);
            }
        }
    }
    return H;
}