#ifndef DISC_RANDOM_H
#define DISC_RANDOM_H

#include <random>

namespace disc {

  using Engine = std::mt19937;

  Engine getCorrectlyInitializedEngine();

}

#endif // DISC_RANDOM_H
