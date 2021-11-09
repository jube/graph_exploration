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
#ifndef DISC_COVER_H
#define DISC_COVER_H

#include <iostream>

#include "Graph.h"
#include "Metrics.h"
#include "Random.h"

namespace disc {

  Metrics coverGraphOnceRandom(const Graph& g, Engine& engine, std::size_t length);
  std::vector<Metrics> coverGraphMultipleRandom(const Graph& g, Engine& engine, std::size_t length, std::size_t tries);

  Metrics coverGraphOnceUnexplored(const Graph& g, Engine& engine, std::size_t length);
  std::vector<Metrics> coverGraphMultipleUnexplored(const Graph& g, Engine& engine, std::size_t length, std::size_t tries);

  template<typename Distribution>
  Metrics coverGraphOnce(const Graph& g, Engine& engine, Distribution distribution, std::size_t length) {
    Metrics res;

    std::size_t count = g.getVertexCount();

    std::set<VertexDescriptor> visited;

    std::size_t iterations = 0;

    bool has_50 = false;
    bool has_90 = false;
    bool has_95 = false;
    bool has_99 = false;

    while (visited.size() != count) {
      VertexDescriptor v = distribution(engine);

      auto derived = buildGraphCrossingOneVertex(g, v);
      auto paths = derived.computePathCountOfMaximumLength(length);
      auto path = derived.makeUniformPath(length, engine, paths);

      auto newlyVisited = derived.getVertexData(path);
      visited.insert(newlyVisited.begin(), newlyVisited.end());

      ++iterations;

      if (!has_50 && 2 * visited.size() >= count) {
        has_50 = true;
        res.covered_50 = iterations;
        std::cout << '5' << std::flush;
      }

      if (!has_90 && 10 * visited.size() >= 9 * count) {
        has_90 = true;
        res.covered_90 = iterations;
        std::cout << '9' << std::flush;
      }

      if (!has_95 && 20 * visited.size() >= 19 * count) {
        has_95 = true;
        res.covered_95 = iterations;
        std::cout << '=' << std::flush;
      }

      if (!has_99 && 100 * visited.size() >= 99 * count) {
        has_99 = true;
        res.covered_99 = iterations;
        std::cout << '-' << std::flush;
      }
    }

    res.covered_100 = iterations;
    return res;
  }

  template<typename Distribution>
  std::vector<Metrics> coverGraphMultiple(const Graph& g, Engine& engine, Distribution distribution, std::size_t length, std::size_t tries) {
    std::vector<Metrics> results;

    for (std::size_t i = 0; i < tries; ++i) {
      auto m = coverGraphOnce(g, engine, distribution, length);
      std::cout << '.' << std::flush;
      results.push_back(m);
    }

    std::cout << '\n';
    return results;
  }

}

#endif // DISC_COVER_H
