#pragma once

#include "Vec3.hpp"

#include <iostream>

using Color = Vec3; 

void writeColor(std::ostream& out, const Color& pixel_color) {
	auto r = pixel_color.x(); 
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	int rByte = int(255.999 * r);
	int gByte = int(255.999 * g);
	int bByte = int(255.999 * b);

	out << rByte << ' ' << gByte << ' ' << bByte << std::endl;
}