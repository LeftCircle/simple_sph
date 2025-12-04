#ifndef CATON_VECTOR_H
#define CATON_VECTOR_H

#include <cmath>

namespace cato{

template<typename T>
struct Vec2T{
	using value_type = T;
	T x, y;

	Vec2T() : x(0), y(0) {}
	Vec2T(T x, T y) : x(x), y(y) {};
	Vec2T(T v) : x(v), y(v) {};
	T magnitude() const {
		return sqrt(x * x + y * y);
	}

	T magnitude_squared() const {
		return x * x + y * y;
	}

	T distance_to(const Vec2T& v) const {
		return (*this - v).magnitude();
	}

	Vec2T normalized() const {
		T mag = magnitude();
		if (mag < 0.0001){
			return Vec2T{ static_cast<T>(0), static_cast<T>(0) };
		}
		return Vec2T{ x / mag, y / mag };
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

	template<typename U>
	Vec2T operator*(U s) const {
		return Vec2T{ static_cast<T>(x * s), static_cast<T>(y * s) };
	}

	Vec2T operator*(const Vec2T& v) const {
		return Vec2T{ x * v.x, y * v.y };
	}

	template<typename U>
	Vec2T& operator*=(U s) {
		x = static_cast<T>(x * s);
		y = static_cast<T>(y * s);
		return *this;
	}

	template<typename U>
	Vec2T operator/(U s) const {
		return Vec2T{ static_cast<T>(x / s), static_cast<T>(y / s) };
	}

	template<typename U>
	Vec2T& operator/=(U s) {
		x = static_cast<T>(x / s);
		y = static_cast<T>(y / s);
		return *this;
	}
};

template<typename T>
struct Vec3T{
	using value_type = T;
	T x, y, z;

	Vec3T(T x, T y, T z) : x(x), y(y), z(z) {};
	Vec3T() : x(0), y(0), z(0) {};
	Vec3T(T v) : x(v), y(v), z(v) {};

	T magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	T magnitude_squared() const {
		return x * x + y * y + z * z;
	}

	T distance_to(const Vec3T& v) const {
		return (*this - v).magnitude();
	}

	Vec3T normalized() const {
		T mag = magnitude();
		if (mag < 0.0001){
			return Vec3T{ 0, 0, 0 };
		}
		return Vec3T{ x / mag, y / mag, z / mag };
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

	
	template<typename U>
	Vec3T operator*(U s) const {
		return Vec3T{ static_cast<T>(x * s), static_cast<T>(y * s), static_cast<T>(z * s) };
	}

	Vec3T operator*(const Vec3T& v) const {
		return Vec3T{ x * v.x, y * v.y, z * v.z };
	}

	template<typename U>
	Vec3T& operator*=(U s) {
		x = static_cast<T>(x * s);
		y = static_cast<T>(y * s);
		z = static_cast<T>(z * s);
		return *this;
	}

	template<typename U>
	Vec3T operator/(U s) const {
		return Vec3T{ static_cast<T>(x / s), static_cast<T>(y / s), static_cast<T>(z / s) };
	}

	template<typename U>
	Vec3T& operator/=(U s) {
		x = static_cast<T>(x / s);
		y = static_cast<T>(y / s);
		z = static_cast<T>(z / s);
		return *this;
	}

};


template<typename T, typename U>
inline Vec3T<T> operator*(U s, const Vec3T<T>& v) {
	return Vec3T<T>{ static_cast<T>(v.x * s), static_cast<T>(v.y * s), static_cast<T>(v.z * s) };
}


template<typename T, typename U>
inline Vec2T<T> operator*(U s, const Vec2T<T>& v) {
	return Vec2T<T>{ static_cast<T>(v.x * s), static_cast<T>(v.y * s) };
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