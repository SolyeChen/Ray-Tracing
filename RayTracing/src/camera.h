#pragma once
#include "rtweekend.h"

class camera {
public:
    camera(
        point3 lookfrom,        //λ�õ�
        point3 lookAt,          //����
        vec3 vup,               //�ӿڳ��Ϸ���
        double vfov,            //��ֱ fov �Ƕ�
        double aspect_ratio,    //�ӿڱ���
        double aperture,        //��Ȧ��С
        double focus_dist       //����
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        
        //�ӿڶ���
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        //+w�ں�-w��ǰ
        auto w = unit_vector(lookfrom - lookAt);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        //ԭ�㣨�������
        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        //�ӿ����½�
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