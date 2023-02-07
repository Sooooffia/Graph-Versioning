using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    partial class FastGraph
    {
        // Technique due to Dow&Korf: ComputeDVS looks at the current elimination order, and determines of it contains any dependent vertex sequences
        // If check == true: returns true or false depending on whether the current elimination order contains a duplicated dependent vertex sequence (and can be pruned)
        // If check == false: computes the dependent vertex sequences and adds them to the table. This operation is only valid if ComputeDVS(true) is true.
        bool ComputeDVS(bool check)
        {
            if (currentOrder.Count < 2)
                return true;

            // Set up union-find data structure
            foreach (FVertex v in currentOrder)
            {
                rep[v.Id] = v;
                rank[v.Id] = 0;
            }

            // Goal: enumerate over G_i = G_0, G_1,...G_r
            // Consider the graph induced by v_i+1,...,v_r
            // If v_i+1 and v_r are in the same component
            // Then the vertices in this component form a DVS for G_i

            // Idea: begin with current graph, then "uneliminate" vertices as we go
            // Start with singleton graph consisting of currentOrder[Count-1]
            for (int i = currentOrder.Count - 2; i >= 0; i--)
            {
                // "Undo" the elimination of currentOrder[i]
                // What does this do:
                // - Restore original edges
                // - Remove edges added because of clique property but these edges don't affect the connectivity of the vertices we are interested in
                // Ergo: just restore the connectivity of the removed edges
                foreach (int v in currentOrder[i].Adj)
                    if (!vertexPresent[v])
                        Union(baseVertices[v], currentOrder[i]);

                // We found a new valid DVS
                if (Find(currentOrder[i]) == Find(currentOrder[currentOrder.Count - 1]))
                {
                    // Find the vertices of the component
                    FVertex[] depSeq = currentOrder.Skip(i).Where(v => Find(v) == Find(currentOrder[i])).ToArray();

                    // Compute the key representing this DVS
                    BitArray key = new BitArray(baseVertices.Length);
                    foreach (FVertex v in depSeq)
                        key[v.Id] = true;
                    bool preexisting = false;

                    // Look whether any layer contains this DVS
                    for (int j = i; j >= 0; j--)
                    {
                        if (layerData[baseVertices.Length - 1 - j].DVS.ContainsKey(key))
                        {
                            // We found it, is this a different order of eliminating the same vertices (bad) or the same order (okay)
                            if (!depSeq.SequenceEqual(layerData[baseVertices.Length - 1 - j].DVS[key]))
                                return false;
                            else
                            {
                                preexisting = true;
                                break;
                            }
                        }
                    }

                    // If it did not exist before, we can add it
                    if (!preexisting)
                        layerData[baseVertices.Length - 1 - i].DVS.Add(key, depSeq);
                }
            }

            return true;
        }

        // Union by rank
        void Union(FVertex x, FVertex y)
        {
            x = Find(x); y = Find(y);
            if (rank[x.Id] < rank[y.Id])
                rep[x.Id] = y;
            else
            {
                rep[y.Id] = x;
                if (rank[x.Id] == rank[y.Id])
                    rank[x.Id]++;
            }
        }

        // Find with path compression
        FVertex Find(FVertex cur)
        {
            if (rep[cur.Id] != cur)
                rep[cur.Id] = Find(rep[cur.Id]);
            return rep[cur.Id];
        }
    }
}
