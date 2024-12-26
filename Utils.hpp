#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

inline double randomDouble() {
	thread_local static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	thread_local static std::mt19937 generator;
	return distribution(generator);
}

inline double randomDouble(double min, double max) {
	return min + (max - min) * randomDouble();
}

#include "Color.hpp"
#include "Interval.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"