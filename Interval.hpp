#pragma once 

class Interval {
private:
	double _min, _max;
public:

	Interval() : _min(+infinity), _max(-infinity) {}

	Interval(double min, double max) : _min(min), _max(max) {}

	Interval(const Interval& a, const Interval& b) {
		_min = a.min() <= b.min() ? a.min() : b.min();
		_max = a.max() >= b.max() ? a.max() : b.max();
	}

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

	Interval expand(double delta) const {
		auto padding = delta / 2;
		return Interval(_min - padding, _max + padding);
	}
	
	double min() const {
		return _min;
	}

	double max() const {
		return _max;
	}

	void setMin(double min) {
		this->_min = min;
	}

	void setMax(double max) {
		this->_max = max;
	}

	static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

Interval operator+(const Interval& interval, double displacement) {
	return Interval(interval.min() + displacement, interval.max() + displacement);
}

Interval operator+(double displacement, const Interval& interval) {
	return interval + displacement;
}