#include <iostream>
#include "Core/Camera.h"

int main()
{
    ray::core::Material blue(glm::vec3(.0, .0, 1.f));
    ray::core::Material red(glm::vec3(1.f, .0, .0f));
    ray::core::Material green(glm::vec3(.0, 1.f, .0f));
    ray::core::Material white(glm::vec3(1.f, 1.f, 1.f));


    ray::core::Scene scene;
    scene.AddSphere({glm::vec3(5.f, -9.f, 6.f), 1.f, blue});
    scene.AddSphere({glm::vec3(-4.f, -6.f, 10.f), 4.f, red});

    // cornell box
    scene.AddPlane({{.0f, .0f, 10.f}, {.0, .0f, -1.f}, white}); // back wall
    scene.AddPlane({{-10.f, .0f, .0f}, {1.f, .0f, .0f}, red}); // left wall
    scene.AddPlane({{10.f, .0f, .0f}, {-1.f, .0f, .0f}, green}); // right wall
    scene.AddPlane({{.0f, 10.f, .0f}, {.0, -1.f, .0f}, white}); // ceiling
    scene.AddPlane({{.0f, -10.f, .0f}, {.0, 1.f, .0f}, white}); // floor

    // lights
    scene.AddLight({{1.f, .84f, .67f}, {.0f, 9.f, .0f}});
    scene.AddLight({{.2f, .0f, .0f}, {8.f, .0, .0f}});

    ray::core::Camera cam(800, 800, 2.f, {.0f, .0f, -20.f}, 2.f);
    ray::core::Image result = cam.Render(scene);
    result.Write("render.png");

    return 0;
}
