#include "OPT.h"
#include <iostream>
#include <sstream>
using std::ostringstream;
IntGraph prob3_ILP(const IntGraph &G, int S) {
    auto edges = G.get_edges(true);
    // Create an environment
    GRBEnv env = GRBEnv(true);
    env.set("LogFile", "prob3_ILP.log");
    env.start();

    // Create an empty model
    GRBModel model = GRBModel(env);

    unordered_map<int, unordered_map<int, GRBVar>> x, I;
    unordered_map<int, GRBLinExpr> flowConstrLHS, flowConstrRHS; /// LHS = sum of in edges, RHS = sum of out edges.
    GRBLinExpr objExpr, storageConstrExpr;
    for (auto &[u,v,w] : edges) {
        ostringstream ssx, ssI;
        ssx << "x_" << u << "," << v;
        ssI << "I_" << u << "," << v;
        x[u][v] = model.addVar(0, G.size(false), 0, GRB_INTEGER, ssx.str());
        I[u][v] = model.addVar(0, 1, 0, GRB_BINARY, ssx.str());

        model.addConstr(x[u][v] <= I[u][v] * G.size(false)); // indicator constraint
        objExpr += w.retrieval * x[u][v]; // coeff in the objective
        storageConstrExpr += w.storage * I[u][v]; // coeff in the storage constraint
        flowConstrLHS[v] += x[u][v];
        if (u != 0)
            flowConstrRHS[u] += x[u][v];
    }

    // Storage Constraint
    model.addConstr(storageConstrExpr <= S, "Stroage Constraint");

    // Flow conservative constraint. We want LHS = RHS + 1
    for (int v : G.get_nodes(false)) {
        ostringstream ss;
        ss << "flow_" << v;
        model.addConstr(flowConstrLHS[v] == flowConstrRHS[v] + 1, ss.str());
    }

    // Set objective
    model.setObjective(objExpr, GRB_MINIMIZE);

    // Optimize model
    model.optimize();

    vector<tuple<int, int, edge_variables>> Soln_edges;
    for (auto &[u,v,w] : edges) {
        if (I[u][v].get(GRB_DoubleAttr_X) > 0)
            Soln_edges.emplace_back(u,v,w);
    }
    return IntGraph(Soln_edges);

}
