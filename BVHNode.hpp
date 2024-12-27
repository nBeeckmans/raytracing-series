#pragma once

#include "AABB.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"

#include <algorithm>

class BVHNode : public Hittable {
private: 
	shared_ptr<Hittable> _left;
	shared_ptr<Hittable> _right;
	AABB _boundingBox;

public:
	BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {}

	BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
		_boundingBox = AABB::empty;

		for (size_t objectIndex = start; objectIndex < end; ++objectIndex) {
			_boundingBox = AABB(_boundingBox, objects[objectIndex]->boundingBox());
		}
		int axis = _boundingBox.longestAxis();

		auto comparator = (axis == 0) ? boxXCompare
			: (axis == 1) ? boxYCompare
			: boxZCompare;

		size_t objectSpan = end - start;

		if (objectSpan == 1) {
			_left = _right = objects[start];
		}
		else if (objectSpan == 2) {
			_left = objects[start];
			_right = objects[start+1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			auto mid = start + objectSpan / 2;
			_left = make_shared<BVHNode>(objects, start, mid);
			_right = make_shared<BVHNode>(objects, mid, end);
		}

	}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override {
		if (!_boundingBox.hit(r, rayT)) {
			return false;
		}

		bool hitLeft = _left->hit(r, rayT, record);
		bool hitRight = _right->hit(r, Interval(rayT.min(), hitLeft ? record.t : rayT.max()), record);

		return hitLeft || hitRight;
	}

	AABB boundingBox() const override { return _boundingBox; }

private:
	static bool boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex) {
		auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
		auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
		return aAxisInterval.min() < bAxisInterval.min();
	}

	static bool boxXCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return boxCompare(a, b, 0);
	}

	static bool boxYCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return boxCompare(a, b, 1);
	}

	static bool boxZCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return boxCompare(a, b, 2);
	}
};