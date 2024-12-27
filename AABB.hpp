#pragma once

#include "Utils.hpp"

class AABB {
private: 
	Interval _x, _y, _z;
public:
	AABB() = default;

	AABB(const Interval& x, const Interval& y, const Interval& z) : _x(x), _y(y), _z(z) {}

	AABB(const Point3& a, const Point3& b) {
		_x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
		_y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
		_z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
	}

	AABB(const AABB& b1, const AABB& b2) {
		_x = Interval(b1._x, b2._x);
		_y = Interval(b1._y, b2._y);
		_z = Interval(b1._z, b2._z);
	}

	const Interval& axisInterval(int n) const {
		if (n == 1) return _y;
		if (n == 2) return _z;
		return _x;
	}

	bool hit(const Ray& r, Interval rayT) const {
		const Point3& rayOrig = r.origin();
		const Vec3& rayDirection = r.direction();

		for (int axis = 0; axis < 3; ++axis) {
			const Interval& ax = axisInterval(axis);
			const double adinv = 1.0 / rayDirection[axis];

			auto t0 = (ax.min() - rayOrig[axis]) * adinv;
			auto t1 = (ax.max() - rayOrig[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > rayT.min()) rayT.setMin(t0);
				if (t1 < rayT.max()) rayT.setMax(t1);
			}
			else {
				if (t1 > rayT.min()) rayT.setMin(t1);
				if (t0 < rayT.max()) rayT.setMax(t0);
			}

			if (rayT.max() <= rayT.min()) {
				return false;
			}
		}

		return true;
	}

	int longestAxis() const {
		if (_x.size() > _y.size()) {
			return _x.size() > _z.size() ? 0 : 2;
		}
		else {
			return _y.size() > _z.size() ? 1 : 2;
		}
	}
	static const AABB empty, universe;
};

const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);