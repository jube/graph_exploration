#include <cstdlib>

#include <iostream>
#include <fstream>

#include <disc/graph/Cover.h>
#include <disc/graph/Graph.h>
#include <disc/graph/Metrics.h>
#include <disc/graph/Random.h>

#include "common.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: xp_randon_and_uniform <graph>\n";
    return EXIT_FAILURE;
  }

  std::cerr << "Importing graph...\n";
  std::ifstream input(argv[1]);
  disc::Graph g = disc::Graph::import(input);

  disc::Engine engine = disc::getCorrectlyInitializedEngine();

  std::size_t ecc = g.getEccentricity();
  std::size_t length = static_cast<std::size_t>(disc::LengthFactor * ecc);

  // random

  {
    std::size_t count = g.getVertexCount();
    std::uniform_int_distribution<uint64_t> distribution(0, count - 1);

    std::cout << "Uniform:\n";
    auto metrics = disc::coverGraphMultiple(g, engine, distribution, length, disc::CoverTries);
    auto mean = disc::computeMeanMetrics(metrics);
    std::cout << mean << '\n';
  }

  {
    std::cout << "Random:\n";
    auto metrics = disc::coverGraphMultipleRandom(g, engine, length, disc::CoverTries);
    auto mean = disc::computeMeanMetrics(metrics);
    std::cout << mean << '\n';
  }

  return EXIT_SUCCESS;
}
