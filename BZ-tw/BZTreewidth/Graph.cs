using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    // Represents an undirected graph and contains the functions pertaining to tree decompositions
    // Also used to represent the tree decomposition itself
    // Note that vertices are only compared by their labels, so distinct vertices from different graphs are really the "same" vertex; this is used extensively (and sometimes counter-intuitively)
    partial class Graph
    {
        public Dictionary<Vertex, Vertex> vertices; // This is kind of a hack: this is really just a hashset, the additional key information is only used so we can get our OWN copy of the vertex of another copy of the graph
        public HashSet<Edge> edges;

        public Graph()
        {
            vertices = new Dictionary<Vertex, Vertex>();
            edges = new HashSet<Edge>();
        }

        // Takes the subgraph induced by some set of vertices
        public Graph(List<Vertex> sourceVertices)
            : this()
        {
            // Copy vertices
            foreach (Vertex v in sourceVertices)
                AddVertex(v);

            // Note that this only copies induced edges, AddEdge ignores the others
            foreach (Vertex v in sourceVertices)
                foreach (Edge e in v.Edges)
                    AddEdge(e.a, e.b);
        }

        // Copy a given vertex to this graph
        public Vertex AddVertex(Vertex v)
        {
            return AddVertex(v.Label);
        }

        // Create a new vertex with a given label
        public Vertex AddVertex(int label)
        {
            Vertex v = new Vertex(label);
            if (!vertices.ContainsKey(v)) vertices.Add(v, v);
            return vertices[v];
        }

        // Copy the vertices and edges of another graph to this graph
        public void AddGraph(Graph other)
        {
            foreach (Vertex v in other.vertices.Keys)
                if (!vertices.ContainsKey(v))
                    vertices.Add(v, v);
            foreach (Edge e in other.edges)
                AddEdge(e.a, e.b);
        }

        // Adds an edge, only if the vertices exist and the edge doesn't already exist
        public Edge? AddEdge(Edge e)
        {
            return AddEdge(e.a, e.b);
        }

        // Adds an edge, only if the vertices exist and the edge doesn't already exist
        public Edge? AddEdge(Vertex a, Vertex b)
        {
            Vertex v1, v2;
            if (!vertices.TryGetValue(a, out v1))
                return null;
            if (!vertices.TryGetValue(b, out v2))
                return null;

            Edge e = new Edge(v1, v2);
            if (!edges.Contains(e))
            {
                edges.Add(e);

                v1.Edges.Add(e);
                v2.Edges.Add(e);

                return e;
            }

            return null;
        }

        // Computes the connected components. The graph should not be modified after calling this method, as it does not reinitialize variabeles.
        List<Vertex> component = new List<Vertex>();
        List<List<Vertex>> components = new List<List<Vertex>>();
        public List<List<Vertex>> getComponents()
        {
            foreach (Vertex v in vertices.Keys)
            {
                if (v.depth > -1)
                    continue;
                v.depth = 0;
                LabelDFS(v);

                components.Add(component);
                component = new List<Vertex>();
            }

            return components;
        }

        // Returns an articulation point (if it exists). The graph should not be modified after calling this method, as it does not reinitialize variabeles.
        public Vertex ArticulationPoint()
        {
            return ArticulationPoints().FirstOrDefault();
        }

        // Enumerates the articulation points of the graph. The graph should not be modified after calling this method, as it does not reinitialize variabeles.
        public IEnumerable<Vertex> ArticulationPoints()
        {
            int treecount;

            foreach (Vertex v in vertices.Keys)
            {
                if (v.cv) yield return v;

                if (v.depth == 0)
                {
                    treecount = 0;
                    foreach (Edge e in v.Edges)
                        if (e.oe(v).depth == 1)
                            treecount++;
                    if (treecount > 1)
                        yield return v;
                }
                if (v.depth > -1) continue;

                v.depth = 0;
                LabelDFS(v);

                treecount = 0;
                foreach (Edge e in v.Edges)
                    if (e.oe(v).depth == 1)
                        treecount++;
                if (treecount > 1)
                    yield return v;
            }

            yield break;
        }

        // Enumerates the articulation points of the graph, and also returns any vertices in a component of a given exact size
        public IEnumerable<Vertex> ArticulationPointsOrComponentSize(int targetSize)
        {
            int treecount;

            foreach (Vertex v in vertices.Keys)
            {
                if (v.cv) yield return v;

                if (v.depth == 0)
                {
                    treecount = 0;
                    foreach (Edge e in v.Edges)
                        if (e.oe(v).depth == 1)
                            treecount++;
                    if (treecount > 1)
                        yield return v;
                }
                if (v.depth > -1) continue;

                int oldCompCount = component.Count;

                v.depth = 0;
                LabelDFS(v);

                if (component.Count - oldCompCount == targetSize)
                {
                    for (int i = oldCompCount; i < component.Count; i++)
                        component[i].cv = true;
                    yield return v;
                }

                treecount = 0;
                foreach (Edge e in v.Edges)
                    if (e.oe(v).depth == 1)
                        treecount++;
                if (treecount > 1)
                    yield return v;
            }

            yield break;
        }

        // Subroutine (DFS) used for articulation points and connected components (Tarjan/Hopcroft)
        public int LabelDFS(Vertex v)
        {
            int minD = v.depth;

            component.Add(v);

            foreach (Edge e in v.Edges)
            {
                if (e.oe(v).depth < 0)
                {
                    e.oe(v).depth = v.depth + 1;
                    int m = LabelDFS(e.oe(v));
                    if (m >= v.depth && v.depth > 0)
                        v.cv = true;
                    minD = Math.Min(minD, m);
                }
                else
                    minD = Math.Min(minD, e.oe(v).depth);
            }
            return minD;
        }

        // Get a vertex with a specified label
        public Vertex this[int label]
        {
            get
            {
                Vertex temp = new Vertex(label);
                return vertices[temp];
            }
        }
    }
}
