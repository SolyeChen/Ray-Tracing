#pragma once

#include <iostream>
#include "rtweekend.h"

class vec3 {
public:
	vec3() : m_arr{ 0, 0, 0 } {}
	vec3(double vx, double vy, double vz) :m_arr{ vx, vy, vz } {}

	double x() const { return m_arr[0]; }
	double y() const { return m_arr[1]; }
	double z() const { return m_arr[2]; }

	vec3 operator-() const { return vec3(-m_arr[0], -m_arr[1], -m_arr[2]); }
	double operator[](int i) const{ return m_arr[i]; }
	double& operator[](int i) { return m_arr[i]; }

	vec3& operator+=(const double t) { 
		m_arr[0] += t;
		m_arr[1] += t;
		m_arr[2] += t;
		return *this;
	}

	vec3& operator+=(const vec3 &v) {
		m_arr[0] += v.m_arr[0];
		m_arr[1] += v.m_arr[1];
		m_arr[2] += v.m_arr[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		m_arr[0] *= t;
		m_arr[1] *= t;
		m_arr[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1/t;
	}


	double length() {
		return std::sqrt(length_squared());
	}

	double length_squared() {
		return m_arr[0] * m_arr[0] + m_arr[1] * m_arr[1] + m_arr[2] * m_arr[2];
	}

	//Utils
	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}


public:
	double m_arr[3];
};


//Type aliases for vec3
using point3 = vec3; //3D Position
using color = vec3;	//RGB color


inline std::ostream& operator << (std::ostream &out, const vec3& v3) {
	return out << v3.x() << v3.y() << v3.z();
}

inline vec3 operator + (const vec3& v1, const vec3& v2) {
	return vec3(v1.m_arr[0] + v2.m_arr[0], v1.m_arr[1] + v2.m_arr[1], v1.m_arr[2] + v2.m_arr[2]);
}

inline vec3 operator - (const vec3& v1, const vec3& v2) {
	return vec3(v1.m_arr[0] - v2.m_arr[0], v1.m_arr[1] - v2.m_arr[1], v1.m_arr[2] - v2.m_arr[2]);
}

inline vec3 operator * (const vec3& v1, const vec3& v2) {
	return vec3(v1.m_arr[0] * v2.m_arr[0], v1.m_arr[1] * v2.m_arr[1], v1.m_arr[2] * v2.m_arr[2]);
}

inline vec3 operator * (const double t, const vec3& v) {
	return vec3(v.m_arr[0] * t, v.m_arr[1] * t, v.m_arr[2] * t);
}

inline vec3 operator * (vec3& v, const double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3& v1, const vec3& v2) {
	return v1.m_arr[0] * v2.m_arr[0]+ v1.m_arr[1] * v2.m_arr[1]+ v1.m_arr[2] * v2.m_arr[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(v1.m_arr[1] * v2.m_arr[2] - v1.m_arr[2] * v2.m_arr[1],
		v1.m_arr[2] * v2.m_arr[0] - v1.m_arr[0] * v2.m_arr[2],
		v1.m_arr[0] * v2.m_arr[1] - v1.m_arr[1] * v2.m_arr[0]);
}

inline vec3 unit_vec3(vec3 v) {
	return v / v.length();
}
inline vec3 normalize(vec3 v){
	return v / v.length();
}

inline vec3 copyVec3(vec3& v1, const vec3& v2) {
	v1.m_arr[0] = v2.m_arr[0];
	v1.m_arr[1] = v2.m_arr[1];
	v1.m_arr[2] = v2.m_arr[2];
	return v1;
}

inline vec3 setVec3(vec3& v, double x, double y, double z) {
	v.m_arr[0] = x;
	v.m_arr[1] = y;
	v.m_arr[2] = z;
	return v;
}

inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

