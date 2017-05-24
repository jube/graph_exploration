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
#ifndef DISC_METRICS_H
#define DISC_METRICS_H

#include <cstddef>

#include <iosfwd>
#include <vector>

namespace disc {

  struct Metrics {
    std::size_t covered_50;
    std::size_t covered_90;
    std::size_t covered_95;
    std::size_t covered_99;
    std::size_t covered_100;
  };

  struct MinMaxAvg {
    double min;
    double max;
    double avg;
  };

  struct MeanMetrics {
    MinMaxAvg covered_50;
    MinMaxAvg covered_90;
    MinMaxAvg covered_95;
    MinMaxAvg covered_99;
    MinMaxAvg covered_100;
  };

  MeanMetrics computeMeanMetrics(const std::vector<Metrics>& results);

  std::ostream& operator<<(std::ostream& o, const MeanMetrics& mean);

}

#endif // DISC_METRICS_H
