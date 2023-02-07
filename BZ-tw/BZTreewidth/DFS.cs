using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    // Stores data needed for one layer of DFS
    class DFSData
    {
        public List<FVertex> nogoods;
        public Dictionary<BitArray, FVertex[]> DVS = new Dictionary<BitArray, FVertex[]>();

        public DFSData(int vertexCount)
        {
            nogoods = new List<FVertex>(vertexCount);
        }
    }

    partial class FastGraph
    {
        public List<int> DFS(int upper, FVertex prev)
        {
            // Keep track of current elimination order
            if (prev != null)
            {
                currentOrder.RemoveRange(baseVertices.Length - vertexCount - 1, currentOrder.Count - (baseVertices.Length - vertexCount - 1));
                currentOrder.Add(prev);
            }

            // Apply DVS pruning rule
            currentLayer.DVS.Clear();
            if (!ComputeDVS(true))
                return null;
            ComputeDVS(false);

            // Use MMW or not
#if DFS_MMW
            int lb = MMW(upper);
#elif DFS_MMW_LIMITED
            int lb = baseVertices.Length - vertexCount < 7 ? MMW(upper) : 0;
#else
            int lb = 0;
#endif

            Program.TotalExpanded++; // Statistics
            //if (Program.TotalExpanded % 100000 == 1) Console.WriteLine(Program.TotalExpanded / 1000 + ",000 " + upper + " " + baseVertices.Length + " " + lb + " " + vertexCount + " " + currentLayer.DVS.Count());

            // Inreasible?
            if (lb > upper)
                return null;

            // Are we done because any order will work from this point?
            if (vertexCount <= upper)
                return Vertices.Select(v => v.Id).ToList();

            // Try to find the highest-degree (almost)-simplicial vertex
            FVertex simplicialVertex = null;
            foreach (FVertex v in Vertices)
            {
                if ((simplicialVertex == null || v.Degree > simplicialVertex.Degree) && (TestSimplicial(v) || TestAlmostSimplicial(v, upper)))
                {
                    simplicialVertex = v;
                    if (v.Degree >= upper)
                        return null;
                }
            }

            // We found a simplicial vertex
            if (simplicialVertex != null)
            {
                // Pruned due to nogoods
                if (currentLayer.nogoods.Contains(simplicialVertex))
                    return null;

                // A nogood vertex becomes good if we eliminate a vertex adjacent to it
                List<FVertex> newnogoods = nextLayer.nogoods;
                newnogoods.Clear();
                foreach (FVertex u in currentLayer.nogoods)
                    if (!IsEdge(u, simplicialVertex))
                        newnogoods.Add(u);

                // Eliminate and go deeper
                EliminateVertex(simplicialVertex);
                EdgeAddition(upper);
                List<int> result = DFS(upper, simplicialVertex);
                Uneliminate();

                // Feasible
                if (result != null)
                {
                    result.Add(simplicialVertex.Id);
                    return result;
                }

                // Not feasible
                return null;
            }

            // Branching!
            foreach (FVertex v in Vertices)
            {
                // Check nogoods, and we never need to eliminate two adjacent vertices subsequently (note that we can not apply this latter rule to prune simplicial vertices)
                if (currentLayer.nogoods.Contains(v) || (prev != null && IsEdge(prev, v))) continue;

                if (v.Degree < upper)
                {
                    // A nogood vertex becomes good if we eliminate a vertex adjacent to it
                    List<FVertex> newnogoods = nextLayer.nogoods;
                    newnogoods.Clear();
                    foreach (FVertex u in currentLayer.nogoods)
                        if (!IsEdge(u, v))
                            newnogoods.Add(u);

                    // Eliminate and go deeper
                    EliminateVertex(v);
                    EdgeAddition(upper);
                    List<int> result = DFS(upper, v);
                    Uneliminate();

                    // Found a solution
                    if (result != null)
                    {
                        result.Add(v.Id);
                        return result;
                    }

                    // In next layers, do not eliminate v before eliminating a neighbor of v
                    currentLayer.nogoods.Add(v);
                }
            }

            // We didn't manage to find a solution in this branch
            return null;
        }

        // Implements the edge addition heuristic
        public void EdgeAddition(int upper)
        {
#if DFS_EDGEADDITION
            UndoInfo temp = undoStack.Pop();

            bool repeat = true;
            while (repeat)
            {
                repeat = false;
                foreach (FVertex v1 in Vertices)
                {
                    if (v1.Degree < upper) continue;
                    foreach (FVertex v2 in Vertices)
                    {
                        if (v2.Degree < upper) continue;

                        if (v1.Id >= v2.Id || IsEdge(v1, v2)) continue;
                        if (CommonNB(v1, v2) >= upper)
                        {
                            if (!AddEdge(v1, v2)) throw new Exception();
                            undoStack.Push(new UndoInfo(v1.Id, v2.Id));
                            repeat = true;
                        }
                    }
                }
            }

            undoStack.Push(temp);
#endif
        }

        // Computes the minor-min-width LB heuristic
        public int MMW(int upper)
        {
            int oldCount = vertexCount;

            foreach (FVertex v in Vertices)
                degreeLists[v.Degree].Add(v.Id);
            int min = 0;

            int lb = upper;
            while (vertexCount > 0)
            {
                // We won't be able to find a better bound this way (max degree = upper - 1)
                if (vertexCount <= lb)
                    break;

                // We use a priority queue implemented by having a list for each priority, to find the smallest vertex more quickly
                // Idea due to Dow and Korf
                FVertex minDegree = null;
                while (true)
                {
                    while (degreeLists[min].Count == 0)
                        min++;
                    minDegree = baseVertices[degreeLists[min][degreeLists[min].Count - 1]];
                    degreeLists[min].RemoveAt(degreeLists[min].Count - 1);
                    if (minDegree.Degree == min && IsVertex(minDegree))
                        break;
                }

                lb = Math.Max(lb, minDegree.Degree + 1);

                // We already found a good enough bound
                if (lb > upper)
                    break;

                // Find neighbor with least number of common edges
                FVertex minNb = null;
                int mintersect = int.MaxValue;
                foreach (int v in minDegree.Adj)
                {
                    int mytersect = CommonNB(minDegree, baseVertices[v]);

                    if (mytersect < mintersect || (mytersect == mintersect && baseVertices[v].Degree > minNb.Degree))
                    {
                        mintersect = mytersect;
                        minNb = baseVertices[v];
                    }
                }

                // We are going to contract minDegree into minNb
                foreach (int u in minDegree.Adj)
                {
                    if (u == minNb.Id)
                        continue;

                    if (AddEdge(u, minNb.Id))
                        undoStack.Push(new UndoInfo(u, minNb.Id));
                    else
                    {
                        degreeLists[baseVertices[u].Degree - 1].Add(u);
                        min = Math.Min(min, baseVertices[u].Degree - 1);
                    }
                }

                RemoveVertex(minDegree);
                degreeLists[minNb.Degree].Add(minNb.Id);
                min = Math.Min(min, minNb.Degree);
            }

            // Restore the damage
            while (vertexCount < oldCount)
                Uneliminate();

            while (min < vertexCount)
                degreeLists[min++].Clear();

            return lb;
        }

        public static Graph DFSInit(Graph G)
        {
            for (int width = Math.Min(G.vertices.Count, Math.Max(2, Program.BagsList.Count > 0 ? Program.BagsList.Max((b) => b.Count) : 0)); width <= G.vertices.Count; width++)
            {
                FastGraph fastCopy = new FastGraph(G);
                List<int> eliminationOrder = fastCopy.DFS(width, null);
                if (eliminationOrder != null) // We know that width is the minimum width possible
                    return G.TDFromEliminationOrder(eliminationOrder.Select((i) => fastCopy.baseVertices[i].OriginalVertex).ToList());
            }

            throw new Exception("DFS failed to find a solution");
        }
    }
}
