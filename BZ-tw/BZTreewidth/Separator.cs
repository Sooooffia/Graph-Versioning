using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    partial class Graph
    {
        // Attempt to branch on a separator
        public Graph BranchOnSeparator()
        {
            // Attempt to branch on a separator
            // Theorem: a graph of treewidth k has a 0.5(n-k) separator of size at most k+1

            int bestSeparatorSize = vertices.Count;
            List<Vertex> bestSeparator = new List<Vertex>(vertices.Keys);

            List<List<Vertex>> separators = new List<List<Vertex>>();
            foreach (Vertex v in vertices.Keys) // Initialization: all vertex subsets of size 1
                separators.Add(new List<Vertex>(new Vertex[] { v }));

            int lower = Program.BagsList.Count > 0 ? Program.BagsList.Max((b) => b.Count) : 0;

            // We enumerate over all vertex subsets of increasing size, and check whether they form a balanced separator (n choose k options)
            // If so, we branch on removing this separator
            // We consider all subsets of size k-1, and extend it to a subset of size k by adding an articulation point
            // This reduces the number of points we have to consider (in the last step) greatly
            // One caveat: if a component was off by 1 from the upper, limit, we have to consider all possibilities for this component
            for (int i = 2; i < bestSeparatorSize - 1; i++)
            {
                List<List<Vertex>> newSeparators = new List<List<Vertex>>();
                foreach (List<Vertex> separator in separators) // Enumerate over separators of size i-1
                    foreach (Vertex v in vertices.Keys)
                    {
                        if (v.Label <= separator[separator.Count - 1].Label) continue; // Optimization: only consider separators where the vertices are in increasing order

                        List<Vertex> newSeparator = new List<Vertex>(separator);
                        newSeparator.Add(v);
                        newSeparators.Add(newSeparator);

                        // Copy the graph, remove potential separator
                        List<Vertex> tempVertices = vertices.Keys.ToList();
                        foreach (Vertex u in newSeparator)
                            tempVertices.Remove(u);

                        // Consider the possible articulation points that can extend this set to a separator
                        // OR vertices of components that are just marginally too large
                        IEnumerable<Vertex> aps = new Graph(tempVertices).ArticulationPointsOrComponentSize((vertices.Count - newSeparator.Count + 2) / 2);

                        // Try each one as a possible separator
                        foreach (Vertex ap in aps)
                        {
                            if (ap.Label <= separator[separator.Count - 1].Label) continue; // This one will be generated some other way as well

                            Program.SeparatorTried++;

                            newSeparator.Add(ap);
                            int oldBagCount = Program.BagsList.Count;

                            // There is (guaranteed) a balanced separator (biggest component at most (n-k+1)/2 vertices)
                            // We have to round this number up because of the adj-trick
                            Graph candidateDecomposition = RemoveSeparator(newSeparator, (vertices.Count - newSeparator.Count + 1) / 2);
                            if (candidateDecomposition != null)
                            {
                                int candidateWidth = Program.BagsList.Max((b) => b.Count);

                                if (candidateWidth == newSeparator.Count || candidateWidth <= lower)
                                    return RemoveSeparator(newSeparator);

                                if (candidateWidth < bestSeparatorSize)
                                {
                                    bestSeparatorSize = candidateWidth;
                                    bestSeparator = new List<Vertex>(newSeparator);
                                }
                            }

                            Program.BagsList.RemoveRange(oldBagCount, Program.BagsList.Count - oldBagCount);
                            newSeparator.RemoveAt(newSeparator.Count - 1);

                            if (newSeparator.Count + 1 == bestSeparatorSize)
                                return RemoveSeparator(bestSeparator);

#if LIMIT_SEPARATOR
                            if(Program.TotalExpanded > 30000000 || Program.SeparatorTried > 12000000) return null; // Apparently finding a separator in this graph is really hard, don't know how to solve, just fall back to DFS/DP and hope for the best
#endif
                        }
                    }

                separators = newSeparators;
            }

            return RemoveSeparator(bestSeparator);
        }

        // Removes a separator from the graph, and recursively computes a tree decomposition. The separator should be safe.
        public Graph RemoveSeparator(List<Vertex> separator)
        {
            return RemoveSeparator(separator, false);
        }

        // Removes a separator from the graph, and recursively computes a tree decomposition. The separator should be safe.
        // If check3isolated is true, then this also checks the property that given a separator of size 3, it should not split off any isolated vertices (or else it is not safe).
        public Graph RemoveSeparator(List<Vertex> separator, bool check3isolated)
        {
            return RemoveSeparator(separator, check3isolated, vertices.Count);
        }

        // Removes a separator from the graph, and recursively computes a tree decomposition. The separator should be safe.
        // If any connected component has more than maximumSize vertices, do not proceed (balanced separator check)
        public Graph RemoveSeparator(List<Vertex> separator, int maximumSize)
        {
            return RemoveSeparator(separator, false, maximumSize);
        }

        // Removes a separator from the graph, and recursively computes a tree decomposition. The separator should be safe.
        // If check3isolated is true, then this also checks the property that given a separator of size 3, it should not split off any isolated vertices (or else it is not safe).
        // If any connected component has more than maximumSize vertices, do not proceed (balanced separator check)
        public Graph RemoveSeparator(List<Vertex> separator, bool check3isolated, int maximumSize)
        {
            // Compute the graph after removing separator
            List<Vertex> tempVertices = vertices.Keys.ToList();
            foreach (Vertex v in separator)
                tempVertices.Remove(v);

            // Get the connected components
            Graph componentsGraph = new Graph(tempVertices);
            List<List<Vertex>> components = componentsGraph.getComponents();

            // If applicable, check that it is safe
            if (check3isolated && components.Any((c) => c.Count == 1))
                return null;

            // Check balance
            if (components.Any((c) => c.Count > maximumSize))
                return null;

            // Prepare the new tree decomposition: the root bag contains the separator
            Graph result = new Graph();
            Program.BagsList.Add(new List<Vertex>(separator));
            Vertex root = result.AddVertex(Program.BagsList.Count - 1);

            // Decompose each component
            foreach (List<Vertex> component in components)
            {
                // Create a new graph with the component in it
                Graph componentGraph = new Graph(component);

                // Add the separator
                foreach (Vertex v in separator)
                    componentGraph.AddVertex(v);

                foreach (Vertex u in separator)
                {
                    // Make the separator a clique
                    foreach (Vertex v in separator)
                    {
                        if (u.Label <= v.Label) // Not strictly necessary, but might save some time
                            continue;
                        componentGraph.AddEdge(u, v);
                    }
                    // And add edges from the separator to the rest
                    foreach (Vertex v in u.Adj)
                        componentGraph.AddEdge(u, v);
                }

                // Compute the TD of the component
                Graph TDComponent = componentGraph.Decompose();
                result.AddGraph(TDComponent);

                // Find an appropriate bag in the decomposition to connect our root bag to
                foreach (Vertex v in TDComponent.vertices.Keys)
                {
                    if (separator.All((u) => { return Program.BagsList[v.Label].Contains(u); }))
                    {
                        result.AddEdge(root, v);
                        break;
                    }
                }
            }

            return result;
        }
    }
}
