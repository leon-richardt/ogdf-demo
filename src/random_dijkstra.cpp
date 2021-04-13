#include <ogdf/basic/EdgeArray.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/graphalg/Dijkstra.h>

#include <deque>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, const char **argv)
{
    if (argc != 3)
    {
        std::cerr
            << "Provide number of nodes and number of edges for random graph"
            << std::endl;
        return 1;
    }

    constexpr const int SEED = 0;
    ogdf::setSeed(SEED);

    // Generate random graph
    int NUM_NODES, NUM_EDGES;
    try
    {
        NUM_NODES = std::stoi(argv[1]);
        NUM_EDGES = std::stoi(argv[2]);
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Could not parse number of nodes or edges" << std::endl;
        return 2;
    }

    ogdf::Graph g;
    ogdf::randomSimpleConnectedGraph(g, NUM_NODES, NUM_EDGES);

    // Set up random edge weights
    constexpr const double MIN_WEIGHT = 0.1;
    constexpr const double MAX_WEIGHT = 100.0;

    ogdf::EdgeArray<double> weights(g);
    for (const auto &e : g.edges)
    {
        weights[e] = ogdf::randomDouble(MIN_WEIGHT, MAX_WEIGHT);
    }

    // Set up and call Dijkstra
    ogdf::NodeArray<ogdf::edge> preds;
    ogdf::NodeArray<double> dist;
    const ogdf::node origin = g.firstNode();

    ogdf::Dijkstra<double>().call(g, weights, origin, preds, dist, true);

    // Sort nodes by distance to origin
    std::vector<ogdf::node> nodes(g.nodes.size());
    for (const auto &n : g.nodes)
    {
        nodes.push_back(n);
    }

    std::sort(nodes.begin(), nodes.end(),
              [&dist](const ogdf::node &n1, const ogdf::node &n2) {
                  if (n1 == nullptr)
                      return false;

                  if (n2 == nullptr)
                      return true;

                  return dist[n1] <= dist[n2];
              });

    // Print results
    std::cout << "Minimum distance from node " << origin << " to ...\n";
    for (const auto &n : nodes)
    {
        if (n == nullptr)
            continue;

        std::cout << "  ... node " << n << ": " << dist[n];

        // Build path in "correct" order (starting from source)
        std::deque<ogdf::edge> path;
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
        if (path.empty())
        {
            // Path from start node to start node -> empty
            std::cout << "\n";
            continue;
        }

        std::cout << ", path: " << path.front()->source();
        for (const ogdf::edge &e : path)
        {
            std::cout << " -> " << e->target();
        }
        std::cout << "\n";
    }
}
