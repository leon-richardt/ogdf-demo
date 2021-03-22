#include <ogdf/basic/EdgeArray.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

#include <iostream>

using namespace ogdf;

void setupGraph(Graph &g, GraphAttributes &attrs) {
  node upperLeft = g.newNode();
  attrs.x(upperLeft) = 0;
  attrs.y(upperLeft) = 2;

  node upperRight = g.newNode();
  attrs.x(upperRight) = 4;
  attrs.y(upperRight) = 2;

  node rooftop = g.newNode();
  attrs.x(rooftop) = 2;
  attrs.y(rooftop) = 0;

  node lowerLeft = g.newNode();
  attrs.x(lowerLeft) = 0;
  attrs.y(lowerLeft) = 6;

  node lowerRight = g.newNode();
  attrs.x(lowerRight) = 4;
  attrs.y(lowerRight) = 6;

  g.newEdge(upperLeft, rooftop);
  g.newEdge(upperLeft, upperRight);
  g.newEdge(rooftop, upperRight);

  g.newEdge(upperLeft, lowerLeft);
  g.newEdge(upperLeft, lowerRight);

  g.newEdge(lowerLeft, upperRight);
  g.newEdge(lowerLeft, lowerRight);
  g.newEdge(upperRight, lowerRight);
}

void makeReadable(Graph &g, GraphAttributes &attrs) {
  Array<node> nodes;
  g.allNodes(nodes);

  for (node n : nodes) {
    attrs.width(n) = 1;
    attrs.height(n) = 1;
    attrs.strokeWidth(n) = 0.1;
  }

  Array<edge> edges;
  g.allEdges(edges);
  for (edge e : edges) {
    attrs.strokeWidth(e) = 0.05;
  }
}

int main() {
  Graph g;
  GraphAttributes attrs(
      g, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
             GraphAttributes::nodeStyle | GraphAttributes::edgeStyle);

  setupGraph(g, attrs);
  makeReadable(g, attrs);

  GraphIO::write(attrs, "output-manual.svg", GraphIO::drawSVG);

  return 0;
}
