/*
 * Graph exploration
 * Copyright (C) 2017 Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
  if (argc != 4) {
    std::cerr << "Usage: xp_approx <graph> <factor> <threshold>\n";
    return EXIT_FAILURE;
  }

  std::cerr << "Importing graph...\n";
  std::ifstream input(argv[1]);
  disc::Graph g = disc::Graph::import(input);

  std::size_t factor = std::stoul(argv[2]);
  double threshold = std::stod(argv[3]);

  disc::Engine engine = disc::getCorrectlyInitializedEngine();

  std::size_t ecc = g.getEccentricity();
  std::size_t length = static_cast<std::size_t>(disc::LengthFactor * ecc);

  // random

  auto coeffs = g.computeApproxNormalizedAlphaMatrixWithThreshold(length, g.getVertexCount() * factor, engine, threshold);
  auto pi = disc::computePii(coeffs, nullptr);

  for (auto x : pi) {
    std::cout << x << ' ';
  }
  std::cout << '\n';

  std::discrete_distribution<uint64_t> distribution(pi.begin(), pi.end());

  std::cout << "Approx-" << factor << " :\n";
  auto metrics = disc::coverGraphMultiple(g, engine, distribution, length, disc::CoverTries);
  auto mean = disc::computeMeanMetrics(metrics);
  std::cout << mean << '\n';

  return EXIT_SUCCESS;
}
