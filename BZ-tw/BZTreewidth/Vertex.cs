using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    // Represents a vertex which is identified using an integer label
    class Vertex
    {
        public List<Edge> Edges = new List<Edge>();
        public int Label;
        public Edge? SignalEdge = null; // If this edge is added, we should check whether this vertex becomes simplicial

        // For 2-connected components algorithm
        public int depth = -1; public bool cv = false;

        public Vertex(int label)
        {
            this.Label = label;
        }

        // Adjacency list
        public IEnumerable<Vertex> Adj
        {
            get
            {
                return Edges.Select((e) => e.oe(this));
            }
        }

        Vertex[] adjLookup;

        // FAST Adjacency list (memoization, adjacency should not be modified after first use of FAdj)
        public Vertex[] FAdj
        {
            get
            {
                if (adjLookup == null)
                {
                    lock (this)
                        if (adjLookup == null)
                        {
                            adjLookup = Edges.Select((e) => e.oe(this)).ToArray();
                        }
                }
                return adjLookup;
            }
        }

        public override int GetHashCode()
        {
            return Label.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            return Label == ((Vertex)obj).Label;
        }
    }
}