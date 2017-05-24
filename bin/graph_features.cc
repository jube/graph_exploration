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

#include <disc/graph/Graph.h>

#include "common.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
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
