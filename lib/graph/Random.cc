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
