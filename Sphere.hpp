#pragma once 

#include "Hittable.hpp"
#include "Utils.hpp"

class Sphere : public Hittable {
private :
	Point3 _center;
	double _radius;
	shared_ptr<Material> _material;
public:
	Sphere(const Point3& center, double radius, shared_ptr<Material> mat) 
		: _center(center), _radius(std::fmax(0, radius)), _material(mat) {}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		Vec3 oc = _center - r.origin();
		auto a = r.direction().lengthSquared();
		auto h = dot(r.direction(), oc);
		auto c = oc.lengthSquared() - _radius * _radius;

		auto discriminant = h * h - a * c;
		if (discriminant < 0)
			return false;
		
		auto squrtd = std::sqrt(discriminant);

		auto root = (h - squrtd) / a;
		if (!rayT.surrounds(root)) {
			root = (h + squrtd) / a;
			if (!rayT.surrounds(root))
				return false;
		}

		record.t = root; 
		record.p = r.at(record.t);
		Vec3 outwardNormal = (record.p - _center) / _radius;
		record.setFaceNormal(r, outwardNormal);
		record.material = _material;
		return true;
	}
};