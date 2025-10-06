#include <iostream>
#include "Core/Camera.h"

int main()
{
    std::vector<ray::core::Sphere> scene;
    scene.emplace_back(glm::vec3(0.f, 0.f, 0.f), 1.f);
    ray::core::Camera cam(800, 600, 2.f, {.0f, .0f, -5.f}, 1);
    ray::core::Image result = cam.Render(scene);
    result.Write("render.png");

    return 0;
}
