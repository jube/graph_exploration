#include <disc/graph/Cover.h>

namespace disc {

  Metrics coverGraphOnceRandom(const Graph& g, Engine& engine, std::size_t length) {
    Metrics res;

    std::size_t count = g.getVertexCount();

    std::set<VertexDescriptor> visited;

    std::size_t iterations = 0;

    bool has_50 = false;
    bool has_90 = false;
    bool has_95 = false;
    bool has_99 = false;

    while (visited.size() != count) {
      auto path = g.makeRandomPath(length, engine);
      visited.insert(path.begin(), path.end());

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

  std::vector<Metrics> coverGraphMultipleRandom(const Graph& g, Engine& engine, std::size_t length, std::size_t tries) {
    std::vector<Metrics> results;

    for (std::size_t i = 0; i < tries; ++i) {
      auto m = coverGraphOnceRandom(g, engine, length);
      std::cout << '.' << std::flush;
      results.push_back(m);
    }

    std::cout << '\n';
    return results;
  }

}
