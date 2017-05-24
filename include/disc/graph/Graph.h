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
#ifndef DISC_GRAPH_H
#define DISC_GRAPH_H

#include <cstdint>

#include <iosfwd>
#include <set>
#include <type_traits>
#include <vector>

#include "Range.h"
#include "Random.h"
#include "Matrix.h"

namespace disc {
  /*
   * descriptor
   */
  template<typename Tag>
  struct Descriptor {
    uint64_t index;

    Descriptor() = default;

    constexpr Descriptor(uint64_t value) noexcept
    : index(value)
    {

    }

    Descriptor& operator++() noexcept {
      ++index;
      return *this;
    }

    Descriptor operator++(int) noexcept {
      Descriptor tmp = *this;
      ++index;
      return tmp;
    }

    Descriptor next() const {
      return { index + 1 };
    }
  };

  template<typename Tag>
  constexpr bool operator<(Descriptor<Tag> lhs, Descriptor<Tag> rhs) noexcept {
    return lhs.index < rhs.index;
  }

  template<typename Tag>
  constexpr bool operator==(Descriptor<Tag> lhs, Descriptor<Tag> rhs) noexcept {
    return lhs.index == rhs.index;
  }

  template<typename Tag>
  constexpr bool operator!=(Descriptor<Tag> lhs, Descriptor<Tag> rhs) noexcept {
    return lhs.index != rhs.index;
  }

  /*
   * vertex descriptor
   */

  struct VertexTag { };

  using VertexDescriptor = Descriptor<VertexTag>;

  constexpr VertexDescriptor InvalidVertexDescriptor = { 0xFFFFFFFF };

  /*
   * edge descriptor
   */

  struct EdgeTag { };

  using EdgeDescriptor = Descriptor<EdgeTag>;

  class Graph {
  public:
    struct Vertex {
      VertexDescriptor id;
    };

    struct Edge {
      EdgeDescriptor id;
      VertexDescriptor source;
      VertexDescriptor target;
    };

    // constructor

    explicit Graph(std::size_t n = 0);

    // vertices

    VertexDescriptor addVertex();

    using VertexRange = IntegerRange<VertexDescriptor>;

    VertexRange getVertices() const;

    std::size_t getVertexCount() const;

    // edges

    EdgeDescriptor addEdge(VertexDescriptor source, VertexDescriptor target);

    using EdgeRange = IntegerRange<EdgeDescriptor>;

    EdgeRange getEdges() const;

    std::size_t getEdgeCount() const;

    VertexDescriptor getSource(EdgeDescriptor e) const;
    VertexDescriptor getTarget(EdgeDescriptor e) const;

    // out edges

    using OutEdgeRange = IteratorRange<std::multiset<EdgeDescriptor>::const_iterator>;

    OutEdgeRange getOutEdges(VertexDescriptor v) const;

    // initial and final states

    void setInitialState(VertexDescriptor v);

    VertexDescriptor getInitialState() const;

    bool isInitialState(VertexDescriptor v) const;

    void addFinalState(VertexDescriptor v);

    bool isFinalState(VertexDescriptor v) const;

    using FinalStateRange = IteratorRange<std::set<VertexDescriptor>::const_iterator>;

    FinalStateRange getFinalStates() const;

    // properties

    bool isConnected() const;

    std::size_t getEccentricity() const;

    Matrix<double> computePathCountOfExactLength(std::size_t length) const;

    Matrix<double> computePathCountOfMaximumLength(std::size_t length) const;

    double countPathOfMaximumLengthFromInitialState(std::size_t length) const;

    // operations

    std::vector<VertexDescriptor> makeUniformPath(std::size_t length, Engine& engine, const Matrix<double>& paths) const;

    std::vector<VertexDescriptor> makeRandomPath(std::size_t length, Engine& engine) const;

    Matrix<double> computeExactAlphaMatrix(std::size_t length) const;

    Matrix<double> computeExactNormalizedAlphaMatrix(std::size_t length) const;

    Matrix<double> computeApproxAlphaMatrix(std::size_t length, std::size_t tries, Engine& engine) const;

    Matrix<double> computeApproxNormalizedAlphaMatrix(std::size_t length, std::size_t tries, Engine& engine) const;

    // import

    void clear();

    static Graph import(std::istream& in);

  private:
    VertexDescriptor m_nextVertexId;
    EdgeDescriptor m_nextEdgeId;

    std::vector<Vertex> m_vertices;
    std::vector<Edge> m_edges;

    std::vector<std::multiset<EdgeDescriptor>> m_outEdges;

    VertexDescriptor m_initialState;
    std::set<VertexDescriptor> m_finalStates;
  };


  /*
   * DecoratedGraph
   */

  template<typename V, typename E>
  class DecoratedGraph : public Graph {
  public:
    DecoratedGraph(std::size_t n)
    : Graph(n)
    {
      if (n > 0) {
        m_vertexData.reserve(n);
        m_edgeData.reserve(n);
      }
    }

    VertexDescriptor addVertex(V v) {
      auto id = Graph::addVertex();
      m_vertexData.push_back(std::move(v));
      return id;
    }

    V& operator()(VertexDescriptor v) {
      return m_vertexData[v.index];
    }

    const V& operator()(VertexDescriptor v) const {
      return m_vertexData[v.index];
    }

    EdgeDescriptor addEdge(VertexDescriptor source, VertexDescriptor target, E e) {
      auto id = Graph::addEdge(source, target);
      m_edgeData.push_back(std::move(e));
      return id;
    }

    E& operator()(EdgeDescriptor e) {
      return m_edgeData[e.index];
    }

    const E& operator()(EdgeDescriptor e) const {
      return m_edgeData[e.index];
    }

    std::set<V> getVertexData(const std::vector<VertexDescriptor>& vertices) {
      std::set<V> res;

      for (auto v : vertices) {
        res.insert(m_vertexData[v.index]);
      }

      return res;
    }

    std::set<E> getEdgeData(const std::vector<EdgeDescriptor>& edges) {
      std::set<E> res;

      for (auto e : edges) {
        res.insert(m_edgeData[e.index]);
      }

      return res;
    }

  private:
    std::vector<V> m_vertexData;
    std::vector<E> m_edgeData;
  };

  using DerivedGraph = DecoratedGraph<VertexDescriptor, EdgeDescriptor>;

  DerivedGraph buildGraphCrossingOneVertex(const Graph& origin, VertexDescriptor x);
  DerivedGraph buildGraphCrossingTwoVertices(const Graph& origin, VertexDescriptor x, VertexDescriptor y);


}

#endif // DISC_GRAPH_H
