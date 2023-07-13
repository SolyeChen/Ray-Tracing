#pragma once
#include "rtweekend.h"

class camera {
public:
    camera(
        point3 lookFrom,
        point3 lookAt,
        vec3 vup,
        double vfov, //垂直 fov 角度
        double aspect_ratio
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        
        //视口定义
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        //+w在后，-w在前
        auto w = unit_vector(lookFrom - lookAt);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        //原点（摄像机）
        origin = lookFrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        //视口左下角
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    ray get_ray(double s, double t) const {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};