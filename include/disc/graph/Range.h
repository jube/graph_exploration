#ifndef DISC_RANGE_H
#define DISC_RANGE_H

#include <type_traits>

namespace disc {

  template<class T>
  class IntegerRange {
  public:
    using IntegerType = T;

    IntegerRange(IntegerType beg, IntegerType end)
    : m_beg(beg), m_end(end)
    {
    }

    struct iterator {
      iterator& operator++() {
        ++value;
        return *this;
      }

      IntegerType operator*() {
        return value;
      }

      bool operator!=(const iterator& other) {
        return value != other.value;
      }

      IntegerType value;
    };

    iterator begin() const {
      return { m_beg };
    }

    iterator end() const {
      return { m_end };
    }

  private:
    IntegerType m_beg;
    IntegerType m_end;
  };

  template<class T>
  inline
  IntegerRange<T> makeIntegerRange(T beg, T end) {
    return IntegerRange<T>(beg, end);
  }

  template<class T>
  class IteratorRange {
  public:
    using iterator = T;

    IteratorRange(iterator beg, iterator end)
    : m_beg(beg), m_end(end)
    {
    }

    iterator begin() {
      return m_beg;
    }

    iterator end() {
      return m_end;
    }

  private:
    T m_beg;
    T m_end;
  };

  template<class T>
  inline
  IteratorRange<typename T::iterator> makeIteratorRange(T& cont) {
    return IteratorRange<typename T::iterator>(cont.begin(), cont.end());
  }

  template<class T>
  inline
  IteratorRange<typename T::const_iterator> makeIteratorRange(const T& cont) {
    return IteratorRange<typename T::const_iterator>(cont.begin(), cont.end());
  }

}

#endif // DISC_RANGE_H
