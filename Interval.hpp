#pragma once 

class Interval {
private:
	double _min, _max;
public:

	Interval() : _min(+infinity), _max(-infinity) {}

	Interval(double min, double max) : _min(min), _max(max) {}

	double size() const {
		return _max - _min;
	}

	bool contains(double x) const {
		return _min <= x && x <= _max;
	}

	bool surrounds(double x) const {
		return _min < x && x < _max;
	}

	double clamp(double x) const {
		if (x < _min) return _min;
		if (x > _max) return _max;

		return x;
	}
	
	double min() {
		return _min;
	}

	double max() {
		return _max;
	}

	static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);