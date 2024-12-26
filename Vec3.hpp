#pragma once 

#include <cassert>
#include "Utils.hpp"

class Vec3 {
public:
	double e[3];
	Vec3() : e{ 0,0,0 } {}
	Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	double operator[] (int i) const { assert(i > -1 && i < 3); return e[i]; }
	double& operator[] (int i) { assert(i> -1 && i < 3); return e[i]; }

	Vec3& operator+=(const Vec3& o) {
		e[0] += o.e[0];
		e[1] += o.e[1];
		e[2] += o.e[2];
		return *this;
	}

	Vec3& operator*=(double t) {
		e[0] *= t; 
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vec3& operator/=(double t) {
		return *this *= 1 / t;
	}

	double length() const {
		return std::sqrt(lengthSquared()); 
	}

	double lengthSquared() const {
		return e[0] * e[0]
			+ e[1] * e[1]
			+ e[2] * e[2];
	}	

	bool nearZero() const {
		auto s = 1e-8;
		return (std::fabs(e[0] < s)) && (std::fabs(e[1] < s)) && (std::fabs(e[2] < s));
	}
	
	static Vec3 random() {
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	static Vec3 random(double min, double max) {
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}
	
};

using Point3 = Vec3; 

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2]; 
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] + v2.e[0],  v1.e[1] + v2.e[1],  v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] * v2.e[0],  v1.e[1] * v2.e[1],  v1.e[2] * v2.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
	return t * v; 
}

inline Vec3 operator/(const Vec3& v, double t) {
	return (1 / t) * v; 
}

inline double dot(const Vec3& v1, const Vec3& v2) {
	return v1.e[0] * v2.e[0]
		+ v1.e[1] * v2.e[1]
		+ v1.e[2] * v2.e[2]; 
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
	);
}

inline Vec3 unitVector(const Vec3& v) {
	return v / v.length(); 
}

inline Vec3 randomUnitVector() {
	while (true) {
		auto p = Vec3::random(-1, 1);
		auto lensq = p.lengthSquared();
		if (1e-160 < lensq && lensq <= 1) {
			return p / sqrt(lensq);
		}
	}
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector();
	if (dot(onUnitSphere, normal) > 0.0) {
		return onUnitSphere;
	}
	else {
		return -onUnitSphere;
	}
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etaiOverEtat) {
	auto cosTheta = std::fmin(dot(-uv, n), 1.0);
	Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
	Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.lengthSquared())) * n;
	return rOutPerp + rOutParallel;
}

inline Vec3 randomInUnitDisk() {
	while (true) {
		auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
		if (p.lengthSquared() < 1) {
			return p;
		}
	}
}
