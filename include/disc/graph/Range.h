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
