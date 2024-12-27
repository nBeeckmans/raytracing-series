#pragma once

#include "Utils.hpp"

class Perlin {
private:
	static const int pointCount = 256;
	Vec3 _randfloat[pointCount];
	int _permX[pointCount];
	int _permY[pointCount];
	int _permZ[pointCount];
public:
	Perlin() {
		for (int i = 0; i < pointCount; ++i) {
			_randfloat[i] = unitVector(Vec3::random(-1,1));
		}

		perlinGeneratePerm(_permX);
		perlinGeneratePerm(_permY);
		perlinGeneratePerm(_permZ);
	}

	double noise(const Point3& p) const {
		auto u = p.x() - std::floor(p.x());
		auto v = p.y() - std::floor(p.y());
		auto w = p.z() - std::floor(p.z());

		auto i = int(std::floor(p.x()));
		auto j = int(std::floor(p.y()));
		auto k = int(std::floor(p.z()));

		Vec3 c[2][2][2];

		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = _randfloat[_permX[(i + di) & 255] ^ _permY[(j + dj) & 255] ^ _permZ[(k + dk) & 255]];
				}
			}
		}

		return trilinearInterpolation(c, u, v, w);
	}

	double turb(const Point3& p, int depth) const {
		auto accumulator = 0.0;
		auto tempP = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; ++i) {
			accumulator += weight * noise(tempP);
			weight *= 0.5;
			tempP *= 2;
		}
		return std::fabs(accumulator);
	}

private:
	static void perlinGeneratePerm(int* p) {
		for (int i = 0; i < pointCount; ++i) {
			p[i] = i;
		}
		permute(p, pointCount);
	}
	
	static void permute(int *p, int n) {
		for (int i = n - 1; i > 0; --i) {
			int target = randomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static double trilinearInterpolation(const Vec3 c[2][2][2], double u, double v, double w) {
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);
		auto accumulator = 0.0;

		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					Vec3 weightV(u - i, v - j, w - k);
					accumulator +=
						(i * uu + (1 - i) * (1 - u)) *
						(j * vv + (1 - j) * (1 - v)) *
						(k * ww + (1 - k) * (1 - w)) *
						 dot(c[i][j][k], weightV);
				}
			}
		}
		return accumulator;
	}
};