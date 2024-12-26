#pragma once

#include "Hittable.hpp"

class Material {
public:
	virtual ~Material() = default;
	virtual bool scatter(const Ray& rIn, const HitRecord& record, Color& attenuation, Ray& scattered) const { return false; }
};

class Lambertian : public Material {
private:
	Color _albedo;
public:
	Lambertian(const Color& albedo) : _albedo (albedo) {}

	bool scatter(const Ray& rIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
		auto scatterDirection = record.normal + randomUnitVector();

		if (scatterDirection.nearZero()) {
			scatterDirection = record.normal;
		}

		scattered = Ray(record.p, scatterDirection);
		attenuation = _albedo;
		return true; 
	}
};

class Metal : public Material {
private:
	Color _albedo;
	double _fuzz;
public:
	Metal(const Color& albedo, double fuzz) : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& rIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
		Vec3 reflected = reflect(rIn.direction(), record.normal);
		reflected = unitVector(reflected) + (_fuzz * randomUnitVector());
		scattered = Ray(record.p, reflected);
		attenuation = _albedo;
		return (dot(scattered.direction(), record.normal) > 0); 
	}
};

class Dielectric : public Material {
private:
	double _refractionIndex;
public:
	Dielectric(double refractionIndex) : _refractionIndex(refractionIndex) {}
	bool scatter(const Ray& rIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
		attenuation = Color(1.0, 1.0, 1.0);

		double ri = record.frontFace ? (1.0 / _refractionIndex) : _refractionIndex;
		Vec3 unitDirection = unitVector(rIn.direction());

		double cosTheta = std::fmin(dot(-unitDirection, record.normal), 1.0);
		double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = ri * sinTheta > 1.0;
		Vec3 direction;
		

		if (cannotRefract  || reflectance(cosTheta, ri) > randomDouble()) {
			direction = reflect(unitDirection, record.normal);
		}
		else {
			direction = refract(unitDirection, record.normal, ri);
		}
		scattered = Ray(record.p, direction);
		return true;
	}

	static double reflectance(double cosine, double refractionIndex) {
		auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};