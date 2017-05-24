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
#include <disc/graph/Metrics.h>

#include <algorithm>
#include <iostream>

namespace disc {

  MeanMetrics computeMeanMetrics(const std::vector<Metrics>& results) {
    MeanMetrics res;

    auto order_50 = [](const Metrics& lhs, const Metrics& rhs) { return lhs.covered_50 < rhs.covered_50; };
    res.covered_50.min = std::min_element(results.begin(), results.end(), order_50)->covered_50;
    res.covered_50.max = std::max_element(results.begin(), results.end(), order_50)->covered_50;

    auto order_90 = [](const Metrics& lhs, const Metrics& rhs) { return lhs.covered_90 < rhs.covered_90; };
    res.covered_90.min = std::min_element(results.begin(), results.end(), order_90)->covered_90;
    res.covered_90.max = std::max_element(results.begin(), results.end(), order_90)->covered_90;

    auto order_95 = [](const Metrics& lhs, const Metrics& rhs) { return lhs.covered_95 < rhs.covered_95; };
    res.covered_95.min = std::min_element(results.begin(), results.end(), order_95)->covered_95;
    res.covered_95.max = std::max_element(results.begin(), results.end(), order_95)->covered_95;

    auto order_99 = [](const Metrics& lhs, const Metrics& rhs) { return lhs.covered_99 < rhs.covered_99; };
    res.covered_99.min = std::min_element(results.begin(), results.end(), order_99)->covered_99;
    res.covered_99.max = std::max_element(results.begin(), results.end(), order_99)->covered_99;

    auto order_100 = [](const Metrics& lhs, const Metrics& rhs) { return lhs.covered_100 < rhs.covered_100; };
    res.covered_100.min = std::min_element(results.begin(), results.end(), order_100)->covered_100;
    res.covered_100.max = std::max_element(results.begin(), results.end(), order_100)->covered_100;

    double total_50 = 0.0;
    double total_90 = 0.0;
    double total_95 = 0.0;
    double total_99 = 0.0;
    double total_100 = 0.0;

    for (auto m : results) {
      total_50 += m.covered_50;
      total_90 += m.covered_90;
      total_95 += m.covered_95;
      total_99 += m.covered_99;
      total_100 += m.covered_100;
    }

    res.covered_50.avg = total_50 / results.size();
    res.covered_90.avg = total_90 / results.size();
    res.covered_95.avg = total_95 / results.size();
    res.covered_99.avg = total_99 / results.size();
    res.covered_100.avg = total_100 / results.size();

    return res;
  }

  std::ostream& operator<<(std::ostream& o, const MeanMetrics& mean) {
    o << "50%:  " << mean.covered_50.min << '/' << mean.covered_50.max << '/' << mean.covered_50.avg << '\n';
    o << "90%:  " << mean.covered_90.min << '/' << mean.covered_90.max << '/' << mean.covered_90.avg << '\n';
    o << "95%:  " << mean.covered_95.min << '/' << mean.covered_95.max << '/' << mean.covered_95.avg << '\n';
    o << "99%:  " << mean.covered_99.min << '/' << mean.covered_99.max << '/' << mean.covered_99.avg << '\n';
    o << "100%: " << mean.covered_100.min << '/' << mean.covered_100.max << '/' << mean.covered_100.avg;
    return o;
  }

}
