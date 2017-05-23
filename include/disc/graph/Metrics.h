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
