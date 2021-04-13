#include <ogdf/basic/EdgeArray.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/graphalg/Dijkstra.h>

#include <fstream>
#include <iostream>
#include <list>
#include <string>

int main(int argc, const char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Please only provide the input GraphML file" << std::endl;
        return 1;
    }

    std::ifstream file;
    file.open(argv[1]);

    ogdf::Graph g;
    ogdf::GraphAttributes ga(g, ogdf::GraphAttributes::all);
    ogdf::GraphIO::readGraphML(ga, g, file);

    // Obtain edge weights as an EdgeArray for Dijkstra
    ogdf::EdgeArray<double> weights(g);
    for (const auto &e : g.edges)
    {
        weights[e] = ga.doubleWeight(e);
    }

    ogdf::NodeArray<ogdf::edge> preds;
    ogdf::NodeArray<double> dist;
    const ogdf::node origin = g.firstNode();

    // Needs to be called on an instance for some reason?
    ogdf::Dijkstra<double>().call(g, weights, origin, preds, dist, false);

    std::cout << "Minimum distance from " << origin << " to ...\n";
    for (const auto &n : g.nodes)
    {
        std::cout << "  ... node " << n << ": " << dist[n];

        // Build path in "correct" order (starting from source)
        std::list<ogdf::edge> path;
        ogdf::node curTarget = n;
        ogdf::edge curEdge = preds[curTarget];
        while (curEdge != nullptr)
        {
            path.push_front(curEdge);
            ogdf::node source = curEdge->opposite(curTarget);
            curEdge = preds[source];
            curTarget = source;
        }

        // Print path
        if (path.size() == 0)
        {
            // Path from start node to start node -> empty
            std::cout << "\n";
            continue;
        }

        std::cout << ", path: " << path.front()->source();
        ogdf::node lastVisited = path.front()->source();
        for (const ogdf::edge &e : path)
        {
            std::cout << " -> " << e->opposite(lastVisited);
            lastVisited = e->opposite(lastVisited);
        }
        std::cout << "\n";
    }
}
