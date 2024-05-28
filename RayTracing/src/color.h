#pragma once

#include "interval.h"
#include "vec3.h"

using color = vec3;

void write_image(uint8_t* image, int nx, int ny) {
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = 0; j <= ny - 1; j++) {
        for (int i = 0; i < nx; ++i) {
            std::cout << static_cast<int>(image[3 * (j * nx + i) + 0]) << " ";
            std::cout << static_cast<int>(image[3 * (j * nx + i) + 1]) << " ";
            std::cout << static_cast<int>(image[3 * (j * nx + i) + 2]) << std::endl;
        }
    }
}

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

void write_color(uint8_t* image, int idx, color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    image[idx + 0] = rbyte;
    image[idx + 1] = gbyte;
    image[idx + 2] = bbyte;
}