//
// Created by Bellaedris on 05/10/2025.
//

#include "Camera.h"
#include "Ray.h"

namespace ray::core
{
    Camera::Camera(int width, int height, float viewportHeight, glm::vec3 position, float focalLength)
        : m_width(width)
        , m_height(height)
        , m_viewportHeight(viewportHeight)
        , m_position(position)
        , m_focalLength(focalLength)
    {
        m_aspectRatio = m_width / static_cast<float>(m_height);
        m_viewportWidth = m_viewportHeight * m_aspectRatio;
    }

    Image Camera::Render(const std::vector<Sphere>& scene)
    {
       Image result(m_width, m_height);

        float pixelSizeX = m_viewportWidth / m_width;
        float pixelSizeY = m_viewportHeight / m_height;

        glm::vec3 upperLeftPixel = m_position - glm::vec3(m_viewportWidth / 2.f, m_viewportHeight / 2.f, -m_focalLength);
        for(int y = 0; y < m_height; y++)
            for(int x = 0; x < m_width; x++)
            {
                glm::vec3 currentPixel = upperLeftPixel + glm::vec3(pixelSizeX * x, pixelSizeY * y, .0f);
                Ray r(m_position, (currentPixel - m_position));
                for(const auto& sphere : scene)
                {
                    std::optional<RayHit> hit = Intersect(r, sphere);
                    if (hit.has_value())
                    {
                        // imagine there is a light above the sphere (hardcoded stuff, don't do that at home)
                        glm::vec3 lightDir = glm::normalize(glm::vec3(.0f, 10.f, 0.f) - hit->m_point);
                        float cosTheta = std::max(glm::dot(-lightDir, hit->m_normal), .0f);
                        result.SetPixel(x, y, {static_cast<unsigned char>(255.f * cosTheta), 0u, 0u});
                    }
                }
                //result.SetPixel(x, y, {x / (float)m_width * 255u, y / (float)m_height * 255u, 0.f});
            }

        return result;
    }

    template<Hittable Primitive>
    std::optional<RayHit> Camera::Intersect(const Ray &ray, const Primitive &primitive)
    {
        return primitive.Intersect(ray);
    }
}