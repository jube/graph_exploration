#include <cstdlib>

#include <iostream>
#include <fstream>

#include <disc/graph/Cover.h>
#include <disc/graph/Graph.h>
#include <disc/graph/Metrics.h>
#include <disc/graph/Problem.h>
#include <disc/graph/Random.h>

#include "common.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cerr << "Usage: xp_exact <graph>\n";
    return EXIT_FAILURE;
  }

  std::cerr << "Importing graph...\n";
  std::ifstream input(argv[1]);
  disc::Graph g = disc::Graph::import(input);

  disc::Engine engine = disc::getCorrectlyInitializedEngine();

  std::size_t ecc = g.getEccentricity();
  std::size_t length = static_cast<std::size_t>(disc::LengthFactor * ecc);

  // random

  auto coeffs = g.computeExactNormalizedAlphaMatrix(length);
  auto pi = disc::computePii(coeffs, nullptr);

  std::discrete_distribution<uint64_t> distribution(pi.begin(), pi.end());

  std::cout << "Exact:\n";
  auto metrics = disc::coverGraphMultiple(g, engine, distribution, length, disc::CoverTries);
  auto mean = disc::computeMeanMetrics(metrics);
  std::cout << mean << '\n';

  return EXIT_SUCCESS;
}
