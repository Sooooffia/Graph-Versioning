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

//    LMG_vs_LMGA_on_git_graph("datasharing-compressed");//29 nodes and 74 edges, mat=7672, sto=395
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation");//246 nodes and 628 edges, mat=1.737e+08, sto=1.19243e+07
//    LMG_vs_LMGA_on_git_graph("styleguide"); // 493 nodes and 1250 edges, mat=1.3568e+06, sto=8659
//    LMG_vs_LMGA_on_git_graph("996.ICU"); // 3189 nodes and 9210 edges, mat=1.54364e+07, sto=337038
//    LMG_vs_LMGA_on_git_graph("freeCodeCamp-compressed"); // 31270 nodes and 71534 edges, mat=2.47536e+07, sto=14800
/// freeCodeCamp not done yet
//    LMG_vs_LMGA_on_git_graph("styleguide(RG, p=0.05)"); // 493 nodes and 12040 edges
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=0.05)"); // 246 nodes and 3032 edges, mat=1.712e+08, sto=1.06751e+08
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=0.2)"); // 246 nodes and 11932 edges, mat=1.68149e+08, sto=1.06164e+08
//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation(RG, p=1)"); // 246 nodes and 60270 edges, mat=1.67895e+08, sto=1.04198e+08

//    test_DP_on_git_graph("datasharing", 0.05, true);
//    test_DP_on_git_graph("styleguide", 0.05, true);
//    test_DP_on_git_graph("LeetCodeAnimation", 0.05, true);
//    test_DP_on_git_graph("996.ICU", 0.05, true);
//    test_DP_on_git_graph("LeetCodeAnimation(RG, p=0.05)", 0.05, true);
//    test_DP_on_git_graph("LeetCodeAnimation(RG, p=0.2)", 0.05, true);
//    test_DP_on_git_graph("LeetCodeAnimation(RG, p=1)", 0.05, true);
//    test_DP_on_git_graph("freeCodeCamp", 0.1, true);
/// freeCodeCamp not done yet


//    test_OPT_on_git_grpah("LeetCodeAnimation");
//    test_OPT_on_git_grpah("styleguide");
//    test_OPT_on_git_grpah("gitignore");

//    test_MP_vs_DP_on_git_graph("datasharing", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation", 500);
//    test_MP_vs_DP_on_git_graph("styleguide", 500);
//    test_MP_vs_DP_on_git_graph("996.ICU", 500);
    test_MP_vs_DP_on_git_graph("freeCodeCamp-compressed", 200);
/// freeCodeCamp not done yet
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation(RG, p=0.05)", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation(RG, p=0.2)", 200);
//    test_MP_vs_DP_on_git_graph("LeetCodeAnimation(RG, p=1)", 200);

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

//    write_graph_to_PACE("datasharing");
//    write_graph_to_PACE("styleguide");
//    write_graph_to_PACE("LeetCodeAnimation");
//    write_graph_to_PACE("gitignore");
//    write_graph_to_PACE("996.ICU");

//    write_compressed_graph("datasharing");
//    write_compressed_graph("styleguide");
//    write_compressed_graph("LeetCodeAnimation");
//    write_compressed_graph("LeetCodeAnimation(RG, p=0.05)");
//    write_compressed_graph("LeetCodeAnimation(RG, p=0.2)");
//    write_compressed_graph("LeetCodeAnimation(RG, p=1)");
//    write_compressed_graph("996.ICU");
//    write_compressed_graph("freeCodeCamp");
    return 0;
}
