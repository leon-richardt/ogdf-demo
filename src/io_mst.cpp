#include <ogdf/basic/EdgeArray.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/fileformats/GraphIO.h>

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

    const double cost = ogdf::makeMinimumSpanningTree(g, weights);

    std::cout << "Cost of MST: " << cost << " , edges in MST:\n";
    for (const ogdf::edge &e : g.edges)
        std::cout << e << "\n";
}
