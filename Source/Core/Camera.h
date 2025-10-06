//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <glm/vec3.hpp>
#include "Image.h"
#include "Sphere.h"
#include "Hittable.h"

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
        Camera(int width, int height, float viewportHeight, glm::vec3 position, float focalLength);

        /**
         * \brief Iterates over all pixels in the scene and sends rays to color the image
         * \tparam Primitive an object that can be tested for intersection with a ray
         * \param scene a collection of hittable primitives
         * \return a rendered frame
         */
        template<Hittable Primitive>
        Image Render(const std::vector<Primitive>& scene)
        {
            Image result(m_width, m_height);

            float pixelSizeX = m_viewportWidth / m_width;
            float pixelSizeY = m_viewportHeight / m_height;

            glm::vec3 upperLeftPixel = m_position - glm::vec3(m_viewportWidth / 2.f, m_viewportHeight / 2.f, -m_focalLength);
            for(int y = 0; y < m_height; y++)
                for(int x = 0; x < m_width; x++)
                {
                    glm::vec3 currentPixel = upperLeftPixel + glm::vec3(pixelSizeX * static_cast<float>(x), pixelSizeY * static_cast<float>(y), .0f);
                    Ray r(m_position, (currentPixel - m_position));
                    result.SetPixel(x, y, RayColor(scene, r));
                }

            return result;
        }

        /**
         * \brief Intersects a single ray with a scene
         * \tparam Primitive an object that can be tested for intersection with a ray
         * \param scene a collection of hittable primitives
         * \param ray a ray
         * \return the color of the emitted ray
         */
        template<Hittable Primitive>
        glm::vec<3, unsigned char> RayColor(const std::vector<Primitive> &scene, const Ray &ray)
        {
            std::optional<RayHit> hit;
            float closest = std::numeric_limits<float>::max();
            glm::vec<3, unsigned char> result(0u, 0u, 0u);
            for(const auto& primitives : scene)
            {
                hit = Intersect(ray, primitives, 0.001f, closest);
                if (hit.has_value())
                {
                    // imagine there is a light above the sphere (hardcoded stuff, don't do that at home)
                    glm::vec3 lightDir = glm::normalize(glm::vec3(.0f, 10.f, 0.f) - hit->m_point);
                    float cosTheta = std::max(glm::dot(-lightDir, hit->m_normal), .0f);
                    result = {static_cast<unsigned char>(255.f * cosTheta), 0u, 0u};
                    closest = hit->m_distance;
                }
            }

            return result;
        }
    };
}
