#pragma once

#include "Utils.hpp"
#include "AABB.hpp"
class Material;

struct HitRecord {
	Point3 p; 
	Vec3 normal; 
	shared_ptr<Material> material;
	double t;
	double u, v;
	bool frontFace;

	void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
		frontFace = dot(r.direction(), outwardNormal) < 0; 
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public : 
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;

	virtual AABB boundingBox() const = 0;
};