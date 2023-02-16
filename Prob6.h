#ifndef GRAPH_VERSIONING_PROB6_H
#define GRAPH_VERSIONING_PROB6_H
#include "Graph.hpp"

struct cmp {
    bool operator()(const tuple<int,int>& a, const tuple<int,int>& b) const{
        return get<1>(a) < get<1>(b);
    }
};
IntGraph Modified_Prim(const IntGraph &G, int R) {
    priority_queue<tuple<int, int>, vector<tuple<int,int>>, cmp> PQ;
    PQ.push({0,0});
    unordered_map<int, int> p;
    vector<int> d(G.size(false), INT32_MAX), l(G.size(false), INT32_MAX);
    p[0] = d[0] = l[0] = 0;
    unordered_set<int> X;

    IntGraph ans;
    while (not PQ.empty()) {
        auto [i, li] = PQ.top(); PQ.pop();
        ans.add_or_modify_edge(i, p[i]);
        if (i != 0)
            X.insert(i);
        for (const int &j : G.get_out_neighbors_of(i)) {
            if (X.find(i) != X.end()) {
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
                PQ.push({j, l[j]});
            }
        }
    }
    return ans;
}

unordered_map<int, unordered_map<int, unordered_set<int>>> all_pair_Dijkstra_paths(const IntGraph &G, int cutoff = INT32_MAX) {
    int n = G.size(false);
    vector<int> nodes = G.get_nodes_in_bfs_order(false);
    vector<vector<int>> dist(n+1, vector<int>(n+1, INT32_MAX));
    vector<vector<int>> pred(n+1, vector<int>(n+1, -1));
    unordered_map<int, unordered_map<int, unordered_set<int>>> path;
    for (int i = 1; i <= n; i++) {// i is the current source
        unordered_set<int> active_nodes(nodes.begin(), nodes.end());
        while (not active_nodes.empty()) {
            int v = -1, mindist = INT32_MAX;
            for (const auto &j : active_nodes) if (mindist > dist[i][j]) {
                    mindist = dist[i][j];
                    v = j;
                }

            active_nodes.erase(v);
            for (const auto &u : G.get_out_neighbors_of(v))
                if (active_nodes.find(u) != active_nodes.end()) {
                    int newdist = dist[i][u] + G[v][u].retrieval;
                    if (dist[i][v] > newdist) {
                        dist[i][v] = newdist;
                        pred[i][v] = u;
                    }
                }
        }

        for (int v = 1; v <= n; v ++) {
            if (dist[i][v] > cutoff)
                continue;
            int u = v;
            while (u != i) {
                path[i][v].insert(u);
                u = pred[i][u];
            }
        }
    }
    return path;
}

int DP_BMR(const IntGraph &G, int R) {
    int n = G.size(false);
    vector<vector<int>> DP(n+1, vector<int>(n+1, INT32_MAX));
    vector<int> OPT(G.size(true), INT32_MAX);
    vector<int> nodes = G.get_nodes_in_bfs_order(false);
    auto paths = all_pair_Dijkstra_paths(G, R);

    for (const auto &v : nodes) {
        for (const auto &u : nodes) {
            if (paths[u].find(v) == paths[u].end())
                continue;
            int sum = 0;
            //TODO: finish this
        }
    }
}

#endif //GRAPH_VERSIONING_PROB6_H
