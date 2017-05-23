#ifndef DISC_MATRIX_H
#define DISC_MATRIX_H

#include <cassert>
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



}

#endif // MATRIX_H
