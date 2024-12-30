#pragma once

#include "Hittable.hpp"
#include "Utils.hpp"

class ConstantMedium : public Hittable {
private:
	double _negInvDensity;
	shared_ptr<Hittable> _boundary;
	shared_ptr<Material> _phaseFunction;
public:
	ConstantMedium(shared_ptr<Hittable> boundary, double density, shared_ptr<Texture> tex)
		: _boundary(boundary), _negInvDensity(-1 / density), _phaseFunction(make_shared<Isotropic>(tex))
	{}

	ConstantMedium(shared_ptr<Hittable> boundary, double density, const Color& albedo)
		: _boundary(boundary), _negInvDensity(-1 / density), _phaseFunction(make_shared<Isotropic>(albedo))
	{}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		HitRecord rec1, rec2;

		if (!_boundary->hit(r, Interval::universe, rec1)) {
			return false;
		}

		if (!_boundary->hit(r, Interval(rec1.t + 0.0001, infinity), rec2)) {
			return false;
		}

		if (rec1.t < rayT.min()) rec1.t = rayT.min();
		if (rec1.t > rayT.max()) rec2.t = rayT.max();

		if (rec1.t >= rec2.t) {
			return false;
		}

		if (rec1.t < 0) {
			rec1.t = 0;
		}

		auto rayLength = r.direction().length();
		auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
		auto hitDistance = _negInvDensity * std::log(randomDouble());

		if (hitDistance > distanceInsideBoundary) {
			return false;
		}

		record.t = rec1.t + hitDistance / rayLength;
		record.p = r.at(record.t);

		record.normal = Vec3(1, 0, 0);
		record.frontFace = true;
		record.material = _phaseFunction;

		return true;
	}

	AABB boundingBox() const override { return _boundary->boundingBox(); };

};