#ifndef CATON_VECTOR_H
#define CATON_VECTOR_H

#include <cmath>

namespace cato{

struct Vec2{
	float x, y;


	float magnitude() const {
		return sqrt(x * x + y * y);
	}

	Vec2 operator+(const Vec2& v) const {
		return Vec2{ x + v.x, y + v.y };
	}

	Vec2 operator*(float s) const {
		return Vec2{ x * s, y * s };
	}

	Vec2 operator/(float s) const {
		return Vec2{ x / s, y / s };
	}

	Vec2 operator+=(const Vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
};
};
	
#endif // CATON_VECTOR_H