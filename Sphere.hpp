#pragma once 

#include "Hittable.hpp"
#include "Utils.hpp"

class Sphere : public Hittable {
private :
	Ray _center;
	double _radius;
	shared_ptr<Material> _material;
	AABB _boundingBox;
public:
	Sphere(const Point3& center_ori, const Point3& center_dst, double radius, shared_ptr<Material> mat) 
		: _center(center_ori, center_dst - center_ori), _radius(std::fmax(0, radius)), _material(mat) 
	{
		auto rvec = Vec3(radius, radius, radius);
		AABB box1(_center.at(0) - rvec, _center.at(0) + rvec);
		AABB box2(_center.at(1) - rvec, _center.at(1) + rvec);
		_boundingBox = AABB(box1, box2);
	}

	Sphere(const Point3& center, double radius, shared_ptr<Material> mat) 
		: _center(center, Vec3(0,0,0)), _radius(std::fmax(0, radius)), _material(mat) 
	{
		auto rvec = Vec3(radius, radius, radius);
		_boundingBox = AABB(center - rvec, center + rvec);
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		Point3 currentCenter = _center.at(r.getTime());
		Vec3 oc = currentCenter - r.origin();
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
		Vec3 outwardNormal = (record.p - currentCenter) / _radius;
		record.setFaceNormal(r, outwardNormal);
		getSphereUV(outwardNormal, record.u, record.v);
		record.material = _material;
		return true;
	}

	AABB boundingBox() const override { return _boundingBox; }

private:
	static void getSphereUV(const Point3& p, double& u, double& v) {
		auto theta = std::acos(-p.y());
		auto phi = std::atan2(-p.z(), p.x()) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}
};