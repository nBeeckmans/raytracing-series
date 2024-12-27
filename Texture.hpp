#pragma once

#include "Utils.hpp"
#include "Image.hpp"
#include "Perlin.hpp"

class Texture {
public:
	virtual ~Texture() = default;
	virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
private:
	Color _albedo;

public :
	SolidColor(const Color& albedo) : _albedo(albedo) {}
	
	SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

	Color value(double u, double v, const Point3& p) const override {
		return _albedo;
	}
};

class CheckerTexture : public Texture {
private:
	double _invScale;
	shared_ptr<Texture> _even;
	shared_ptr<Texture> _odd;
public:
	CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) : _invScale(1.0 / scale), _even(even), _odd(odd) {}

	CheckerTexture(double scale, const Color& c1, const Color& c2) : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	Color value(double u, double v, const Point3& p) const override {
		auto xInteger = int(std::floor(_invScale * p.x()));
		auto yInteger = int(std::floor(_invScale * p.y()));
		auto zInteger = int(std::floor(_invScale * p.z()));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? _even->value(u, v, p) : _odd->value(u, v, p);
	}
};

class ImageTexture : public Texture {
	Image _image;
public:
	ImageTexture(const char* filename) : _image(filename) {}


	Color value(double u, double v, const Point3& p) const override {
		if (_image.height() <= 0) return Color(0, 1, 1);

		u = Interval(0, 1).clamp(u);
		v = 1.0 - Interval(0, 1).clamp(v);

		auto i = int(u * _image.width());
		auto j = int(v * _image.height());
		auto pixel = _image.pixelData(i, j);
		
		auto colorScale = 1.0 / 255.0;
		return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}
};

class NoiseTexture : public Texture {
public:
	NoiseTexture(double scale) : _scale(scale){}

	Color value(double u, double v, const Point3& p) const override {
		return Color(0.5, 0.5, 0.5) * (1 + std::sin(_scale * p.z() + 10 * _noise.turb(p, 7)));
	}

private :
	Perlin _noise;
	double _scale;

};