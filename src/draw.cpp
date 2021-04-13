#include <ogdf/basic/EdgeArray.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

#include <iostream>
#include <tuple>

void setupGraph(ogdf::Graph &g, ogdf::GraphAttributes &attrs)
{
    static const std::vector<ogdf::string> labels = {"1", "2", "3", "4", "5"};

    // Nodes (insertion order is preserved)
    ogdf::node lowerLeft = g.newNode();
    ogdf::node upperRight = g.newNode();
    ogdf::node upperLeft = g.newNode();
    ogdf::node rooftop = g.newNode();
    ogdf::node lowerRight = g.newNode();

    int i = 0;
    for (const ogdf::node &n : attrs.constGraph().nodes)
        attrs.label(n) = labels[i++];

    // Edges
    g.newEdge(lowerLeft, upperRight);
    g.newEdge(upperRight, upperLeft);
    g.newEdge(upperLeft, rooftop);
    g.newEdge(rooftop, upperRight);
    g.newEdge(upperRight, lowerRight);
    g.newEdge(lowerRight, upperLeft);
    g.newEdge(upperLeft, lowerLeft);
    g.newEdge(lowerLeft, lowerRight);
}

void naiveLayout(ogdf::GraphAttributes &attrs)
{
    static const std::vector<std::tuple<int, int>> positions = {
        {0, 120}, {80, 40}, {0, 40}, {40, 0}, {80, 120}};

    int i = 0;

    // We rely on the fact that node insertion order is preserved here
    for (const auto &n : attrs.constGraph().nodes)
    {
        const auto &tup = positions[i++];
        const int x = std::get<0>(tup);
        const int y = std::get<1>(tup);

        attrs.x(n) = x;
        attrs.y(n) = y;
    }
}

void planarLayout(ogdf::GraphAttributes &attrs)
{
    ogdf::PlanarizationLayout pl;
    auto *emb = new ogdf::EmbedderMinDepthMaxFaceLayers;
    pl.setEmbedder(emb);

    auto *ol = new ogdf::OrthoLayout;
    ol->separation(20.0);
    ol->cOverhang(0.4);
    pl.setPlanarLayouter(ol);

    pl.call(attrs);
}

int main()
{
    ogdf::Graph g;
    using GA = ogdf::GraphAttributes;
    ogdf::GraphAttributes attrs(
        g, GA::nodeGraphics | GA::nodeStyle | GA::nodeType | GA::nodeLabel |
               GA::edgeGraphics | GA::edgeStyle | GA::edgeType);

    setupGraph(g, attrs);

    naiveLayout(attrs);
    ogdf::GraphIO::write(attrs, "output-naive.svg", ogdf::GraphIO::drawSVG);

    planarLayout(attrs);
    ogdf::GraphIO::write(attrs, "output-planar.svg", ogdf::GraphIO::drawSVG);

    return 0;
}
