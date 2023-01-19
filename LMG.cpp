#include "LMG.h"
using std::map;

/// Initialization helper: calculating dependency and retrieval for the first time.
void initialize_dependency_and_retrieval(IntGraph& H, unordered_map<int, int>& dependency_count,
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
        int dep = 1;
        auto children = H.get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            dep += dependency_count[child];
        }
        dependency_count[stack[ind]] = dep;
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
    initialize_dependency_and_retrieval(H, dependency_count, retrieval_cost);


    unordered_set<int> active_nodes; // The set of nodes that are not materialized, and that our budget allows for materialization.
    for (auto v : H.get_nodes(false)) {
        auto pred = H.get_in_neighbors_of(v, true)[0];
        if (pred != 0 && G[0][v].storage - H[pred][v].storage <= storage_surplus) {
            active_nodes.insert(v);
        }
    }

    /// Iteratively materialize nodes and modifying dependency and retrieval accordingly.
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

        // 2. Modify dependency and retrieval
        int dependency_reduction = dependency_count[candidate], retrieval_reduction = retrieval_cost[candidate];
        int cur = candidate;
        while (cur != 0) {
            dependency_count[cur] -= dependency_reduction;
            cur = H.get_in_neighbors_of(cur, true)[0];
        }

        vector<int> stack{candidate};
        size_t ind = 0;
        while (ind != stack.size()) {
            retrieval_cost[stack[ind]] -= retrieval_reduction;
            auto children = H.get_out_neighbors_of(stack[ind]);
            stack.insert(stack.end(), children.begin(), children.end());
            ind++;
        }

        // 3. Update the graph and active nodes
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