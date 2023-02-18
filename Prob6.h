#ifndef GRAPH_VERSIONING_PROB6_H
#define GRAPH_VERSIONING_PROB6_H
#include "Graph.hpp"

IntGraph Modified_Prim(const IntGraph &G, int R) {
    int n = G.size(false);

    vector<int> p(n+1);
    vector<long long> d(n+1, INT32_MAX), l(n+1, INT32_MAX);
    p[0] = d[0] = l[0] = 0;
    auto cmp = [&l](const int &a, const int &b) {
        return l[a] < l[b];
    };
    set<int, decltype(cmp)> PQ(cmp);
    PQ.insert(0);

    IntGraph ans;
    while (not PQ.empty()) {
        int i = *(PQ.begin());
        PQ.erase(PQ.begin());
        if (i != 0)
            ans.add_or_modify_edge(p[i], i, G[p[i]][i], true);
        for (auto j : G.get_out_neighbors_of(i)) {
            auto nodes = ans.get_nodes(false);
            if (nodes.find(j) != nodes.end()) {
                if (G[i][j].retrieval + d[i] <= d[j] and G[i][j].storage <= l[j]) {
                    ans.delete_edge(p[j], j);
                    p[j] = i;
                    ans.add_or_modify_edge(i, j, G[i][j]);
                    d[j] = d[i] + G[i][j].retrieval;
                    l[j] = G[i][j].storage;
                }
            } else if (G[i][j].retrieval + d[i] <= R and G[i][j].storage <= l[j]) {
                d[j] = d[i] + G[i][j].retrieval;
                l[j] = G[i][j].storage;
                p[j] = i;
                PQ.insert(j);
            }
        }
    }
    return ans;
}

long long DP_BMR(const IntGraph &G, long long R) {
    int n = G.size(false);
    vector<vector<long long>> DP(n+1, vector<long long>(n+1, INT64_MAX));
    vector<long long> OPT(G.size(true), INT64_MAX);
    IntGraph H = MST_with_designated_root(G, 1); // hierarchy
    vector<int> nodes = H.get_nodes_in_bfs_order(1);
    std::reverse(nodes.begin(), nodes.end());

    for (const auto &v : nodes) {
        //paths and distances, for each source u with target v.
        vector<long long> dist(n+1, INT64_MAX); // dist[u] is the distance from u to v.
        unordered_map<int, vector<int>> paths; // nodes traversed going from u to v, in BACKWARDS order. (namely, paths[u][0]=v for all u)
        vector<int> pq = {v};
        vector<bool>visited(n+1, false);
        int ind = 0;
        dist[v] = 0;
        visited[v] = true;
        paths[v].push_back(v);

        while (ind < pq.size()) {
            int u = pq[ind];
            for (const auto &child : G.get_out_neighbors_of(u)) {
                if (not visited[child]) {
                    visited[child] = true;
                    dist[child] = dist[u] + G[child][u].retrieval;
                    paths[child] = paths[u]; // copy?
                    paths[child].push_back(child);
                    pq.push_back(child);
                }
            }
            ind++;
        }

        // recursion
        for (const auto &u : nodes) {
            if (dist[u] > R)
                continue;
            long long sum = 0;
            for (const auto &w : H.get_out_neighbors_of(v)) {
                if (u != v and paths[u][1] == w) // if w is in the path from u to v
                    sum += DP[w][u];
                else
                    sum += std::min(DP[w][u], OPT[w]);
            }
            if (u == v)
                sum += G[0][v].storage;
            else
                sum += G[paths[u][1]][v].storage;

            DP[v][u] = sum;
            if (v == 1 or v == u or paths[u][1] != H.get_in_neighbors_of(v,false)[0])
                OPT[v] = std::min(OPT[v], sum);
        }
    }
    return OPT[1];
}

#endif //GRAPH_VERSIONING_PROB6_H
