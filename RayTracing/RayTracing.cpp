// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "vector3.h"
#include "color.h"
#include "ray.h"
auto sphereCenter = Point3(0, 0, -1);

double hit_sphere(const Point3& center, double radius, const Ray& r) {
    Vector3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    //auto b = 2.0 * dot(oc, r.direction());
    auto harf_b = dot(oc, r.direction());

    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = harf_b * harf_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    }
    else {
        //t小的 距离更近
        return (-harf_b - sqrt(discriminant)) / a;
    }
}

Color ray_color(const Ray& ray) {
    auto t = hit_sphere(sphereCenter, 0.5, ray);
    if (t > 0.0) {
        Vector3 normal = unitVector3(ray.at(t) - sphereCenter);
        return (normal += 1.0) * 0.5;
    }
    Vector3 unit_direction = unitVector3(ray.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void writePPMImage() {
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //视口平面距离原点（摄像机）的距离
    auto focal_length = 1.0;
    //原点（摄像机）
    auto origin = Point3(0, 0, 0);
    //视口定义
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto horizontal = Vector3(viewport_width, 0, 0);
    auto vertical = Vector3(0, viewport_height, 0);
    //视口左下角
    auto lower_left_corner_point= origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);

    //render
    Color color = Color();
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            Ray r(origin, lower_left_corner_point + u * horizontal + v * vertical - origin);
            copyVec3(color, ray_color(r));
            WriteColor(std::cout, color);
        }
    }
    std::cerr << "\nDone.\n";
}
int main()
{
    //
    writePPMImage();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
