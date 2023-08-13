// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <iomanip>
std::mutex progressMutex;


void processTaskComplete(int& ct, int totalTasks) {
    // 模拟任务处理
    // ...

    // 完成一个任务，更新进度
    std::lock_guard<std::mutex> lock(progressMutex);
    ct++;
}

bool progressStop = false;
void updateProgressThread(clock_t start, int& completeTasks, int totalTasks) {
    while (!progressStop)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int ctn = completeTasks;
        if (ctn == 0)
        {
            ctn = 1;
        }

        if (ctn == totalTasks)
        {
            progressStop = true;
            break;
        }
        clock_t ct = clock();
        float progress = (float)(ctn) / totalTasks * 100;

        int dt = ((double)ct - (double)start) / 1000;
        std::cerr << "\r已花费时间: " << dt << "s; ";
        std::cerr << "预计总时间: " << dt * totalTasks / ctn << "s; " ;
        std::cerr << "处理进度: " << progress << "%"  << std::flush;
    }
}

auto sphereCenter = point3(0, 0, -1);

color ray_color(const ray& r, const hittable& world, int depth) {
    // 如果已经超过了定义的射线反弹次数，就不会再聚集更多的光线。
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    //background
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    return world;
}

int main(int argc, char* argv[])
{
    clock_t begin, end;
    begin = clock();

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1080;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World

    //45
    auto R = cos(pi / 4);
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));

    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


    // Camrea

    point3 cameraStandPos(13, 2, 3);
    point3 cameraFocusPos(0, 0, 0);
    vec3 worldUp(0, 1, 0);
    auto vfov = 20;

    auto aperture = 0;
    auto dist_to_focus = 10;
    camera cam(cameraStandPos, cameraFocusPos, worldUp, vfov, aspect_ratio, aperture, dist_to_focus);

    hittable_list randomWorld = random_scene();
    std::vector<uint8_t> image(image_width * image_height * 3);

    int completeTasks = 0;
    int totalTasks = image_height * image_width;

    std::thread progressThread(updateProgressThread, begin, std::ref(completeTasks), totalTasks);

    const int k_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(k_threads);
    for (int threadId = 0; threadId < k_threads; ++threadId) {
        threads[threadId] = std::thread(std::bind([&](int start, int end, int t) {
            for (int j = start; j < end; j++) {
                for (int i = 0; i < image_width; i++) {
                    color col(0, 0, 0);
                    for (int s = 0; s < samples_per_pixel; s++) {
                        float u = float(i + random_double()) / float(image_width);
                        float v = float(j + random_double()) / float(image_height);
                        ray r = cam.get_ray(u, v);
                        col += ray_color(r, randomWorld, max_depth);
                    }
                    write_color(&image[0], 3 * (j * image_width + i), col, samples_per_pixel);
                    processTaskComplete(std::ref(completeTasks), totalTasks);
                }
            }
            }, threadId* image_height / k_threads, (threadId + 1) == k_threads ? image_height : (threadId + 1) * image_height / k_threads, threadId));
    }

    progressThread.join();
    for (int t = 0; t < k_threads; ++t) {
        threads[t].join();
    }

    write_image(&image[0], image_width, image_height);

    end = clock();
    std::cerr << "\nDone. Cost(s): " << ((double)end - (double)begin) / 1000 << "\n";

    return 0;
}
