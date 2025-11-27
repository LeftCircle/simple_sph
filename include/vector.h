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


template<typename T>
struct Vec3T{
	T x, y, z;

	Vec3T(T x, T y, T z) : x(x), y(y), z(z) {};

	T magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	Vec3T operator+(const Vec3T& v) const {
		return Vec3T{ x + v.x, y + v.y, z + v.z };
	}

	Vec3T operator+=(const Vec3T& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3T operator-(const Vec3T& v) const {
		return Vec3T{ x - v.x, y - v.y, z - v.z };
	}

	Vec3T operator-=(const Vec3T& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vec3T operator-() const {
		return Vec3T{ -x, -y, -z };
	}

	Vec3T operator*(T s) const {
		return Vec3T{ x * s, y * s, z * s };
	}

	Vec3T operator/(T s) const {
		return Vec3T{ x / s, y / s, z / s };
	}

};


// Type alias for common vector types
using Vec3f = Vec3T<float>;
using Vec3d = Vec3T<double>;
using Vec3i = Vec3T<int>;
using Vec3s = Vec3T<size_t>;

}; // Namespace cato
	
#endif // CATON_VECTOR_H