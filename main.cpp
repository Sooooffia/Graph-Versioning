#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include "test_DP.h"
#include "GraphIO.h"
#include "test_LMG_all.h"
#include "test_OPT.h"

using std::cout;
int main() {
//    test_graph_construction();
//    test_LMG_all();
//    test_LMG_with_OPT();
//    try_DP_on_rg();
//    test_make_binary();

//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation");//246 nodes and 628 edges
//    LMG_vs_LMGA_on_git_graph("styleguide"); // 493 nodes and 1250 edges
//    LMG_vs_LMGA_on_git_graph("gitignore");

//    test_DP_on_git_graph("styleguide", 0.1);

    test_OPT_on_git_grpah("LeetCodeAnimation");
    test_OPT_on_git_grpah("styleguide");
    test_OPT_on_git_grpah("gitignore");
    return 0;
}
