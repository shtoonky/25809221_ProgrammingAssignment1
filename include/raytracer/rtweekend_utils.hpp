#ifndef RTWEEKEND_UTILS_HPP
#define RTWEEKEND_UTILS_HPP

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();

// Returns a random real [0, 1).
inline double RandomDouble() {
    return std::rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min, max).
inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble();
}

#endif