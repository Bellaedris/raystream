//
// Created by Bellaedris on 05/10/2025.
//

#include "Camera.h"
#include "Ray.h"
#include "RandomUtils.h"
#include "Timer.h"
#include "../MonteCarlo/CosineWeighted.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

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
        Ray ln = ray;
        Color result(0, 0, 0);
        glm::vec3 throughput(1.f);
        for(int depth = 1; depth < currentDepth; depth++)
        {
            std::optional<RayHit> hit = scene.Intersect(ln, VectorUtils::M_EPSILON, std::numeric_limits<float>::max());
            if (hit.has_value())
            {
                result += throughput * hit->m_material->m_emission;

                Color direct {0, 0, 0};
                Color indirect {0, 0, 0};
                float pdfSum = .0f;
                // Evaluate direct lighting
                for (int i = 0; i < m_bouncesPerHit; ++i)
                {
                    Scene::SampledLightInfo lightSample = scene.GetSampleOnRandomLight();
                    glm::vec3 pointToLight = lightSample.sampledPoint - hit->m_point;
                    Ray directRay(hit->m_point, pointToLight);
                    auto directHit = scene.Intersect(
                            directRay,
                            VectorUtils::M_EPSILON,
                            glm::length(pointToLight) - VectorUtils::M_EPSILON
                    );
                    if (directHit.has_value() == false)
                    {
                        float LdotD = std::fmax(.0f, glm::dot(lightSample.sampledNormal, glm::normalize(-pointToLight)));
                        float d2 = glm::length2(pointToLight);
                        float pdfLightToSolidAngle = lightSample.pdf * LdotD / d2;
                        direct = throughput * (lightSample.mat->m_emission *
                                  (hit->m_material->m_albedo /glm::pi<float>()) *
                                  std::fmax(.0f, glm::dot(hit->m_normal, glm::normalize(pointToLight))) *
                                  LdotD /
                                  d2 /
                                  lightSample.pdf);
                        pdfSum += lightSample.pdf;
                    }
                }
                direct /= static_cast<float>(m_bouncesPerHit);

                // evaluate indirect lighting
                mc::CosineWeighted distribution(hit->m_normal);
                std::optional<ScatterInfos> scatter = hit->m_material->Scatter<mc::CosineWeighted>(ln, hit.value(), distribution, glm::normalize(m_position - hit->m_point));
                if(scatter.has_value())
                {
                    //send secondary ray
                    auto bounce = scene.Intersect(scatter->m_ray, VectorUtils::M_EPSILON, std::numeric_limits<float>::max());
                    if(bounce.has_value() && bounce->m_material->m_type == Material::MATERIAL_EMISSIVE)
                    {
                        pdfSum += scatter->m_pdf;
                        indirect = throughput * (bounce->m_material->m_emission * scatter->m_brdf / scatter->m_pdf);
                    }
                    throughput = throughput * scatter->m_brdf * glm::max(glm::dot(scatter->m_ray.m_direction, hit->m_normal), .0f) / scatter->m_pdf;
                    ln = scatter->m_ray;
                }
                else
                    break;

                result += (direct + direct) / 2.f; // move this to the scattering function?

                // russian roulette
                // TODO eventually add a minimum depth to begin russian roulette
                float q = std::clamp(std::max(std::max(throughput.x, throughput.y), throughput.z), .05f, 1.f);
                if(RNG::Float() > q)
                    break;
                else
                    throughput /= q;
            }
            else
            {
                result += throughput * ColorUtils::ColorLerp({0.5f, 0.7f, 1.0f}, {1, 1, 1}, ray.m_direction.y);
                break;
            }
        }

        return result;

//        if(currentDepth < 1)
//            return {0, 0, 0};
//
//        std::optional<RayHit> hit = scene.Intersect(ray, VectorUtils::M_EPSILON, std::numeric_limits<float>::max());
//        if (hit.has_value())
//        {
//            Color direct {0, 0, 0};
//            Color indirect {0, 0, 0};
//            // Evaluate direct lighting
//            for (int i = 0; i < m_bouncesPerHit; ++i)
//            {
//                Scene::SampledLightInfo lightSample = scene.GetSampleOnRandomLight();
//                glm::vec3 pointToLight = lightSample.sampledPoint - hit->m_point;
//                Ray directRay(hit->m_point, pointToLight);
//
//                auto directHit = scene.Intersect(
//                        directRay,
//                        VectorUtils::M_EPSILON,
//                        glm::length(pointToLight) - VectorUtils::M_EPSILON
//                );
//                if (directHit.has_value() == false)
//                {
//                    direct += lightSample.mat->m_emission * (hit->m_material->m_albedo /
//                              glm::pi<float>()) *
//                              std::fmax(.0f, glm::dot(hit->m_normal, glm::normalize(pointToLight))) *
//                              std::fmax(.0f, glm::dot(lightSample.sampledNormal, glm::normalize(-pointToLight))) /
//                              glm::length2(pointToLight) /
//                              lightSample.pdf;
//                }
//            }
//            direct /= static_cast<float>(m_bouncesPerHit);
//
//            // bounce rays through the scene
//            mc::CosineWeighted distribution(hit->m_normal);
//            std::optional<RayAttenuation> scatter = hit->m_material->Scatter<mc::CosineWeighted>(ray, hit.value(), distribution, glm::normalize(m_position - hit->m_point));
//            if(scatter.has_value())
//                indirect += scatter->m_attenuation * RayColor(scene, scatter->m_ray, currentDepth - 1);
//
//            return hit->m_material->m_emission + (direct + indirect) / 2.f; // move this to the scattering function?
//        }
//        else
//        {
//            // TODO skybox, since this one is a straight up copypaste
//            return ColorUtils::ColorLerp({0.5f, 0.7f, 1.0f}, {1, 1, 1}, ray.m_direction.y) ;
//        }
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
            #pragma omp parallel for schedule(dynamic, 8)
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

                    res = ColorUtils::ColorToGamma(ColorUtils::TonemapReinhart(res / static_cast<float>(m_spp)), 2.2f);
                    result.SetPixel(x, y, ColorUtils::ColorToRGB(res));
                }
                #pragma omp atomic
                finished++;
            }
        }

        return result;
    }
}