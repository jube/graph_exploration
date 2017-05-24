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
#include <disc/graph/Random.h>

#include <algorithm>
#include <functional>
#include <iterator>

namespace disc {

  // see http://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
  // and http://www.pcg-random.org/posts/cpp-seeding-surprises.html
  Engine getCorrectlyInitializedEngine() {
    std::mt19937::result_type data[std::mt19937::state_size];
    std::random_device source;
    std::generate(std::begin(data), std::end(data), std::ref(source));
    std::seed_seq seeds(std::begin(data), std::end(data));
    return std::mt19937(seeds);
  }

}
