using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    partial class FastGraph
    {
        int vertexCount; // Stores how many vertices are currently present
        BitArray vertexPresent; // Indexing for which vertices are currently present
        FVertex[] baseVertices, currentVertices; // Stores all of the vertices (including deleted ones), while the second list stores only the currently present vertices
        int[,] adjMatrix; // Adjacency matrix, which contains pointers to the locations in the respective adjacency lists
        Stack<UndoInfo> undoStack = new Stack<UndoInfo>(); // Allows us to undo eliminations and contractions
        DFSData[] layerData; // Stores data specific to each layer in the search
        List<FVertex> currentOrder = new List<FVertex>(); // Stores the current elimination order
        List<int>[] degreeLists; // Used for MMW computation
        //int[,] commonNeighbors; // Track number of common neighbors, not used for now. Appears to give some benefit in some cases, but not for the sample graphs

        // Union-find data structure
        FVertex[] rep; int[] rank;

        public FastGraph(Graph G)
        {
            Vertex[] temp = G.vertices.Keys.ToArray();

            vertexCount = temp.Length;
            vertexPresent = new BitArray(vertexCount);
            baseVertices = new FVertex[vertexCount]; currentVertices = new FVertex[vertexCount];
            adjMatrix = new int[vertexCount, vertexCount];
            layerData = new DFSData[vertexCount];
            rep = new FVertex[vertexCount]; rank = new int[vertexCount];
            degreeLists = new List<int>[vertexCount];
            //commonNeighbors = new int[vertexCount, vertexCount];

            for (int i = 0; i < vertexCount; i++)
            {
                baseVertices[i] = new FVertex(i, i, vertexCount, temp[i]);
                currentVertices[i] = baseVertices[i];
                vertexPresent[i] = true;
                layerData[i] = new DFSData(vertexCount);
                degreeLists[i] = new List<int>();
            }

            foreach (Edge e in G.edges)
                AddEdge(Array.IndexOf(temp, e.a), Array.IndexOf(temp, e.b));
        }

        public IEnumerable<FVertex> Vertices
        {
            get
            {
                return currentVertices.Take(vertexCount);
            }
        }

        // Undoes the elimination (or contraction) of a vertex
        public void Uneliminate()
        {
            FVertex restoreVertex = baseVertices[undoStack.Pop().a];
            foreach (int u in restoreVertex.Adj)
                AddAdj(baseVertices[u], restoreVertex);

            currentVertices[vertexCount] = currentVertices[restoreVertex.ListLocation];
            currentVertices[restoreVertex.ListLocation].ListLocation = vertexCount;
            currentVertices[restoreVertex.ListLocation] = restoreVertex;
            vertexPresent[restoreVertex.Id] = true;
            vertexCount++;

            while (undoStack.Count > 0)
            {
                UndoInfo ui = undoStack.Peek();
                if (ui.b < 0) break;
                undoStack.Pop();
                RemoveEdge(ui.a, ui.b);
            }
        }

        // Eliminate a vertex
        public void EliminateVertex(FVertex a)
        {
            foreach (int u in a.Adj)
                foreach (int v in a.Adj)
                    if (u < v && AddEdge(u, v))
                        undoStack.Push(new UndoInfo(u, v));

            RemoveVertex(a);
        }

        // Remove vertex and incident edges
        public void RemoveVertex(FVertex a)
        {
            foreach (int u in a.Adj)
                RemoveAdj(baseVertices[u], a);

            vertexCount--;
            currentVertices[a.ListLocation] = currentVertices[vertexCount];
            currentVertices[vertexCount].ListLocation = a.ListLocation;
            vertexPresent[a.Id] = false;

            undoStack.Push(new UndoInfo(a.Id, -1));
        }

        public bool IsEdge(FVertex a, FVertex b)
        {
            return IsEdge(a.Id, b.Id);
        }

        public bool IsEdge(int a, int b)
        {
            return baseVertices[a].AdjVector[b];
        }

        public bool AddEdge(int a, int b)
        {
            return AddEdge(baseVertices[a], baseVertices[b]);
        }

        public bool RemoveEdge(int a, int b)
        {
            return RemoveEdge(baseVertices[a], baseVertices[b]);
        }

        // Add b to a's adj list
        public void AddAdj(FVertex a, FVertex b)
        {
            a.AdjVector[b.Id] = true;
            a.AdjList[a.Degree] = b.Id;
            adjMatrix[a.Id, b.Id] = a.Degree;
            a.Degree++;

            //foreach (int c in b.Adj)
                //commonNeighbors[a.Id, c]++;
        }

        // Remove b from a's adj list
        public void RemoveAdj(FVertex a, FVertex b)
        {
            a.AdjVector[b.Id] = false;
            a.Degree--;
            a.AdjList[adjMatrix[a.Id, b.Id]] = a.AdjList[a.Degree];
            adjMatrix[a.Id, a.AdjList[a.Degree]] = adjMatrix[a.Id, b.Id];

            //foreach (int c in b.Adj)
                //commonNeighbors[a.Id, c]--;
        }

        // Attempts to remove the edge between a and b, returns true if the edge was removed, false if the edge does not exist
        public bool RemoveEdge(FVertex a, FVertex b)
        {
            if (!IsEdge(a, b)) return false;

            RemoveAdj(a, b);
            RemoveAdj(b, a);

            return true;
        }

        // Adds an edge between a and b, returns true if the edge was added, false if the edge already existed
        public bool AddEdge(FVertex a, FVertex b)
        {
            if (IsEdge(a, b)) return false;

            AddAdj(a, b);
            AddAdj(b, a);

            return true;
        }

        // Tests whether a given vertex is currently in the graph
        public bool IsVertex(int v)
        {
            return vertexPresent[v];
        }

        // Tests whether a given vertex is currently in the graph
        public bool IsVertex(FVertex v)
        {
            return IsVertex(v.Id);
        }

        // Tests whether a vertex is simplicial
        public bool TestSimplicial(FVertex v)
        {
#if !DFS_SIMPLICIALRULE
            return false;
#else

            // Signal edges: we remember which edges are currently missing to hopefully speed the check up
            if (v.SignalA != null && v.SignalB != null && IsVertex(v.SignalA) && IsEdge(v, v.SignalA) && IsEdge(v, v.SignalB) && IsVertex(v.SignalB) && !IsEdge(v.SignalA, v.SignalB))
                return false;

            for (int i = 0; i < v.Degree; i++)
                for (int j = 0; j < i; j++)
                    if (!IsEdge(v.AdjList[i], v.AdjList[j]))
                    {
                        v.SignalA = baseVertices[v.AdjList[i]];
                        v.SignalB = baseVertices[v.AdjList[j]];
                        return false;
                    }

            return true;
#endif
        }

        // Tests whether a vertex is almost simplicial
        public bool TestAlmostSimplicial(FVertex v, int upper)
        {
#if !DFS_ALMOSTSIMPLICIALRULE
            return false;
#else
            if (v.Degree >= upper)
                return false;

            int sa = v.SignalA.Id, sb = v.SignalB.Id;

            for (int i = 0; i < v.Degree; i++)
                for (int j = 0; j < i; j++)
                    if (!IsEdge(v.AdjList[i], v.AdjList[j]))
                    {
                        if ((i == sa && j == sb) || (j == sa && i == sb))
                            continue;
                        else if (i == sa || j == sa)
                            sb = sa;
                        else if (i == sb || j == sb)
                            sa = sb;
                        else
                            return false;
                    }

            return true;
#endif
        }

        public int CommonNB(FVertex a, FVertex b)
        {
            return a.AdjVector.XORCount(b.AdjVector);
        }

        // Returns layer-specific data for current layer
        public DFSData currentLayer
        {
            get
            {
                return layerData[vertexCount - 1];
            }
        }

        // Returns layer-specific data for next layer
        public DFSData nextLayer
        {
            get
            {
                return layerData[vertexCount - 2];
            }
        }
    }

    // Fast Graph Vertex
    class FVertex
    {
        public Vertex OriginalVertex; // Original label
        public int Id; // Number of this vertex
        public int ListLocation; // Location in currentVertices
        public int Degree;
        public int[] AdjList;
        public FVertex SignalA, SignalB;
        public BitArray AdjVector;

        public IEnumerable<int> Adj
        {
            get
            {
                return AdjList.Take(Degree);
            }
        }

        public FVertex(int Id, int ListLocation, int vertexCount, Vertex OriginalVertex)
        {
            this.Id = Id;
            this.ListLocation = ListLocation;
            this.OriginalVertex = OriginalVertex;
            AdjVector = new BitArray(vertexCount);
            AdjList = new int[vertexCount];
            Degree = 0;
        }
    }

    // Used to store what edge/vertex needs to be restored
    // Note that a vertex is represented by setting b to -1, while an edge uses both a and b to represent the endpoints
    struct UndoInfo
    {
        public int a, b;

        public UndoInfo(int a, int b)
        {
            this.a = a; this.b = b;
        }
    }

}
