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

using namespace ogdf;

void setupGraph(Graph &g, GraphAttributes &attrs)
{
    static const std::vector<string> labels = {"1", "2", "3", "4", "5"};

    // Nodes (insertion order is preserved)
    node lowerLeft = g.newNode();
    node upperRight = g.newNode();
    node upperLeft = g.newNode();
    node rooftop = g.newNode();
    node lowerRight = g.newNode();

    int i = 0;
    for (const node &n : attrs.constGraph().nodes)
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

void naiveLayout(GraphAttributes &attrs)
{
    static const std::vector<std::tuple<int, int>> positions = {
        {0, 120}, {80, 40}, {0, 40}, {40, 0}, {80, 120}};

    int i = 0;

    // We rely on the fact that node insertion order is preserved here
    for (const node &n : attrs.constGraph().nodes)
    {
        const auto &tup = positions[i++];
        const int x = std::get<0>(tup);
        const int y = std::get<1>(tup);

        attrs.x(n) = x;
        attrs.y(n) = y;
    }
}

void planarLayout(GraphAttributes &attrs)
{
    PlanarizationLayout pl;

    SubgraphPlanarizer crossMin;

    auto *ps = new PlanarSubgraphFast<int>;
    VariableEmbeddingInserter *ves = new VariableEmbeddingInserter;
    ves->removeReinsert(RemoveReinsertType::All);

    crossMin.setSubgraph(ps);
    crossMin.setInserter(ves);

    EmbedderMinDepthMaxFaceLayers *emb = new EmbedderMinDepthMaxFaceLayers;
    pl.setEmbedder(emb);

    OrthoLayout *ol = new OrthoLayout;
    ol->separation(20.0);
    ol->cOverhang(0.4);
    pl.setPlanarLayouter(ol);

    pl.call(attrs);
}

int main()
{
    Graph g;
    GraphAttributes attrs(
        g, GraphAttributes::nodeGraphics | GraphAttributes::nodeStyle |
               GraphAttributes::nodeType | GraphAttributes::nodeLabel |
               GraphAttributes::nodeWeight | GraphAttributes::edgeGraphics |
               GraphAttributes::edgeStyle | GraphAttributes::edgeType);

    setupGraph(g, attrs);

    naiveLayout(attrs);
    GraphIO::write(attrs, "output-naive.svg", GraphIO::drawSVG);

    planarLayout(attrs);
    GraphIO::write(attrs, "output-planar.svg", GraphIO::drawSVG);

    return 0;
}
