#include <iostream>
#include "Graph.hpp"
#include "tests.hpp"
using std::cout;

int main() {
    unordered_set<int> V{0, 1, 1};
    cout << V;
    test_graph_construction();
    return 0;
}
