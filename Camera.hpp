#pragma once

#include "Hittable.hpp"
#include "Material.hpp"
#include <vector>

class Camera {
private: 
	Point3 _center;
	Point3 _lookfrom = Point3(0,0,0);
	Point3 _lookat = Point3(0,0,-1);
	Point3 _pixel00Location;
	Vec3 _vup = Vec3(0, 1, 0);
	Vec3 _u, _v, _w;
	Vec3 _pixelDeltaU, _pixelDeltaV;
	Vec3 _defocusDiskU;
	Vec3 _defocusDiskV;

	double _aspectRatio = 1.0;
	double _pixelSampleScale;
	double _fov = 90;
	double _defocusAngle = 0;
	double _focusDist = 10;

	int _imageWidth = 100;
	int _imageHeight;
	int _samplesPerPixel = 100;
	int _maxDepth = 10;

	std::vector<std::vector<Color>> _listColors;
public:

	void render(const Hittable& world) {
		initialize();
		
		std::cout << "P3\n" << _imageWidth << ' ' << _imageHeight << "\n255" << std::endl;

		#pragma omp parallel for
		for (int j = 0; j < _imageHeight; ++j) {
			for (int i = 0; i < _imageWidth; ++i) {
				Color pixelColor(0, 0, 0);
				for (int sample = 0; sample < _samplesPerPixel; ++sample) {
					Ray r = getRay(i, j);
					pixelColor += rayColor(r, _maxDepth, world);
				}
				_listColors[j][i] = _pixelSampleScale * pixelColor;
			}
		}

		for (int j = 0; j < _imageHeight; ++j) {
			for (int i = 0; i < _imageWidth; ++i) {
				writeColor(std::cout, _listColors[j][i]);
			}
		}

		std::clog << "\rDone.				\n";
	}

	void setRatio(double ratio) {
		this->_aspectRatio = ratio;
	};

	void setWidth(int width) {
		this->_imageWidth = width;
	}

	void setSamplesPerPixel(int samples) {
		this->_samplesPerPixel = samples;
	}

	void setMaxDepth(int depth) {
		this->_maxDepth = depth;
	}

	void setFov(double fov) {
		this->_fov = fov;
	}

	void setLookfrom(const Point3& from) {
		this->_lookfrom = from;
	}

	void setLookat(const Point3& at) {
		this->_lookat = at;
	}

	void setVup(const Vec3& up) {
		this->_vup = up;
	}

	void setDefocusAngle(double angle) {
		this->_defocusAngle = angle;
	}
	
	void setFocusDist(double focus) {
		this->_focusDist = focus;
	}

private:
	void initialize() {
		_imageHeight = int(_imageWidth / _aspectRatio);
		_imageHeight = (_imageHeight < 1) ? 1 : _imageHeight;

		_center = _lookfrom;

		_pixelSampleScale = 1.0 / _samplesPerPixel;

		auto theta = degreesToRadians(_fov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2 * h * _focusDist;
		auto viewportWidth = viewportHeight * (double(_imageWidth) / _imageHeight);

		_w = unitVector(_lookfrom - _lookat);
		_u = unitVector(cross(_vup, _w));
		_v = cross(_w, _u);

		auto viewportU = viewportWidth * _u;
		auto viewportV = viewportHeight * -_v;

		_pixelDeltaU = viewportU / _imageWidth;
		_pixelDeltaV = viewportV / _imageHeight;

		auto viewportUpperLeft = _center - (_focusDist * _w) - viewportU / 2 - viewportV / 2;

		_pixel00Location = viewportUpperLeft + 0.5 * (_pixelDeltaU + _pixelDeltaV);

		auto defocusRadius = _focusDist * std::tan(degreesToRadians(_defocusAngle / 2));
		_defocusDiskU = _u * defocusRadius;
		_defocusDiskV = _v * defocusRadius;
		
		_listColors.resize(_imageHeight);
		for (auto& list : _listColors) {
			list.resize(_imageWidth);
		}
	}

	Color rayColor(const Ray& r, int depth, const Hittable& world) {
		if (depth <= 0) {
			return Color(0, 0, 0);
		}
		HitRecord record;

		if (world.hit(r, Interval(0.001, infinity), record)) {
			Ray scattered;
			Color attenuation;

			if (record.material->scatter(r, record, attenuation, scattered))
				return attenuation * rayColor(scattered, (depth - 1), world);
			return Color(0,0,0);
		}

		Vec3 unitDirection = unitVector(r.direction());
		auto a = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}
	
	Ray getRay(int i, int j) const {
		auto offset = sampleSquare();
		auto pixelSample = _pixel00Location
			+ ((i + offset.x()) * _pixelDeltaU)
			+ ((j + offset.y()) * _pixelDeltaV);

		auto rayOrigin = (_defocusAngle <= 0) ? _center : defocusDiskSample(); 
		auto rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 sampleSquare() const {
		return Vec3(randomDouble() - .5, randomDouble() - .5, 0);
	}

	Point3 defocusDiskSample() const {
		auto p = randomInUnitDisk();
		return _center + (p[0] * _defocusDiskU) + (p[1] * _defocusDiskV);
	}
};