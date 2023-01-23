//
// Created by Anxin Guo on 1/19/23.
//

#ifndef GRAPH_VERSIONING_OPT_H
#define GRAPH_VERSIONING_OPT_H
#include "gurobi_c++.h"
#include "Graph.hpp"
using namespace std;

/**
 *
 * @param G: IntGraph, a version graph.
 * @param S: int, stroage constraint.
 * @return Soln: a subgrpah of G consisted of the chosen edges. (including edges like (0,v))
 */
IntGraph prob3_ILP(const IntGraph &G, int S);

#endif //GRAPH_VERSIONING_OPT_H
