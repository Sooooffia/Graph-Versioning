using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    partial class Graph
    {
        // Computes a tree decomposition using the 2^n dynamic programming algorithm
        // Note: since this uses longs to represent partial results, only works for graphs up to 64 vertices
        public Graph TreewidthDP()
        {
            // Fall back on DFS
            if (vertices.Count > 64)
                return FastGraph.DFSInit(this);

            List<Vertex> atTheEnd = new List<Vertex>();

            // Begin by finding a maximum clique, which should be the final bag.
            List<List<Vertex>> cliques = new List<List<Vertex>>();
            foreach (Vertex v in vertices.Keys) // Initialization: all vertices cliques of size 1
                cliques.Add(new List<Vertex>(new Vertex[] { v }));

            // Enumerate all cliques
            for (int i = 2; i <= vertices.Count; i++)
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
                            atTheEnd = newClique;
                        }
                    }
                if (newCliques.Count == 0)
                    break;
                cliques = newCliques;

                // Limit the number of cases considered, as we want to limit the time and don't necessarily need to consider all of them
                cliques.Shuffle();
                cliques = cliques.Take(5000).ToList();
            }

            int width;
            // We use an iterative deepening approach: we start with the smallest possible width, and increase it by 1 until we find a width that fits
            // This simplifies the DP somewhat
            for (width = Math.Min(vertices.Count - 1, Math.Max(3, Program.BagsList.Count > 0 ? Program.BagsList.Max((b) => b.Count) - 1 : 0)); width < vertices.Count; width++)
            {
                atTheEnd = DPDecompose(width, cliques[0]);
                if (atTheEnd != null) // We know that width is the minimum width possible
                    break;
            }

            // Now compute the permutation that actually gives this width
            while (atTheEnd.Count < vertices.Count)
                DPDecompose(width, atTheEnd);

            return TDFromEliminationOrder(atTheEnd);
        }

        // Uses DP to find a decomposition with bag size at most upper
        public List<Vertex> DPDecompose(int upper, List<Vertex> atTheEnd)
        {
            // Make a list of the original labels
            List<int> vertexLabels = new List<int>();
            foreach (Vertex v in vertices.Keys)
                vertexLabels.Add(v.Label);

            // Replace the labels with 0...n-1
            Graph relabeledGraph = new Graph();
            foreach (Vertex v in vertices.Keys)
                relabeledGraph.AddVertex(vertexLabels.IndexOf(v.Label));

            // Copy the edges (using the relabeled vertices)
            foreach (Edge e in edges)
                relabeledGraph.AddEdge(new Vertex(vertexLabels.IndexOf(e.a.Label)), new Vertex(vertexLabels.IndexOf(e.b.Label)));

            // We know which vertices should go at the end
            List<Vertex> possibleVertices = new List<Vertex>(relabeledGraph.vertices.Keys.ToArray());
            foreach (Vertex v in atTheEnd)
                possibleVertices.Remove(new Vertex(vertexLabels.IndexOf(v.Label)));

            // Initialize the table: just the empty set
            HashSet<ulong> table = new HashSet<ulong>();
            table.Add(0);

            // Once we have processed n - upper - 1 vertices, in the next step there are only upper vertices not in S, and thus we always meet the restrictions
            for (int i = 0; i < Math.Min(vertices.Count - upper - 1, vertices.Count - atTheEnd.Count - 1); i++)
            {
                bool islast = i == Math.Min(vertices.Count - upper - 1, vertices.Count - atTheEnd.Count - 1) - 1;
                HashSet<ulong> newTable = new HashSet<ulong>();
#if SEQUENTIAL
                foreach(ulong S in table)
                {
                    if (islast && newTable.Count > 0) break;

                    // Try to add each vertex
                    foreach (Vertex x in possibleVertices)
                    {
                        ulong newS = S | (1UL << x.Label);
                        if (newS == S || Q(newS, x) > upper) continue; // We either have already seen this vertex, or it would be too expensive
                        newTable.Add(newS);
                    }
                }           
#else
                // Attempt to get parallelism working on the test system
                System.Threading.ThreadPool.SetMaxThreads(48 * 5, 48 * 5);
                System.Threading.ThreadPool.SetMinThreads(8, 8);
                Parallel.ForEach(table, () => new List<ulong>(), (S, loopState, partialResult) =>
                {
                    if (islast && (newTable.Count > 0 || partialResult.Count > 0)) return partialResult;

                    // Try to add each vertex
                    foreach (Vertex x in possibleVertices)
                    {
                        ulong newS = S | (1UL << x.Label);
                        if (newS == S || Q(newS, x) > upper) continue; // We either have already seen this vertex, or it would be too expensive
                        partialResult.Add(newS);
                    }

                    return partialResult;
                }, (localResult) => { lock (newTable) { foreach (ulong l in localResult) newTable.Add(l); } });
             
#endif
                table = newTable;
                Program.TotalExpanded += table.Count;
            }

            // If there are any entries in the table, we know it's feasible
            if (table.Count > 0)
            {
                for (int i = 0; i < vertices.Count; i++)
                    if ((table.First() & (1UL << i)) == 0 && !atTheEnd.Contains(vertices[new Vertex(vertexLabels[i])]))
                        atTheEnd.Add(vertices[new Vertex(vertexLabels[i])]);
                return atTheEnd;
            }
            else
                return null;
        }

        [ThreadStatic]
        static Stack<Vertex> stack;

        // Computes the number of vertices of S reachable from x, using only paths in V-S (DFS)
        public int Q(ulong S, Vertex x)
        {
            ulong visited = 0; int result = 0;
            if (stack == null) stack = new Stack<Vertex>();
            stack.Clear();
            visited |= (1UL << x.Label);
            stack.Push(x);

            while (stack.Count > 0)
            {
                Vertex cur = stack.Pop();
                foreach (Vertex nb in cur.FAdj)
                    if ((visited & (1UL << nb.Label)) == 0)
                    {
                        visited |= 1UL << nb.Label;
                        if ((S & (1UL << nb.Label)) == 0)
                            result++;
                        else
                            stack.Push(nb);
                    }
            }

            return result;
        }

        // Determine whether two vertices are adjacent in the graph that results from eliminating vertices in S
        public bool IsAdj(ulong S, Vertex x, Vertex y)
        {
            ulong visited = 0;
            if (stack == null) stack = new Stack<Vertex>();
            stack.Clear();
            visited |= (1UL << x.Label);
            stack.Push(x);

            while (stack.Count > 0)
            {
                Vertex cur = stack.Pop();
                foreach (Vertex nb in cur.Adj)
                    if ((visited & (1UL << nb.Label)) == 0)
                    {
                        visited |= 1UL << nb.Label;
                        if ((S & (1UL << nb.Label)) == 0)
                        {
                            if (nb == y) return true;
                        }
                        else
                            stack.Push(nb);
                    }
            }

            return false;
        }

        // Takes an elimination order, and produces a corresponding tree decomposition
        public Graph TDFromEliminationOrder(List<Vertex> atTheEnd)
        {
            // Finally, turn the permutation into a graph
            Graph result = new Graph();

            Program.BagsList.Add(new List<Vertex>(new Vertex[] { atTheEnd[0] }));
            result.AddVertex(Program.BagsList.Count - 1);

            for (int i = 1; i < atTheEnd.Count; i++)
            {
                // Figure out what vertices to the left in the permutation we can reach without traversing those vertices themselves
                List<Vertex> reachableToTheLeft = new List<Vertex>();
                List<Vertex> visited = new List<Vertex>();
                Stack<Vertex> stack = new Stack<Vertex>();
                visited.Add(atTheEnd[i]);
                stack.Push(atTheEnd[i]);

                while (stack.Count > 0)
                {
                    Vertex cur = stack.Pop();
                    foreach (Vertex nb in cur.Adj)
                    {
                        if (!visited.Contains(nb))
                        {
                            visited.Add(nb);
                            if (atTheEnd.IndexOf(nb) < i)
                                reachableToTheLeft.Add(nb);
                            else
                                stack.Push(nb);
                        }
                    }
                }

                // Find a suitable parent (i.e. a bag that contains all the vertices that we can reach to the left)
                Vertex parent = null;
                foreach (Vertex v in result.vertices.Keys)
                {
                    if (reachableToTheLeft.All((u) => { return Program.BagsList[v.Label].Contains(u); }))
                    {
                        parent = v;
                        break;
                    }
                }

                // Add the bag and connecting edge
                reachableToTheLeft.Add(atTheEnd[i]);
                Program.BagsList.Add(reachableToTheLeft);
                result.AddEdge(result.AddVertex(Program.BagsList.Count - 1), parent);
            }

            return result;
        }
    }
}
