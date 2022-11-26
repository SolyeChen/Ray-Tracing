#pragma once
#include "vector3.h"


class Ray {
public:
    Ray() {}
    Ray(const Point3& origin, const Vector3& direction)
        : orig(origin), dir(direction)
    {}

    Point3 origin() const { return orig; }
    Vector3 direction() const { return dir; }

    Point3 at(double t) const {
        return orig + t * dir;
    }

public:
    Point3 orig;
    Vector3 dir;
};