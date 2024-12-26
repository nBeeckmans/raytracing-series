#pragma once

#include "Utils.hpp"

#include "Hittable.hpp"
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<shared_ptr<Hittable>> objects;
	HittableList() = default; 
	HittableList(shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(shared_ptr<Hittable> object) {
		objects.push_back(object);
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

};