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

//    LMG_vs_LMGA_on_git_graph("datasharing");//246 nodes and 628 edges
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation");//246 nodes and 628 edges
//    LMG_vs_LMGA_on_git_graph("styleguide"); // 493 nodes and 1250 edges
//    LMG_vs_LMGA_on_git_graph("gitignore"); // 3526 nodes and 9856 edges
////    LMG_vs_LMGA_on_git_graph("SmartThingsPublic"); // 5148 nodes and 15044 edges
//    LMG_vs_LMGA_on_git_graph("996.ICU"); // 3189 nodes and 9210 edges
////    LMG_vs_LMGA_on_git_graph("ChatGPT"); // 451 nodes and 1090 edges
//    LMG_vs_LMGA_on_git_graph("freeCodeCamp"); // 31270 nodes and 71534 edges
////    LMG_vs_LMGA_on_git_graph("styleguide(RG, p=0.05)"); // 493 nodes and 12040 edges
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=0.05)"); // 246 nodes and 3032 edges
////    LMG_vs_LMGA_on_git_graph("styleguide(RG, p=0.2)"); // 493 nodes and 48450 edges
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=0.2)"); // 246 nodes and 11932 edges
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=1)"); // 246 nodes and 11932 edges

//    test_DP_on_git_graph("datasharing", 0.005, true);
//    test_DP_on_git_graph("SmartThingsPublic", 0.02, true);
//    test_DP_on_git_graph("996.ICU", 0.02, true);
//    test_DP_on_git_graph("gitignore", 0.05, true);
//    test_DP_on_git_graph("styleguide", 0.01, true);
//    test_DP_on_git_graph("LeetCodeAnimation", 0.01, true);
//    test_DP_on_git_graph("styleguide(RG, p=0.05)", 0.01, true);
//    test_DP_on_git_graph("LeetCodeAnimation(RG, p=0.05)", 0.01, true);
//    test_DP_on_git_graph("styleguide(RG, p=0.2)", 0.01, true);
//    test_DP_on_git_graph("LeetCodeAnimation(RG, p=0.2)", 0.01, true);
//    test_DP_on_git_graph("styleguide(RG, p=1)", 0.05);
//    test_DP_on_git_graph("LeetCodeAnimation(RG, p=1)", 0.05);
//    test_DP_on_git_graph("ChatGPT", 0.01, true);
//    test_DP_on_git_graph("freeCodeCamp", 0.1, true);


//    test_OPT_on_git_grpah("LeetCodeAnimation");
//    test_OPT_on_git_grpah("styleguide");
//    test_OPT_on_git_grpah("gitignore");

//    test_MP_vs_DP_on_git_graph("datasharing", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation", 200);
//    test_MP_vs_DP_on_git_graph("styleguide", 300);
    test_MP_vs_DP_on_git_graph("gitignore", 200);
    test_MP_vs_DP_on_git_graph("996.ICU", 200);
    test_MP_vs_DP_on_git_graph("freeCodeCamp", 200);
//    test_MP_vs_DP_on_git_graph("styleguide(RG, p=0.05)", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation(RG, p=0.05)", 200);
//    test_MP_vs_DP_on_git_graph("styleguide(RG, p=0.2)", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation(RG, p=0.2)", 200);
//    test_MP_vs_DP_on_git_graph("styleguide(RG, p=1)", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation(RG, p=1)", 200);
//
//    LMG_vs_LMGA_on_git_graph("styleguide(RG, p=1)"); // 493 nodes and 242556 edges
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=1)"); // 246 nodes and 60270 edges

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
