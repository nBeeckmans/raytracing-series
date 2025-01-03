#pragma once

#include "Utils.hpp"

#include "AABB.hpp"
#include "Hittable.hpp"
#include <vector>

class HittableList : public Hittable {
private:
	AABB _boundingBox;
public:
	std::vector<shared_ptr<Hittable>> objects;
	HittableList() = default; 
	HittableList(shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(shared_ptr<Hittable> object) {
		objects.push_back(object);
		_boundingBox = AABB(_boundingBox, object->boundingBox());
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		HitRecord tempRec; 
		bool hitAnything = false;
		auto closestSoFar = rayT.max();

		for (const auto& object : objects) {
			if (object->hit(r, Interval(rayT.min(), closestSoFar), tempRec)) {
				hitAnything = true; 
				closestSoFar = tempRec.t;
				record = tempRec;
			}
		}
		return hitAnything;
	}

	AABB boundingBox() const override { return _boundingBox; }

};