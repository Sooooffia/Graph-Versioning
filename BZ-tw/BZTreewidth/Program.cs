using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace BZTreewidth
{
    class Program
    {
        public static List<List<Vertex>> BagsList; // Global variabele that stores the bags used in the decomposition
        public static int TotalExpanded;
        public static int SeparatorTried;
        public static Random random;

        static void Main(string[] args)
        {
            int rIndex = Array.IndexOf(args, "-s");
            try
            {
                random = new Random(int.Parse(args[rIndex + 1]));
            }
            catch
            {
                random = new Random(4321);
            }

            //QuickTest();

            HandleCase();
        }

        static void QuickTest()
        {
            HandleFile("../../instances/heur-random/RandomGNM_100_100.gr");

            foreach (string dir in new string[] { "medium", "hard" })
            {
                foreach (string f in Directory.EnumerateFiles("../../instances/" + dir))
                    HandleFile(f);
            }
        }

        static void HandleFile(string f)
        {
            if (!f.EndsWith(".gr") && !f.EndsWith(".dgf") && !f.EndsWith(".dimacs") && !f.EndsWith(".col")) return; // Only process input (and not output) files

            string name = Path.GetFileNameWithoutExtension(f).Substring(0, Math.Min(Path.GetFileNameWithoutExtension(f).Length, 29));
            Console.Write(name + new String(' ', 30 - name.Length));

            TextReader oldIn = Console.In;
            TextWriter oldOut = Console.Out;

            StreamReader fileReader = new StreamReader(f);
            Console.SetIn(fileReader);

            MemoryStream outStream = new MemoryStream();
            StreamWriter outStreamWriter = new StreamWriter(outStream);
            Console.SetOut(outStreamWriter);

            Stopwatch sw = new Stopwatch();
            sw.Start();
            HandleCase();
            sw.Stop();

            Console.SetOut(oldOut);
            Console.SetIn(oldIn);

            fileReader.Close();
            outStreamWriter.Close();
            outStream.Close();

            Console.WriteLine(" tw={0}\texp={1} sep={2} {3}s", BagsList.Max((b) => b.Count), TotalExpanded + new String(' ', 8 - TotalExpanded.ToString().Length), SeparatorTried + new String(' ', 8 - SeparatorTried.ToString().Length), Math.Round(sw.ElapsedMilliseconds / 1000.0, 1));
        }

        static void HandleCase()
        {
            Stopwatch timer = new Stopwatch();
            timer.Start();

            // Parse the input graph
            Graph g = new Graph();
            for (string line = Console.ReadLine(); line != null; line = Console.ReadLine() )
            {
                if (line.StartsWith("c") || line.StartsWith("n")) continue; // Comment or dimacs node

                if (line.StartsWith("p")) // Initialization
                {
                    string[] cf = line.Split(' ');
                    
                    for (int i = 0; i < int.Parse(cf[2]); i++)
                        g.AddVertex(i);

                    continue;
                }

                // Dimacs-style edge
                if (line.StartsWith("e"))
                {
                    string[] vt = line.Split(' '); // Edge
                    g.AddVertex(int.Parse(vt[1]) - 1);
                    g.AddVertex(int.Parse(vt[2]) - 1);
                    g.AddEdge(g[int.Parse(vt[1]) - 1], g[int.Parse(vt[2]) - 1]);
                    continue;
                }

                // Something else, possibly an edge
                try
                {
                    string[] vt = line.Split(' '); // Edge
                    g.AddVertex(int.Parse(vt[0]) - 1);
                    g.AddVertex(int.Parse(vt[1]) - 1);
                    g.AddEdge(g[int.Parse(vt[0]) - 1], g[int.Parse(vt[1]) - 1]);
                }
                catch { }
            }

            if (g.vertices.Count == 0)
            {
                Console.WriteLine("s td 0 0 0");
                return;
            }

            // Run the algorithm
            BagsList = new List<List<Vertex>>();
            TotalExpanded = 0;
            SeparatorTried = 0;

            Graph TD = g.Decompose();
            int treewidth = BagsList.Max((b) => b.Count);

            // Write the output decomposition
            Console.Write("s td {0} {1} {2}", TD.vertices.Count, treewidth, g.vertices.Count); //s-line
            
            // Output bag contents
            int bc = 1;
            foreach (Vertex v in TD.vertices.Keys)
            {
                Console.WriteLine();
                Console.Write("b " + bc);
                foreach (Vertex v2 in BagsList[v.Label])
                    Console.Write(" " + (v2.Label + 1));
                v.Label = bc++;
            }

            // Output edges
            foreach (Edge e in TD.edges)
            {
                Console.WriteLine();
                Console.Write((e.a.Label) + " " + (e.b.Label));
            }

            timer.Stop();
            //Console.Write("tw\t" + treewidth + "\texp\t" + TotalExpanded + "\ttime\t" + Math.Round(timer.ElapsedMilliseconds / 1000d, 2));
        }
    }

    static class Helpers
    {
        // http://www.dotnetperls.com/fisher-yates-shuffle
        public static void Shuffle<T>(T[] array)
        {
            int n = array.Length;
            for (int i = 0; i < n; i++)
            {
                int r = i + Program.random.Next(0, n - i);
                T t = array[r];
                array[r] = array[i];
                array[i] = t;
            }
        }

        // http://stackoverflow.com/questions/273313/randomize-a-listt
        public static void Shuffle<T>(this IList<T> list)
        {
            int n = list.Count;
            while (n > 1)
            {
                n--;
                int k = Program.random.Next(n + 1);
                T value = list[k];
                list[k] = list[n];
                list[n] = value;
            }
        }
    }
}
