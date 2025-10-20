//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <glm/vec3.hpp>
#include "Image.h"
#include "../Shapes/Sphere.h"
#include "Hittable.h"
#include "Scene.h"

namespace ray::core
{
    class Camera
    {
    private:
        glm::vec3 m_position;
        int m_width, m_height;
        float m_viewportWidth;
        float m_viewportHeight;
        float m_aspectRatio;
        float m_focalLength;

        int m_raysMaxDepth{1};
        int m_spp{1};

        /**
         * \brief Just a hack to try and avoid inheritance pointer table by using templates
         * \tparam Primitive An type that defines an Intersect(const Ray& ray)
         * \param ray A ray
         * \param primitive A primitive to check intersection with
         * \return A struct filled with infos about the hit, or nothing
         */
        template<Hittable Primitive>
        std::optional<RayHit> Intersect(const Ray &ray, const Primitive &primitive, float tMin, float tMax)
        {
            return primitive.Intersect(ray, tMin, tMax);
        }

    public:
        /***
         * \brief Builds a camera with an inner image (that represents a sensor) and a viewport size
         * \param width width of the camera image
         * \param height height of the camera image
         * \param viewportHeight width of the viewport in world size
         * \param position position of the camera in world space
         * \param focalLength distance of the focal plane in the positive z axis
         */
        Camera(int width, int height, float viewportHeight, glm::vec3 position, float focalLength, int raysMaxDepth, int spp);

        /**
         * \brief Iterates over all pixels in the scene and sends rays to color the image
         * \tparam Primitive an object that can be tested for intersection with a ray
         * \param scene a collection of hittable primitives
         * \return a rendered frame
         */
        Image Render(const Scene& scene);

        /**
         * \brief Intersects a single ray with a scene
         * \tparam Primitive an object that can be tested for intersection with a ray
         * \param scene a collection of hittable primitives
         * \param ray a ray
         * \return the color of the emitted ray
         */
        Color RayColor(const Scene &scene, const Ray &ray, int currentDepth);
    };
}
