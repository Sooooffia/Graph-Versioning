#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include "test_DP.h"
#include "GraphIO.h"
#include "script_LMG_all.h"

using std::cout;
int main() {
//    test_graph_construction();
//    test_LMG_all();
//    test_LMG_with_OPT();
//    try_DP_on_rg();
//    test_make_binary();

//    LMG_vs_LMGA_on_git_graph("LeetCodeAnimation");
//    LMG_vs_LMGA_on_git_graph("styleguide");
//    LMG_vs_LMGA_on_git_graph("gitignore");

    test_DP_on_git_graph("styleguide", 0.1);
    return 0;
}
