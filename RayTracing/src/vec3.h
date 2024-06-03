#pragma once

class vec3 {
public:
	double e[3];

	vec3() : e{ 0, 0, 0 } {}
	vec3(double vx, double vy, double vz) :e{ vx, vy, vz } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const{ return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3& operator+=(const double t) { 
		e[0] += t;
		e[1] += t;
		e[2] += t;
		return *this;
	}

	vec3& operator+=(const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1/t;
	}


	double length() {
		return sqrt(length_squared());
	}

	double length_squared() {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	//Utils
	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};


//Type aliases for vec3
using point3 = vec3; //3D Position


inline std::ostream& operator << (std::ostream &out, const vec3& v3) {
	return out << v3.x() << v3.y() << v3.z();
}

inline vec3 operator + (const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator - (const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator * (const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator * (const double t, const vec3& v) {
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator * (const vec3& v, const double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3& v1, const vec3& v2) {
	return v1.e[0] * v2.e[0]+ v1.e[1] * v2.e[1]+ v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}
inline vec3 normalize(vec3 v){
	return v / v.length();
}

inline vec3 copyVec3(vec3& v1, const vec3& v2) {
	v1.e[0] = v2.e[0];
	v1.e[1] = v2.e[1];
	v1.e[2] = v2.e[2];
	return v1;
}

inline vec3 setVec3(vec3& v, double x, double y, double z) {
	v.e[0] = x;
	v.e[1] = y;
	v.e[2] = z;
	return v;
}

inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}



inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}