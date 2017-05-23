#include <cstdlib>

#include <iostream>
#include <fstream>

#include <disc/graph/Graph.h>

#include "common.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cerr << "Usage: graph_features <graph>\n";
    return EXIT_FAILURE;
  }

  std::cerr << "Importing graph...\n";
  std::ifstream input(argv[1]);
  disc::Graph g = disc::Graph::import(input);

  std::cout << "G has " << g.getVertexCount() << " vertices\n";
  std::cout << "G has " << g.getEdgeCount() << " edges\n";

  bool connected = g.isConnected();

  if (connected) {
    std::cout << "G is connected\n";
  } else {
    std::cout << "G is *not* connected\n";
  }

  if (!connected) {
    return EXIT_SUCCESS;
  }

  std::size_t ecc = g.getEccentricity();
  std::cout << "G has eccentricity: " << ecc << '\n';

  std::size_t length = static_cast<std::size_t>(disc::LengthFactor * ecc);

  auto pathCount = g.countPathOfMaximumLengthFromInitialState(length);
  std::cout << "Path of length " << length << ": " << pathCount << '\n';

  return EXIT_SUCCESS;
}
