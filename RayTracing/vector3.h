#pragma once
#include <iostream>

class Vector3 {
public:
	Vector3() : m_arr{ 0, 0, 0 } {}
	Vector3(double vx, double vy, double vz) :m_arr{ vx, vy, vz } {}

	double x() const { return m_arr[0]; }
	double y() const { return m_arr[1]; }
	double z() const { return m_arr[2]; }

	Vector3 operator-() const { return Vector3(-m_arr[0], -m_arr[1], -m_arr[2]); }
	double operator[](int i) const{ return m_arr[i]; }
	double& operator[](int i) { return m_arr[i]; }

	Vector3& operator+=(const double t) { 
		m_arr[0] += t;
		m_arr[1] += t;
		m_arr[2] += t;
		return *this;
	}

	Vector3& operator+=(const Vector3 &v) {
		m_arr[0] += v.m_arr[0];
		m_arr[1] += v.m_arr[1];
		m_arr[2] += v.m_arr[2];
		return *this;
	}

	Vector3& operator*=(const double t) {
		m_arr[0] *= t;
		m_arr[1] *= t;
		m_arr[2] *= t;
		return *this;
	}

	Vector3& operator/=(const double t) {
		return *this *= 1/t;
	}


	double length() {
		return std::sqrt(length_squared());
	}

	double length_squared() {
		return m_arr[0] * m_arr[0] + m_arr[1] * m_arr[1] + m_arr[2] * m_arr[2];
	}
public:
	double m_arr[3];
};

inline std::ostream& operator << (std::ostream & out, const Vector3& v3) {
	return out << v3.x() << v3.y() << v3.z();
}

inline Vector3 operator + (const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.m_arr[0] + v2.m_arr[0], v1.m_arr[1] + v2.m_arr[1], v1.m_arr[2] + v2.m_arr[2]);
}

inline Vector3 operator - (const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.m_arr[0] - v2.m_arr[0], v1.m_arr[1] - v2.m_arr[1], v1.m_arr[2] - v2.m_arr[2]);
}

inline Vector3 operator * (const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.m_arr[0] * v2.m_arr[0], v1.m_arr[1] * v2.m_arr[1], v1.m_arr[2] * v2.m_arr[2]);
}

inline Vector3 operator * (const double t, const Vector3& v) {
	return Vector3(v.m_arr[0] * t, v.m_arr[1] * t, v.m_arr[2] * t);
}

inline Vector3 operator * (Vector3& v, const double t) {
	return v *= t;
}

inline Vector3 operator/(Vector3 v, double t) {
	return (1 / t) * v;
}

inline double dot(const Vector3& v1, const Vector3& v2) {
	return v1.m_arr[0] * v2.m_arr[0]+ v1.m_arr[1] * v2.m_arr[1]+ v1.m_arr[2] * v2.m_arr[2];

}

inline Vector3 cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.m_arr[1] * v2.m_arr[2] - v1.m_arr[2] * v2.m_arr[1],
		v1.m_arr[2] * v2.m_arr[0] - v1.m_arr[0] * v2.m_arr[2],
		v1.m_arr[0] * v2.m_arr[1] - v1.m_arr[1] * v2.m_arr[0]);
}

inline Vector3 unitVector3(Vector3 v) {
	return v / v.length();
}
inline Vector3 normalize(Vector3 v){
	return v / v.length();
}

inline Vector3 copyVec3(Vector3& v1, const Vector3& v2) {
	v1.m_arr[0] = v2.m_arr[0];
	v1.m_arr[1] = v2.m_arr[1];
	v1.m_arr[2] = v2.m_arr[2];
	return v1;
}

inline Vector3 setVec3(Vector3& v, double x, double y, double z) {
	v.m_arr[0] = x;
	v.m_arr[1] = y;
	v.m_arr[2] = z;
	return v;
}
//Type aliases for vector3
using Point3 = Vector3; //3D Position
using Color = Vector3;	//RGB Color
