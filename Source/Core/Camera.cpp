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

    ColorRGB Camera::RayColor(const Scene &scene, const Ray &ray)
    {
        std::optional<RayHit> hit;
        Color result(0, 0, 0);
        hit = scene.Intersect(ray, 0.001f, std::numeric_limits<float>::max());
        if (hit.has_value())
        {
            const std::vector<PointLight>& lights = scene.GetLights();
            for(const auto& light : lights)
            {
                // imagine there is a light above the sphere (hardcoded stuff, don't do that at home)
                glm::vec3 lightDir = glm::normalize(light.m_position - hit->m_point);
                float cosTheta = std::max(glm::dot(lightDir, hit->m_normal), .0f);
                result += hit->m_material.m_albedo * cosTheta * light.m_color;
            }
            result /= lights.size();
        }
        else
        {
            // TODO skybox
        }

        return ColorUtils::ColorToRGB(result);
    }

    Image Camera::Render(const Scene &scene)
    {
        Image result(m_width, m_height);

        float pixelSizeX = m_viewportWidth / m_width;
        float pixelSizeY = -m_viewportHeight / m_height;

        glm::vec3 upperLeftPixel = m_position - glm::vec3(m_viewportWidth / 2.f, -m_viewportHeight / 2.f, -m_focalLength);
        for(int y = 0; y < m_height; y++)
            for(int x = 0; x < m_width; x++)
            {
                glm::vec3 currentPixel = upperLeftPixel + glm::vec3(pixelSizeX * static_cast<float>(x), pixelSizeY * static_cast<float>(y), .0f);
                Ray r(m_position, (currentPixel - m_position));
                result.SetPixel(x, y, RayColor(scene, r));
            }

        return result;
    }
}