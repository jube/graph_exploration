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
#include <disc/graph/Graph.h>

#include <cassert>

#include <algorithm>
#include <chrono>
#include <deque>
#include <iostream>
#include <limits>

namespace disc {

  Graph::Graph(std::size_t n)
  : m_nextVertexId(0)
  , m_nextEdgeId(0)
  , m_initialState(InvalidVertexDescriptor)
  {
    if (n > 0) {
      m_vertices.reserve(n);
      m_outEdges.reserve(n);
    }
  }

  VertexDescriptor Graph::addVertex() {
    auto id = m_nextVertexId++;
    m_vertices.push_back({ id });
    m_outEdges.push_back({ });
    return id;
  }

  Graph::VertexRange Graph::getVertices() const {
    return makeIntegerRange<VertexDescriptor>({ 0 } , { m_vertices.size() });
  }

  std::size_t Graph::getVertexCount() const {
    return m_vertices.size();
  }

  EdgeDescriptor Graph::addEdge(VertexDescriptor source, VertexDescriptor target) {
    auto id = m_nextEdgeId++;
    m_edges.push_back({ id, source, target });
    m_outEdges[source.index].insert(id);
    return id;
  }

  Graph::EdgeRange Graph::getEdges() const {
    return makeIntegerRange<EdgeDescriptor>({ 0 } , { m_edges.size() });
  }

  std::size_t Graph::getEdgeCount() const {
    return m_edges.size();
  }

  VertexDescriptor Graph::getSource(EdgeDescriptor e) const {
    return m_edges[e.index].source;
  }

  VertexDescriptor Graph::getTarget(EdgeDescriptor e) const {
    return m_edges[e.index].target;
  }

  Graph::OutEdgeRange Graph::getOutEdges(VertexDescriptor v) const {
    return makeIteratorRange(m_outEdges[v.index]);
  }

  void Graph::setInitialState(VertexDescriptor v) {
    m_initialState = v;
  }

  VertexDescriptor Graph::getInitialState() const {
    return m_initialState;
  }

  bool Graph::isInitialState(VertexDescriptor v) const {
    return m_initialState == v;
  }

  void Graph::addFinalState(VertexDescriptor v) {
    m_finalStates.insert(v);
  }

  bool Graph::isFinalState(VertexDescriptor v) const {
    return m_finalStates.count(v) > 0;
  }

  Graph::FinalStateRange Graph::getFinalStates() const {
    return makeIteratorRange(m_finalStates);
  }

  bool Graph::isConnected() const {
    auto count = getVertexCount();

    if (count == 0) {
      return true;
    }

    std::vector<bool> visited(count, false);
    std::deque<VertexDescriptor> todo;

    auto init = getInitialState();

    if (init == InvalidVertexDescriptor) {
      todo.push_back({ 0 });
      visited[0] = true;
    } else {
      todo.push_back(init);
      visited[init.index] = true;
    }

    while (!todo.empty()) {
      auto curr = todo.front();
      assert(visited[curr.index]);

      for (auto e : getOutEdges(curr)) {
        auto next = getTarget(e);

        if (!visited[next.index]) {
          todo.push_back(next);
          visited[next.index] = true;
        }
      }

      todo.pop_front();
    }

    return std::all_of(visited.begin(), visited.end(), [](bool b) { return b; });
  }

  std::size_t Graph::getEccentricity() const {
    auto count = getVertexCount();

    if (count == 0) {
      return 0;
    }

    std::vector<std::size_t> distance(count, count + 1);
    distance[getInitialState().index] = 0;

    for (std::size_t k = 0; k < count; ++k) {
      bool update = false;

      for (auto e : getEdges()) {
        std::size_t newDistance = distance[getSource(e).index] + 1;

        if (newDistance < distance[getTarget(e).index]) {
          distance[getTarget(e).index] = newDistance;
          update = true;
        }
      }

      if (!update) {
        break;
      }
    }

    return *std::max_element(distance.begin(), distance.end());
  }

  Matrix<double> Graph::computePathCountOfExactLength(std::size_t length) const {
    std::size_t count = getVertexCount();

    Matrix<double> paths(count, length + 1);
    paths.assign(0);

    for (auto v : getFinalStates()) {
      paths(v.index, 0) = 1;
    }

    for (std::size_t k = 1; k <= length; ++k) {
      for (auto v : getVertices()) {
        double pathCount = 0;

        for (auto e : getOutEdges(v)) {
          pathCount += paths(getTarget(e).index, k - 1);
        }

        paths(v.index, k) = pathCount;
      }
    }

    return paths;
  }

  Matrix<double> Graph::computePathCountOfMaximumLength(std::size_t length) const {
    auto paths = computePathCountOfExactLength(length);

    for (std::size_t j = 1; j < paths.getCols(); ++j) {
      for (std::size_t i = 0; i < paths.getRows(); ++i) {
        paths(i, j) += paths(i, j - 1);
      }
    }

    return paths;
  }

  double Graph::countPathOfMaximumLengthFromInitialState(std::size_t length) const {
    auto paths = computePathCountOfExactLength(length);

    double count = 0;

    for (std::size_t j = 0; j < paths.getCols(); ++j) {
      count += paths(m_initialState.index, j);
    }

    return count;
  }

  std::vector<VertexDescriptor> Graph::makeUniformPath(std::size_t length, Engine& engine, const Matrix<double>& paths) const {
    assert(paths.getRows() == getVertexCount());
    assert(paths.getCols() == length + 1);

    std::vector<VertexDescriptor> path;

    VertexDescriptor current = getInitialState();
    path.push_back(current);

    for (auto k = length; k > 0; --k) {
      std::vector<VertexDescriptor> vertices;
      std::vector<double> weights;

      for (auto e : getOutEdges(current)) {
        assert(getSource(e) == current);
        VertexDescriptor next = getTarget(e);

        double weight = paths(next.index, k - 1);

        if (weight > 0) {
          vertices.push_back(next);
          weights.push_back(weight);
        }
      }

      if (vertices.empty()) {
        // no more candidates, we are at a final state
        break;
      }

      double sum = std::accumulate(weights.begin(), weights.end(), 0.0);

      if (sum <= std::numeric_limits<double>::epsilon()) {
        std::fill(weights.begin(), weights.end(), 1.0);
      }

      std::discrete_distribution<> dist(weights.begin(), weights.end());
      VertexDescriptor next = vertices[dist(engine)];

      path.push_back(next);
      current = next;
    }

    assert(isFinalState(current));
    // assert(path.size() == length);

    return path;
  }

  std::vector<VertexDescriptor> Graph::makeRandomPath(std::size_t length, Engine& engine) const {
    std::vector<VertexDescriptor> path;

    VertexDescriptor current = getInitialState();
    path.push_back(current);

    for (auto k = length; k > 0; --k) {
      std::vector<VertexDescriptor> vertices;

      for (auto e : getOutEdges(current)) {
        assert(getSource(e) == current);
        VertexDescriptor next = getTarget(e);
        vertices.push_back(next);
      }

      if (vertices.empty()) {
        // no more candidates, we are at a final state
        break;
      }

      std::uniform_int_distribution<std::size_t> dist(0, vertices.size() - 1);
      VertexDescriptor next = vertices[dist(engine)];

      path.push_back(next);
      current = next;
    }

    return path;
  }

  Matrix<double> Graph::computeExactAlphaMatrix(std::size_t length) const {
    std::size_t count = getVertexCount();

    Matrix<double> m(count, count);

    for (auto j : getVertices()) {
      std::cout << '*' << std::flush;

      // alpha_j

      {
        auto derived = buildGraphCrossingOneVertex(*this, j);
        m(j.index, j.index) = derived.countPathOfMaximumLengthFromInitialState(length);
      }

      // alpha_i_j

      for (auto i = j.next(); i.index < count; ++i) {
        if (m(j.index, j.index) > 0) {
          auto derived = buildGraphCrossingTwoVertices(*this, i, j);
          m(i.index, j.index) = m(j.index, i.index) = derived.countPathOfMaximumLengthFromInitialState(length);
        } else {
          m(i.index, j.index) = m(j.index, i.index) = 0;
        }
      }
    }

    std::cout << '\n';
    return m;
  }

  static void matrixNormalizeByDiagonal(Matrix<double>& m) {
    assert(m.getRows() == m.getCols());
    std::size_t size = m.getRows();

    for (std::size_t j = 0; j < size; ++j) {
      if (m(j, j) <= std::numeric_limits<double>::epsilon()) {
        for (std::size_t i = 0; i < size; ++i) {
          m(i, j) = 0;
        }

        m(j, j) = 1;
      } else {
        double alphaJ = m(j, j);

        for (std::size_t i = 0; i < size; ++i) {
          m(i, j) /= alphaJ;
        }
      }
    }
  }

  Matrix<double> Graph::computeExactNormalizedAlphaMatrix(std::size_t length) const {
    auto start = std::chrono::steady_clock::now();

    auto m = computeExactAlphaMatrix(length);
    matrixNormalizeByDiagonal(m);

    auto finish = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff = finish - start;
    std::cout << "alpha_ij_over_alpha_j construction: " << diff.count() << '\n';

    return m;
  }

  Matrix<double> Graph::computeApproxAlphaMatrix(std::size_t length, std::size_t tries, Engine& engine) const {
    auto paths = computePathCountOfMaximumLength(length);
    std::size_t count = getVertexCount();

    Matrix<double> m(count, count);

    for (std::size_t i = 0; i < tries; ++i) {
      auto path = makeUniformPath(length, engine, paths);

      for (auto v : path) {
        ++m(v.index, v.index);
      }

      for (auto u : path) {
        for (auto v : path) {
          if (u != v) {
            ++m(u.index, v.index);
          }
        }
      }

    }

    return m;
  }

  Matrix<double> Graph::computeApproxNormalizedAlphaMatrix(std::size_t length, std::size_t tries, Engine& engine) const {
    auto start = std::chrono::steady_clock::now();

    auto m = computeApproxAlphaMatrix(length, tries, engine);
    matrixNormalizeByDiagonal(m);

    auto finish = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff = finish - start;
    std::cout << "alpha_ij_over_alpha_j construction: " << diff.count() << '\n';

    return m;
  }

  void Graph::clear() {
    m_nextVertexId = 0;
    m_nextEdgeId = 0;
    m_vertices.clear();
    m_edges.clear();
    m_outEdges.clear();
    m_initialState = InvalidVertexDescriptor;
    m_finalStates.clear();
  }

  Graph Graph::import(std::istream& in) {
    std::size_t count;
    in >> count;

    Graph g(count);

    for (std::size_t i = 0; i < count; ++i) {
      auto id = g.addVertex();
      g.addFinalState(id);
    }

    VertexDescriptor init;
    in >> init.index;
    g.setInitialState(init);

    VertexDescriptor source, target;

    while (in >> source.index >> target.index) {
      g.addEdge(source, target);
    }

    return g;
  }

  DerivedGraph buildGraphCrossingOneVertex(const Graph& origin, VertexDescriptor x) {
    std::size_t count = origin.getVertexCount();

    auto prime = [count](VertexDescriptor v) -> VertexDescriptor { return { v.index + count }; };

    DerivedGraph derived(2 * count);

    // vertices

    for (auto v : origin.getVertices()) {
      auto id = derived.addVertex(v);
      assert(id == v);
    }

    for (auto v : origin.getVertices()) {
      auto id = derived.addVertex(v);
      assert(id == prime(v));
    }

    // edges

    for (auto e : origin.getEdges()) {
      auto source = origin.getSource(e);
      auto target = origin.getTarget(e);

      if (source != x) {
        derived.addEdge(source, target, e);
      } else {
        derived.addEdge(source, prime(target), e);
      }

      derived.addEdge(prime(source), prime(target), e);
    }

    // initial and final states

    derived.setInitialState(origin.getInitialState());

    for (auto v : origin.getFinalStates()) {
      derived.addFinalState(prime(v));

      if (v == x) {
        derived.addFinalState(v);
      }
    }

    return derived;
  }

  DerivedGraph buildGraphCrossingTwoVertices(const Graph& origin, VertexDescriptor x, VertexDescriptor y) {
    std::size_t count = origin.getVertexCount();

    auto prime = [count](VertexDescriptor v) -> VertexDescriptor { return { v.index + count }; };
    auto doublePrime = [count](VertexDescriptor v) -> VertexDescriptor { return { v.index + 2 * count }; };
    auto triplePrime = [count](VertexDescriptor v) -> VertexDescriptor { return { v.index + 3 * count }; };

    DerivedGraph derived(4 * count);

    // vertices

    for (auto v : origin.getVertices()) {
      auto id = derived.addVertex(v);
      assert(id == v);
    }

    for (auto v : origin.getVertices()) {
      auto id = derived.addVertex(v);
      assert(id == prime(v));
    }

    for (auto v : origin.getVertices()) {
      auto id = derived.addVertex(v);
      assert(id == doublePrime(v));
    }

    for (auto v : origin.getVertices()) {
      auto id = derived.addVertex(v);
      assert(id == triplePrime(v));
    }

    // edges

    for (auto e : origin.getEdges()) {
      auto source = origin.getSource(e);
      auto target = origin.getTarget(e);

      if (source != x && source != y) {
        derived.addEdge(source, target, e);
        derived.addEdge(prime(source), prime(target), e);
        derived.addEdge(doublePrime(source), doublePrime(target), e);
      } else {
        if (source == x) {
          derived.addEdge(source, prime(target), e);
          derived.addEdge(prime(source), prime(target), e);
          derived.addEdge(doublePrime(source), triplePrime(target), e);
        } else {
          assert(source == y);
          derived.addEdge(source, doublePrime(target), e);
          derived.addEdge(prime(source), triplePrime(target), e);
          derived.addEdge(doublePrime(source), doublePrime(target), e);
        }
      }

      derived.addEdge(triplePrime(source), triplePrime(target), e);
    }

    // initial and final states

    derived.setInitialState(origin.getInitialState());

    for (auto v : origin.getFinalStates()) {
      derived.addFinalState(triplePrime(v));

      if (v == x) {
        derived.addFinalState(doublePrime(v));
      }

      if (v == y) {
        derived.addFinalState(prime(v));
      }
    }

    return derived;
  }

}
