using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    partial class Graph
    {
        // Recursively computes a tree decomposition
        // Tries several reduction rules (connected components, size 1,2,3 separators, almost clique separators)
        public Graph Decompose()
        {
            Graph result = new Graph();

            // Most simple case: the graph is disconnected
            getComponents();
            if (components.Count > 1)
            {
                Vertex prevVertex = null;
                foreach (List<Vertex> component in components)
                {
                    Graph componentGraph = new Graph(component);
                    Graph TDComponent = componentGraph.Decompose();

                    // Combine TDs into path (any way that makes the components connected is OK
                    result.AddGraph(TDComponent);
                    Vertex TDVertex = TDComponent.vertices.First().Value;
                    if (prevVertex != null)
                        result.AddEdge(TDVertex, prevVertex);
                    prevVertex = TDVertex;
                }

                return result;
            }

            // Biconnected graph: An articulation point is always safe (single vertex separator)
            Vertex ap = ArticulationPoint();
            if (ap != null)
                return RemoveSeparator(new List<Vertex>(new Vertex[] { ap }));

            // 3-connected graph: try to find an articulation pair by removing a vertex, then testing if some other vertex is an articulation point
            // These are always safe
            // Note that thia can be done more efficiently, but this is easier and fast enough for reasonable graphs
            foreach (Vertex v1 in vertices.Keys)
            {
                List<Vertex> tempVertices = vertices.Keys.ToList();
                tempVertices.Remove(v1);
                Vertex v2 = new Graph(tempVertices).ArticulationPoint();
                if (v2 != null) // Found an articulation pair (v1, v2)
                    return RemoveSeparator(new List<Vertex>(new Vertex[] { v1, v2 }));
            }

            // 4-connected graph: same idea, remove pair of vertices, then find an articulation point
            // Again quite inefficient (cubic) but we've already done a lot of reduction so the graph is hopefully small at this point
            foreach (Vertex v1 in vertices.Keys)
            {
                foreach (Vertex v2 in vertices.Keys)
                {
                    if (v1.Label >= v2.Label) continue;

                    List<Vertex> tempVertices = vertices.Keys.ToList();
                    tempVertices.Remove(v1);
                    tempVertices.Remove(v2);
                    Vertex v3 = new Graph(tempVertices).ArticulationPoint();
                    if (v3 != null) // Found a separator (v1, v2, v3)
                    {
                        // RemoveSeparator checks that this is a safe separator (not all 3-vertex separators are safe) and returns null if not
                        Graph temp = RemoveSeparator(new List<Vertex>(new Vertex[] { v1, v2, v3 }), true);
                        if (temp != null)
                            return temp;
                    }
                }
            }

            // Our final reduction rule: a minimal separator that is a clique or almost clique (a clique plus a vertex) is safe
            List<List<Vertex>> cliques = new List<List<Vertex>>();
            foreach (Vertex v in vertices.Keys) // Initialization: all vertices cliques of size 1
                cliques.Add(new List<Vertex>(new Vertex[] { v }));

            // Enumerate all cliques; we don't need to consider cliques that are as large as the entire graph
            for (int i = 2; i < vertices.Count - 1; i++)
            {
                List<List<Vertex>> newCliques = new List<List<Vertex>>();
                foreach (List<Vertex> clique in cliques) // Enumerate over cliques of size i-1
                    foreach (Vertex v in vertices.Keys)
                    {
                        if (v.Label <= clique[clique.Count - 1].Label) continue; // Optimization: only consider cliques where the vertices are in increasing order

                        if (clique.All((u) => { return edges.Contains(new Edge(u, v)); })) // Can we add the vertex (is it connected to everything else)?
                        {
                            // Found a new clique
                            List<Vertex> newClique = new List<Vertex>(clique);
                            newClique.Add(v);
                            newCliques.Add(newClique);

                            // Copy the graph, remove a clique
                            List<Vertex> tempVertices = vertices.Keys.ToList();
                            foreach (Vertex u in newClique)
                                tempVertices.Remove(u);

                            // If the graph minus the clique has an AP, the clique plus the AP is an (almost) clique minimal separator
                            Vertex v1 = new Graph(tempVertices).ArticulationPoint();
                            if (v1 != null) // Found an (almost) clique separator
                            {
                                newClique.Add(v1);
                                return RemoveSeparator(newClique);
                            }
                        }
                    }
                cliques = newCliques;

                // Limit the number of cases considered, as we want to limit the time and don't necessarily need to consider all of them
                cliques.Shuffle();
                cliques = cliques.Take(10000).ToList();
            }

            // No more reduction rules apply: we now try to find a tree decomposition using a 2^n DP algorithm
            return ComputeTD();
        }

        // No more preprocessing rules apply, use branching or DP
        public Graph ComputeTD()
        {
            // Graph is too big and not dense enough. Try the separator technique
            if (vertices.Count >= 25 && edges.Count < 5 * vertices.Count)
            {
                Graph result = BranchOnSeparator();
                if (result != null)
                    return result;
            }
         
#if USE_DFS
            return FastGraph.DFSInit(this);
#else
            return TreewidthDP();
#endif
        }
    }
}
