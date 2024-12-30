#pragma once

#include "Hittable.hpp"
#include "HittableList.hpp"

class Quad : public Hittable {
private:
	Point3 _Q;
	Vec3 _u;
	Vec3 _v;
	Vec3 _w;
	Vec3 _normal;
	shared_ptr<Material> _material;
	AABB _boundingBox;
	double _D;
public:
	Quad(const Point3& Q, const Vec3& u, const Vec3& v, shared_ptr<Material> material) 
		: _Q(Q), _u(u), _v(v), _material(material)
	{
		auto n = cross(u, v);
		_normal = unitVector(n);
		_w = n / dot(n, n);
		_D = dot(_normal, Q);
		setBoundingBox();
	}

	virtual void setBoundingBox() {
		auto boundingBoxDiagonal1 = AABB(_Q, _Q + _u + _v);
		auto boundingBoxDiagonal2 = AABB(_Q + _u, _Q  + _v);
		_boundingBox = AABB(boundingBoxDiagonal1, boundingBoxDiagonal2);
	}

	AABB boundingBox() const override { return _boundingBox; }

	bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
		auto denom = dot(_normal, r.direction());

		if (std::fabs(denom) < 1e-8)
			return false;

		auto t = (_D - dot(_normal, r.origin())) / denom;
		if (!rayT.contains(t))
			return false;

		auto intersection = r.at(t);
		Vec3 planarHitptVector = intersection - _Q;
		auto alpha = dot(_w, cross(planarHitptVector, _v));
		auto beta = dot(_w, cross(_u, planarHitptVector));
		if (!isInterior(alpha, beta, rec)) {
			return false;
		}

		rec.t = t;
		rec.p = intersection;
		rec.material = _material;
		rec.setFaceNormal(r, _normal);

		return true;
	}

	virtual bool isInterior(double a, double b, HitRecord& record) const {
		auto unitInterval = Interval(0, 1);

		if (!unitInterval.contains(a) || !unitInterval.contains(b)) {
			return false;
		}

		record.u = a; 
		record.v = b;

		return true;
	}
};

inline shared_ptr<HittableList> box(const Point3& a, const Point3& b, shared_ptr<Material> mat) {
	auto sides = make_shared<HittableList>();

	auto min = Point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
	auto max = Point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));
	
	auto dx = Vec3(max.x() - min.x(), 0, 0);
	auto dy = Vec3(0, max.y() - min.y(), 0);
	auto dz = Vec3( 0, 0, max.z() - min.z());

	sides->add(make_shared<Quad>(Point3(min.x(), min.y(), max.z()), dx, dy, mat));
	sides->add(make_shared<Quad>(Point3(max.x(), min.y(), max.z()), -dz, dy, mat));
	sides->add(make_shared<Quad>(Point3(max.x(), min.y(), min.z()), -dx, dy, mat));
	sides->add(make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dz, dy, mat));
	sides->add(make_shared<Quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, mat));
	sides->add(make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dx, dz, mat));

	return sides;
}
