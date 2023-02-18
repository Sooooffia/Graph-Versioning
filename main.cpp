#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include "test_DP.h"
#include "GraphIO.h"
#include "test_LMG_all.h"
#include "test_prob_6.h"

using std::cout;
int main() {
//    test_graph_construction();
//    test_LMG_all();
//    test_LMG_with_OPT();
//    try_DP_on_rg();
//    test_make_binary();

//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation");//246 nodes and 628 edges
//    LMG_vs_LMGA_on_git_graph("styleguide"); // 493 nodes and 1250 edges
//    LMG_vs_LMGA_on_git_graph("gitignore"); // 3526 nodes and 9856 edges

//    test_DP_on_git_graph("styleguide", 0.1);
//    test_DP_on_git_graph("LeetCodeAnimation", 0.05);
//    test_DP_on_git_graph("gitignore", 0.05);

//    test_OPT_on_git_grpah("LeetCodeAnimation");
//    test_OPT_on_git_grpah("styleguide");
//    test_OPT_on_git_grpah("gitignore");

//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation");
//    test_MP_vs_DP_on_git_graph("styleguide");
    test_MP_vs_DP_on_git_graph("gitignore", 200);

//    IntGraph G(3);
//    G.add_or_modify_edge(0, 3, {30,0});
//    G.add_or_modify_edge(0, 1, {10,0});
//    G.add_or_modify_edge(0, 2, {20,0});
//
//    G.add_or_modify_edge(1, 2, {12,12});
//    G.add_or_modify_edge(2, 1, {21,21});
//    G.add_or_modify_edge(1, 3, {13,13});
//    G.add_or_modify_edge(3, 1, {31,31});
//    cout << Modified_Prim(G, 20).get_total_storage_cost() << endl;
//    cout << DP_BMR(G, 20) << endl;

    return 0;
}
