#include <iostream>
#include "Core/Camera.h"

int main()
{
    auto blue = std::make_shared<ray::core::Material>(ray::core::Material::CreateDiffuse(glm::vec3(.0, .0, 1.f)));
    auto red = std::make_shared<ray::core::Material>(ray::core::Material::CreateDiffuse(glm::vec3(1.f, .0, .0f)));
    auto green = std::make_shared<ray::core::Material>(ray::core::Material::CreateDiffuse(glm::vec3(.0, 1.f, .0f)));
    auto white = std::make_shared<ray::core::Material>(ray::core::Material::CreateDiffuse(glm::vec3(1.f, 1.f, 1.f)));
    auto pink = std::make_shared<ray::core::Material>(ray::core::Material::CreateDiffuse(glm::vec3(.961f, .663f, .722f)));
    auto lightBlue = std::make_shared<ray::core::Material>(ray::core::Material::CreateDiffuse(glm::vec3(.357f, .808f, .98f)));

    auto metal = std::make_shared<ray::core::Material>(ray::core::Material::CreateMetallic(glm::vec3(1.f, 1.f, 1.f), .5f));

    auto glass = std::make_shared<ray::core::Material>(ray::core::Material::CreateDielectric({1, 1, 1}, 1.5f));

    auto lamp = std::make_shared<ray::core::Material>(ray::core::Material::CreateEmissive({1.f, 1.f, 1.f}));

    ray::core::Scene scene;
    scene.AddSphere({glm::vec3(5.f, -7.f, 6.f), 3.f, metal});
    scene.AddSphere({glm::vec3(-4.f, -6.f, 10.f), 4.f, glass});
    scene.AddSphere({glm::vec3(0.f, 5.f, 0.f), 3.f, lamp});

    // cornell box
    scene.AddPlane({{.0f, .0f, 10.f}, {.0, .0f, -1.f}, white}); // back wall
    scene.AddPlane({{-10.f, .0f, .0f}, {1.f, .0f, .0f}, pink}); // left wall
    scene.AddPlane({{10.f, .0f, .0f}, {-1.f, .0f, .0f}, lightBlue}); // right wall
    scene.AddPlane({{.0f, 10.f, .0f}, {.0, -1.f, .0f}, white}); // ceiling
    scene.AddPlane({{.0f, -10.f, .0f}, {.0, 1.f, .0f}, white}); // floor

    // lights
    scene.AddLight({{1.f, .84f, .67f}, {.0f, 9.f, .0f}});
    scene.AddLight({{.2f, .0f, .0f}, {8.f, .0, .0f}});

    ray::core::Camera cam(800, 800, 2.f, {.0f, -2.f, -20.f}, 2.f, 50, 250);
    ray::core::Image result = cam.Render(scene);
    result.Write("render.png");

    return 0;
}
