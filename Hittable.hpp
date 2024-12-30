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

class Translate : public Hittable {
private:
	shared_ptr<Hittable> _object;
	Vec3 _offset;
	AABB _boundingBox;
public:
	Translate(shared_ptr<Hittable> object, const Vec3& offset) : _object(object), _offset(offset)
	{
		_boundingBox = object->boundingBox() + offset;
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		Ray offsetR(r.origin() - _offset, r.direction(), r.getTime());

		if (!_object->hit(offsetR, rayT, record)) {
			return false;
		}

		record.p += _offset;

		return true;
	}

	AABB boundingBox() const override {
		return _boundingBox;
	}

};

class RotateY : public Hittable {
private:
	shared_ptr<Hittable> _object;
	double _sinTheta, _cosTheta;
	AABB _boundingBox;
public:
	RotateY(shared_ptr<Hittable> object, double angle) : _object(object) {
		auto radians = degreesToRadians(angle);
		_sinTheta = std::sin(radians);
		_cosTheta = std::cos(radians);
		_boundingBox = object->boundingBox();

		Point3 min(infinity, infinity, infinity);
		Point3 max(-infinity, -infinity, -infinity);

		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					auto x = i * _boundingBox.getX().max() + (1 - i) * _boundingBox.getX().min();
					auto y = j * _boundingBox.getY().max() + (1 - j) * _boundingBox.getY().min();
					auto z = k * _boundingBox.getZ().max() + (1 - k) * _boundingBox.getZ().min();

					auto newX = _cosTheta * x + _sinTheta * z;
					auto newZ = -_sinTheta * x + _cosTheta * z;

					Vec3 tester(newX, y, newZ);
					for (int c = 0; c < 3; ++c) {
						min[c] = std::fmin(min[c], tester[c]);
						max[c] = std::fmax(max[c], tester[c]);
					}
				}
			}
		}
		_boundingBox = AABB(min, max);
	}


	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		auto origin = Point3(
			(_cosTheta * r.origin().x()) - (_sinTheta * r.origin().z()),
			r.origin().y(),
			(_sinTheta * r.origin().x()) + (_cosTheta * r.origin().z())
		);

		auto direction = Point3(
			(_cosTheta * r.direction().x()) - (_sinTheta * r.direction().z()),
			(r.direction().y()),
			(_sinTheta * r.direction().x()) + (_cosTheta * r.direction().z())
		);

		Ray rotatedR(origin, direction, r.getTime());

		if (!_object->hit(rotatedR, rayT, record)) {
			return false;
		}

		record.p = Point3(
			(_cosTheta * record.p.x()) + (_sinTheta * record.p.z()),
			record.p.y(),
			(-_sinTheta * record.p.x()) + (_cosTheta * record.p.z())
		);

		record.normal = Point3(
			(_cosTheta * record.normal.x()) + (_sinTheta * record.normal.z()),
			record.normal.y(), 
			(-_sinTheta * record.normal.x()) + (_cosTheta * record.normal.z())
		);

		return true;
	}

	AABB boundingBox() const override {
		return _boundingBox;
	}
};