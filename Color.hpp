#pragma once

#include "Vec3.hpp"
#include "Interval.hpp"
#include "Utils.hpp"

using Color = Vec3; 

inline double linearToGamma(double linearComponent) {
	if (linearComponent > 0) {
		return std::sqrt(linearComponent);
	}
	return 0;
}

void writeColor(std::ostream& out, const Color& pixel_color) {
	auto r = linearToGamma(pixel_color.x()); 
	auto g = linearToGamma(pixel_color.y());
	auto b = linearToGamma(pixel_color.z());

	static const Interval intensity(0.000, 0.999);
	int rByte = int(256 * intensity.clamp(r));
	int gByte = int(256 * intensity.clamp(g));
	int bByte = int(256 * intensity.clamp(b));

	out << rByte << ' ' << gByte << ' ' << bByte << std::endl;
}