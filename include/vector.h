#ifndef CATON_VECTOR_H
#define CATON_VECTOR_H

#include <cmath>

namespace cato{

template<typename T>
struct Vec2T{
	T x, y;

	Vec2T() : x(0), y(0) {}
	Vec2T(T x, T y) : x(x), y(y) {};

	T magnitude() const {
		return sqrt(x * x + y * y);
	}

	T magnitude_squared() const {
		return x * x + y * y;
	}

	Vec2T operator+(const Vec2T& v) const {
		return Vec2T{ x + v.x, y + v.y };
	}

	Vec2T operator+=(const Vec2T& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2T operator-(const Vec2T& v) const {
		return Vec2T{ x - v.x, y - v.y };
	}

	Vec2T operator-=(const Vec2T& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2T operator*(T s) const {
		return Vec2T{ x * s, y * s };
	}

	Vec2T operator/(T s) const {
		return Vec2T{ x / s, y / s };
	}

};

template<typename T>
struct Vec3T{
	T x, y, z;

	Vec3T(T x, T y, T z) : x(x), y(y), z(z) {};
	Vec3T() : x(0), y(0), z(0) {};

	T magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	T magnitude_squared() const {
		return x * x + y * y + z * z;
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


template<typename T>
inline Vec3T<T> operator*(T s, const Vec3T<T>& v) {
	return Vec3T<T>{ v.x * s, v.y * s, v.z * s };
}


template<typename T>
inline Vec2T<T> operator*(T s, const Vec2T<T>& v) {
	return Vec2T<T>{ v.x * s, v.y * s };
}

// Type alias for common vector types
using Vec3f = Vec3T<float>;
using Vec3d = Vec3T<double>;
using Vec3i = Vec3T<int>;
using Vec3s = Vec3T<size_t>;

using Vec2f = Vec2T<float>;
using Vec2d = Vec2T<double>;
using Vec2i = Vec2T<int>;
using Vec2s = Vec2T<size_t>;

}; // Namespace cato
	
#endif // CATON_VECTOR_H