//
// Created by Anxin Guo on 11/15/22.
//

#include "Graph.hpp"
#include <cstring>
using namespace std;

ostream& operator<<(ostream& os, const edge_variables& w) {
    os << "(s=" << w.storage << ", r=" << w.retrieval << ")";
    return os;
}

// Helper functions //
void IntGraph::make_node(int v) {
    if (v != 0)
        n++;
    nodes.insert(v);
    inNeighbors.insert({v, {}});
    outNeighbors.insert({v, {}});
}
void IntGraph::make_edge(int u, int v, const edge_variables& costs) {
    inNeighbors[v][u] = costs;
    outNeighbors[u][v] = costs;
}

// Constructors //
IntGraph::IntGraph() { make_node(0); }
IntGraph::IntGraph(int node_number) {
    make_node(0);
    while (n < node_number) {
        make_node(n+1);
        make_edge(0,n,{0,0});
    }
}
IntGraph::IntGraph(const vector<tuple<int,int>>& vertices, const vector<tuple<int, int, edge_variables>>& edges) {
    make_node(0);
    for (auto v : vertices) {
        if (get<0>(v) == 0)
            throw invalid_argument("IntGraph initialization: auxiliary root involved in nodes.");
        int& vertex = get<0>(v);
        int& storage =  get<1>(v);
        make_node(vertex);
        make_edge(0,vertex,{storage,0});
    }
    for (auto e : edges) {
        int& pred = get<0>(e);
        int& succ = get<1>(e);
        auto& costs = get<2>(e);
        if (pred == 0 || succ == 0){
            throw invalid_argument("IntGraph initialization: auxiliary root involved in edges.");
        }
        make_edge(pred, succ, costs);
    }
}
IntGraph::IntGraph(const vector<tuple<int, int, edge_variables>>& edges) {
    make_node(0);
    for (auto e : edges) {
        int& u = get<0>(e);
        int& v = get<1>(e);
        auto& w = get<2>(e);
        add_or_modify_edge(u, v, w, true);
    }
    m = edges.size();
}
IntGraph::IntGraph(int node_count, float p, bool equal_weights) {
    // lambda
    auto make_random_edge = [equal_weights, this](int u, int v) {
        if (equal_weights) {
            int weight = rand() % 100 + 1;
            make_edge(u, v, {weight, weight});
        } else {
            make_edge(u, v, { rand() % 100 + 1,  rand() % 100 + 1});
        }
    };

    nodes.reserve(node_count);
    inNeighbors.reserve(node_count);
    outNeighbors.reserve(node_count);
    make_node(0);
    while (n < node_count) {
        make_node(n+1);
        make_edge(0,n,{rand() % 100 + 1,0});
    }
    // TODO: seed? This random produces the same results each time.
    for (int i = 1; i < n; i ++) {
        for (int j = i+1; j <= n; j++) {
            if (rand() < p * RAND_MAX)
                make_random_edge(i, j);
            if (rand() < p * RAND_MAX)
                make_random_edge(j, i);
        }
    }
}

// Accessors //
unordered_set<int> IntGraph::get_nodes(bool aux) const {
    if (aux) {
        return nodes;
    } else {
        unordered_set<int> ans = nodes;
        ans.erase(0);
        return ans;
    }
}
vector<tuple<int, int>> IntGraph::get_nodes_and_storage(bool aux) const {
    vector<tuple<int, int>> ans;
    if (aux) {
        ans.emplace_back(0,0);
    }
    for (int v : nodes) {
        if (v == 0) {
            ans.emplace_back(0, 0);
        } else {
            ans.emplace_back(v, outNeighbors.at(0).at(v).storage);
        }
    }
    return ans;
}

unordered_map<int, edge_variables> IntGraph::get_in_edges_of(int v,bool aux) const {
    if (inNeighbors.find(v) == inNeighbors.end())
        throw std::invalid_argument("input v of get_in_edges_of() is invalid. v=" + to_string(v));
    unordered_map<int, edge_variables> output = inNeighbors.at(v);
    if (!aux){
        output.erase(0);
    }
    return output;
}
const unordered_map<int, edge_variables>& IntGraph::get_out_edges_of(int v) const {
    if (outNeighbors.find(v) == outNeighbors.end())
        throw std::invalid_argument("input v of get_out_edges_of() is invalid. v=" + to_string(v));
    return outNeighbors.at(v);
}
vector<tuple<int, int, edge_variables>> IntGraph::get_edges(bool aux) const {
    vector<tuple<int, int, edge_variables>> output;
    auto valid_nodes = get_nodes(aux);

    for (const auto& u : valid_nodes){
        for (const auto& e : outNeighbors.at(u)){
            int v = get<0>(e);
            edge_variables costs = get<1>(e);
            output.emplace_back(u,v,costs);
        }
    }
    return output;
}
int IntGraph::size(bool aux) const {
    if (aux)
        return n+1;
    return n;
}
vector<int> IntGraph::get_in_neighbors_of(int v, bool aux) const {
    vector<int> output;
    output.reserve(inNeighbors.at(v).size());
    for (const auto &u: inNeighbors.at(v)){
        if(aux || u.first != 0){
            output.push_back(u.first);
        }
    }
    return output;
}
vector<int> IntGraph::get_out_neighbors_of(int v) const {
    vector<int> output;
    output.reserve(outNeighbors.at(v).size());
    for (const auto &u: outNeighbors.at(v)){
        output.push_back(u.first);
    }
    return output;
}


// Modifiers
vector<int> IntGraph::add_node(int node_count) {
    int i = 1;
    vector<int> output;
    while (node_count != 0){
        if (nodes.find(i) == nodes.end()) {
            make_node(i);
            make_edge(0,i,{0,0});
            output.push_back(i);
            node_count --;
        }
        i++;
    }
    return output;
}
vector<int> IntGraph::add_node_with_storage(const vector<tuple<int, int>>& vertices) {
    vector<int> output;
    for (auto vertex : vertices){
        int v = get<0>(vertex);
        int storage = get<1>(vertex);
        make_node(v);
        make_edge(0,v,{storage,0});
        output.push_back(v);
    }
    return output;
}
void IntGraph::add_or_modify_edge(int u, int v, edge_variables costs, bool new_node) {
    if (u == 0 and v == 0) {
        return;
    }
    if (u == v) {
        throw invalid_argument("Self-loop error in add_or_modify_edge.");
    }
    if (new_node) {
        if (nodes.find(u) == nodes.end())
            make_node(u);
        if (nodes.find(v) == nodes.end())
            make_node(v);
    } else if (nodes.find(u) == nodes.end() or nodes.find(v) == nodes.end()) {
        throw invalid_argument("Nodes cannot be found in add_or_modify_edge.");
    }
    make_edge(u,v,costs);
}
void IntGraph::delete_edge(int u, int v) {
    auto cond1 = nodes.find(v) != nodes.end() && inNeighbors[v].find(u) != inNeighbors[v].end();
    auto cond2 = nodes.find(u) != nodes.end() && outNeighbors[u].find(v) != outNeighbors[u].end();
    if (cond1 != cond2) {
        throw logic_error("Error with symmetry for inputs in delete_edge");
    } else if (cond1) {
        inNeighbors[v].erase(u);
        outNeighbors[u].erase(v);
    }
}
void IntGraph::delete_node(int v) {
    if (nodes.find(v) == nodes.end()) {
        throw invalid_argument("Nodes cannot be found in delete_node.");
    }
    for (const auto& u : nodes) {
        delete_edge(u, v);
        delete_edge(v, u);
    }
    inNeighbors.erase(v);
    outNeighbors.erase(v);
    nodes.erase(v);
    n --;
}
unordered_map<int, edge_variables> IntGraph::operator[](int u) const {
    return get_out_edges_of(u);
}
void IntGraph::get_dependency_count_and_retrieval_cost(unordered_map<int, int>& dependency_count,
                                             unordered_map<int, long long>& retrieval_cost) const {
    for (auto v : get_nodes(false)) {
        dependency_count[v] = 1;
    }
    // 1. Push element into stack in bfs order, and calculate their retrieval cost along the way.
    vector<int> stack{0};
    retrieval_cost[0] = dependency_count[0] = 0;
    size_t ind = 0;
    while (ind < stack.size()) {
        auto children = get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            stack.push_back(child);
            retrieval_cost[child] = retrieval_cost[stack[ind]] + (*this)[stack[ind]][child].retrieval;
        }
        ind++;
    }

    // 2. Go backwards in stack and note the number of descendents
    ind = stack.size() - 1;
    while (ind > 0) {
        auto &dep = dependency_count[stack[ind]];
        auto children = get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            dep += dependency_count[child];
        }
        ind--;
    }
}
void IntGraph::get_dependency_list_and_retrieval_cost(unordered_map<int, unordered_set<int>>& dependency_list,
                                            unordered_map<int, long long>& retrieval_cost) const {
    for (auto v : get_nodes(false)) {
        dependency_list[v].insert(v);
    }
    // 1. Push element into stack in bfs order, and calculate their retrieval cost along the way.
    vector<int> stack{0};
    retrieval_cost[0] = 0;
    int ind = 0;
    while (ind < stack.size()) {
        auto children = get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            stack.push_back(child);
            retrieval_cost[child] = retrieval_cost[stack[ind]] + (*this)[stack[ind]][child].retrieval;
        }
        ind++;
    }

    // 2. Go backwards in stack and note the number of descendents
    ind = stack.size() - 1;
    while (ind >= 0) {
        auto &dep = dependency_list[stack[ind]];
        auto children = get_out_neighbors_of(stack[ind]);
        for (auto child : children) {
            dep.insert(dependency_list[child].begin(), dependency_list[child].end());
        }
        ind--;
    }
}
long long IntGraph::get_total_storage_cost() const {
    long long ans = 0;
    auto edges = get_edges(true);
    for (const auto &[u,v,w] : edges) {
        ans += w.storage;
    }
    return ans;
}
long long IntGraph::get_total_retrieval_cost() const {
    unordered_map<int, int> dependency_count;
    unordered_map<int, long long> retrieval_cost;
    get_dependency_count_and_retrieval_cost(dependency_count, retrieval_cost);
    long long ans = 0;
    for (int v : nodes) {
        ans += retrieval_cost[v];
    }
    return ans;
}
vector<int> IntGraph::get_nodes_in_bfs_order(int r) const {//TODO: not tested
    vector<int> pq = {r};
    vector<bool> visited(nodes.size(), false);
    int ind = 0;
    while (ind < pq.size()) {
        for (const auto &child : get_out_neighbors_of(pq[ind]))
            if (not visited[child]) {
                pq.push_back(child);
                visited[child] = true;
            }
        ind++;
    }
    return std::move(pq);
}

long long IntGraph::get_max_storage_cost() const {
    long long ans = 0;
    for (const auto &v : get_nodes(false)) {
        ans += outNeighbors.at(0).at(v).storage;
    }
    return ans;
}

long long IntGraph::get_max_retrieval_cost() const {
    unordered_map<int, int> dependency_count;
    unordered_map<int, long long> retrieval_cost;
    get_dependency_count_and_retrieval_cost(dependency_count, retrieval_cost);
    long long ans = INT64_MIN;
    for (const auto &v : nodes) {
        ans = std::max(ans, retrieval_cost[v]);
    }
    return ans;
}

IntGraph MST(const IntGraph& G) {
    auto edges = G.get_edges(true);
    auto m = edges.size();

    /// Calculating MST the fast way
    arbok::Gabow alg(G.size(true), m);
    for (const auto& [u,v,w]:edges){// MST uses storage cost as weight
        alg.create_edge(u,v,w.storage);
    }
    alg.run(0);
    auto edge_indices = alg.reconstruct(0);

    vector<tuple<int, int, edge_variables>> MST_edges;
    MST_edges.reserve(edge_indices.size());
    for (const auto& ind : edge_indices) {
        MST_edges.push_back(edges[ind]);
    }
    return IntGraph(MST_edges);
}

IntGraph MST_with_designated_root(const IntGraph &G, int r) {//TODO: not tested
    auto edges = G.get_edges(false);
    auto m = edges.size();

    /// Calculating MST the fast way
    arbok::Gabow alg(G.size(false), m);
    for (const auto& [u,v,w] : edges){
        alg.create_edge(u-1, v-1, w.storage+w.retrieval);
    }
    alg.run(r-1);
    auto edge_indices = alg.reconstruct(r-1);

    vector<tuple<int, int, edge_variables>> MST_edges;
    MST_edges.reserve(edge_indices.size());
    for (auto ind : edge_indices) {
        MST_edges.push_back(edges[ind]);
    }
    IntGraph H(MST_edges);// H has no auxiliary root
    for (auto v : H.get_nodes(false)) {
        H.add_or_modify_edge(0, v, G[0][v]);
    }
    return H;
}

IntGraph Symmetric_MST_with_min_weight(const IntGraph &G, int r) {//TODO: this may improve the choice of tree.
    IntGraph GG;
    for (const auto &[u,v,w] : G.get_edges(false)) {
        edge_variables new_edge{std::min(w.storage, G[v][u].storage), std::min(w.retrieval, G[v][u].retrieval)};
        GG.add_or_modify_edge(u, v, new_edge, true);
        GG.add_or_modify_edge(v, u, new_edge);
    }
    for (const auto &v : G.get_nodes(false))
        GG.add_or_modify_edge(0, v, G[0][v]);
    auto MM = MST_with_designated_root(GG, r);

    cout << MM.size(false) << " " << MM.get_edges(false).size() << endl;

    IntGraph ans;
    for (const auto &[u,v,w] : MM.get_edges(false)) {
        ans.add_or_modify_edge(u, v, G[u][v], true);
        ans.add_or_modify_edge(v, u, G[v][u]);
    }
    for (const auto &v : G.get_nodes(false))
        ans.add_or_modify_edge(0, v, G[0][v]);
    cout << ans.size(false) << " " << ans.get_edges(false).size() << endl;
    return ans;
}

bool IntGraph::is_valid_solution() const {
    for (const auto &v : nodes) {
        if (inNeighbors.at(v).size() != 1)
            return false;
    }
    return true;
}

bool IntGraph::is_arborescence(int r) const {
    if (r == 0) {
        for (const auto &v : nodes) {
            if (v != 0 and inNeighbors.at(v).size() != 1)
                return false;
        }
    } else {
        for (const auto &v: nodes) {
            if (v != 0 and inNeighbors.at(v).size() != 2 and v != r)
                return false;
        }
    }

    return true;
}
