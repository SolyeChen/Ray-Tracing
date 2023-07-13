#pragma once
#include "rtweekend.h"

class camera {
public:
    camera(
        point3 lookfrom,        //位置点
        point3 lookAt,          //焦点
        vec3 vup,               //视口朝上方向
        double vfov,            //垂直 fov 角度
        double aspect_ratio,    //视口比例
        double aperture,        //光圈大小
        double focus_dist       //焦距
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        
        //视口定义
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        //+w在后，-w在前
        auto w = unit_vector(lookfrom - lookAt);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        //原点（摄像机）
        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        //视口左下角
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    vec3 u, v, w;
    double lens_radius;
};