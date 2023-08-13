#pragma once

#include "vec3.h"

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    //r *= scale;
    //g *= scale;
    //b *= scale;

    // Gamma Correction
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

void write_color(uint8_t* image, int idx, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    //r *= scale;
    //g *= scale;
    //b *= scale;

    // Gamma Correction
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    image[idx + 0] = 256 * clamp(r, 0.0, 0.999);
    image[idx + 1] = 256 * clamp(g, 0.0, 0.999);
    image[idx + 2] = 256 * clamp(b, 0.0, 0.999);
}
void write_image(uint8_t* image, int nx, int ny) {
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            std::cout << static_cast<int>(image[3 * (j * nx + i) + 0]) << " ";
            std::cout << static_cast<int>(image[3 * (j * nx + i) + 1]) << " ";
            std::cout << static_cast<int>(image[3 * (j * nx + i) + 2]) << std::endl;
        }
    }
}