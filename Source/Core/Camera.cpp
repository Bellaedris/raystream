//
// Created by Bellaedris on 05/10/2025.
//

#include "Camera.h"
#include "Ray.h"
#include "RandomUtils.h"
#include "Timer.h"
#include "../MonteCarlo/CosineWeighted.h"

namespace ray::core
{
    Camera::Camera(int width, int height, float viewportHeight, glm::vec3 position, float focalLength, int raysMaxDepth, int spp, int bouncesPerHit)
        : m_width(width)
        , m_height(height)
        , m_viewportHeight(viewportHeight)
        , m_position(position)
        , m_focalLength(focalLength)
        , m_raysMaxDepth(std::max(1, raysMaxDepth))
        , m_spp(std::max(1, spp))
        , m_bouncesPerHit(bouncesPerHit)
    {
        m_aspectRatio = m_width / static_cast<float>(m_height);
        m_viewportWidth = m_viewportHeight * m_aspectRatio;
    }

    Color Camera::RayColor(const Scene &scene, const Ray &ray, int currentDepth)
    {
        if(currentDepth < 1)
            return {0, 0, 0};

        std::optional<RayHit> hit = scene.Intersect(ray, 0.001f, std::numeric_limits<float>::max());
        if (hit.has_value())
        {
            // bounce rays through the scene
            Color res {0, 0, 0};
            for (int i = 0; i < m_bouncesPerHit; ++i)
            {
                mc::CosineWeighted distribution(hit->m_normal);
                std::optional<RayAttenuation> scatter = hit->m_material->Scatter<mc::CosineWeighted>(ray, hit.value(), distribution);
                if(scatter.has_value())
                    res += hit->m_material->m_emission + scatter->m_attenuation * RayColor(scene, scatter.value().m_ray, currentDepth - 1) *
                        distribution.Value(scatter->m_ray.m_direction);
                else
                    res +=  hit->m_material->m_emission;
            }
            return res / static_cast<float>(m_bouncesPerHit);
        }
        else
        {
            // TODO skybox, since this one is a straight up copypaste
            return ColorUtils::ColorLerp({0.5f, 0.7f, 1.0f}, {1, 1, 1}, ray.m_direction.y) ;
        }
    }

    Image Camera::Render(const Scene &scene)
    {
        Image result(m_width, m_height);

        float pixelSizeX = m_viewportWidth / m_width;
        float pixelSizeY = m_viewportHeight / m_height;

        glm::vec3 upperLeftPixel = m_position - glm::vec3(m_viewportWidth / 2.f, -m_viewportHeight / 2.f, -m_focalLength);

        {
            ScopedTimer<std::chrono::milliseconds> timer("Tracing");
            int finished = 0;
            #pragma omp parallel for
            for (int y = 0; y < m_height; y++)
            {
                std::cout << "Lines finished: " << finished << " / " << m_height << "\n";
                for (int x = 0; x < m_width; x++)
                {
                    glm::vec3 currentPixel = upperLeftPixel + glm::vec3(
                            pixelSizeX * static_cast<float>(x),
                            -pixelSizeY * static_cast<float>(y),
                            .0f
                    );
                    Color res(0, 0, 0);
                    for (int i = 0; i < m_spp; i++)
                    {
                        glm::vec3 offset(RNG::Float(.0f, pixelSizeX), -RNG::Float(.0f, pixelSizeY), 0);
                        Ray r(m_position + offset, (currentPixel - m_position));
                        res += RayColor(scene, r, m_raysMaxDepth);
                    }

                    res = ColorUtils::ColorToGamma(res / static_cast<float>(m_spp), 2.2f),
                            result.SetPixel(x, y, ColorUtils::ColorToRGB(res));
                }
                finished++;
            }
        }

        return result;
    }
}