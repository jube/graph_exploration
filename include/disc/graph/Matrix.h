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
#ifndef DISC_MATRIX_H
#define DISC_MATRIX_H

#include <cassert>

#include <iostream>
#include <vector>

namespace disc {
  struct SizeOnlyType {
  };

  constexpr SizeOnlyType SizeOnly = SizeOnlyType();

  template<class T>
  class Matrix {
  public:

    Matrix() = default;

    Matrix(std::size_t rows, std::size_t cols)
    : m_rows(rows)
    , m_cols(cols)
    , m_data(rows * cols)
    {

    }

    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;

    template<typename U>
    Matrix(SizeOnlyType, const Matrix<U>& other)
    : Matrix(other.rows(), other.cols())
    {
    }

    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) = default;

    // element access

    T& operator()(std::size_t row, std::size_t col) {
      assert(row < m_rows);
      assert(col < m_cols);
      // return m_data[row * m_cols + col];
      return m_data[col * m_rows + row];
    }

    const T& operator()(std::size_t row, std::size_t col) const {
      assert(row < m_rows);
      assert(col < m_cols);
      // return m_data[row * m_cols + col];
      return m_data[col * m_rows + row];
    }

    // capacity

    bool isEmpty() const {
      return m_data.empty();
    }

    std::size_t getRows() const {
      return m_rows;
    }

    std::size_t getCols() const {
      return m_cols;
    }

    // modifiers

    void clear() {
      m_data.clear();
    }

    void assign(T value) {
      for (auto& content : m_data) {
        content = value;
      }
    }

    void swap(Matrix& other) {
      std::swap(m_rows, other.m_rows);
      std::swap(m_cols, other.m_cols);
      std::swap(m_data, other.m_data);
    }

  private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<T> m_data;
  };


  template<typename T>
  std::ostream& operator<<(std::ostream& o, const Matrix<T>& m) {
    for (std::size_t y = 0; y < m.getCols(); ++y) {
      for (std::size_t x = 0; x < m.getRows(); ++x) {
        o << ' ' << m(x, y);
      }
      o << '\n';
    }

    return o;
  }

}

#endif // MATRIX_H
