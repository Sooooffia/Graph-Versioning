using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    // Represents an undirected edge
    struct Edge
    {
        public Vertex a, b;

        public Edge(Vertex a, Vertex b)
        {
            this.a = a;
            this.b = b;
        }

        // Given one of the endpoints, give the other endpoint
        public Vertex oe(Vertex y)
        {
            return a == y ? b : a;
        }

        public override int GetHashCode()
        {
            return a.GetHashCode() ^ b.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            Edge other = (Edge)obj;
            return ((other.a == a && other.b == b) || (other.b == a && other.a == b));
        }
    }
}
