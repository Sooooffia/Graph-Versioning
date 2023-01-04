#include <iostream>
#include "Graph.hpp"
#include "tests.hpp"
#include "MST/gabow.h"
using std::cout;

int main() {
    IntGraph graph(4,0.5,1);
    auto edges = graph.get_edges(1);
    int m = edges.size();

    arbok::Gabow alg(5,m);

    for (auto e:edges){
        int u = get<0>(e), v = get<1>(e), w = get<2>(e).storage;
        alg.create_edge(u,v,w);
    }

    int root = 0;
    alg.run(root);
    auto res = alg.reconstruct(root);

    for(auto [u,v,w] : edges){
        std::cout << u << ' ' << v << ' ' << w << '\n';
    }

    std::cout <<'\n';

    for(int e_id : res){
        auto [u,v,w] = edges [e_id];
        std::cout << u << ' ' << v << ' ' << w << '\n';
    }

    return 0;
}
