#include <iostream>

#include "Color.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

bool hitSphere(const Point3& center, double radius, const Ray& r) {
	Vec3 oc = center - r.origin();

	auto a = dot(r.direction(), r.direction());
	auto b = -2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c; 
	return discriminant >= 0;

}

Color rayColor(const Ray& r) {
	if (hitSphere(Point3(0, 0, -1), 0.5, r))
		return Color(1, 0, 0);

	Vec3 unitDirection = unitVector(r.direction());
	auto a = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - a) * Color(1.0,1.0,1.0) + a * Color(0.5, 0.7,1.0);
}

int main(void) {
	// Image
	int imageWidth = 400;
	auto aspectRatio = double(16) / double(9);
	int imageHeight = int(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// Camera
	auto focalLength = 1.0;
	auto viewportHeight = 2.0;
	auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
	auto cameraCenter = Point3(0, 0, 0);

	auto viewportU = Vec3(viewportWidth, 0, 0);
	auto viewportV = Vec3(0, -viewportHeight, 0);

	auto pixelDeltaU = viewportU / imageWidth;
	auto pixelDeltaV = viewportV / imageHeight;

	auto viewportUpperLeft = cameraCenter - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
	
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (auto j = 0; j < imageHeight; ++j) {
		std::clog << "\rScanlines remaining: " << (imageHeight - j) << std::endl;
		for (auto i = 0; i < imageWidth; ++i) {
			auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
			auto rayDirection = pixelCenter - cameraCenter;
			Ray r(cameraCenter, rayDirection);

			Color pixelColor = rayColor(r);
			writeColor(std::cout, pixelColor); 
		}
	}
	std::clog << "\rDone.				\n"; 
}