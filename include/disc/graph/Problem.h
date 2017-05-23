#ifndef DISC_PROBLEM_H
#define DISC_PROBLEM_H

#include <vector>

#include "Matrix.h"

namespace disc {

  std::vector<double> computePii(const Matrix<double>& coeffs, const char *filename);

}

#endif // DISC_PROBLEM_H
