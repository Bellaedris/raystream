//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <glm/glm.hpp>

namespace ray::core
{
    struct Ray
    {
        glm::vec3 m_origin;
        glm::vec3 m_direction;

        /**
         * \brief Creates a ray starting at origin and moving in direction
         * \param origin a point in space from which the ray starts
         * \param direction a direction for the ray to move. This will be normalized be the constructor, do not bother normalizing yourself.
         */
        Ray(glm::vec3 origin, glm::vec3 direction) : m_origin(origin), m_direction(glm::normalize(direction)) {};
    };

    struct RayHit
    {
        float m_distance;
        glm::vec3 m_point;
        glm::vec3 m_normal;
        bool m_IsFrontFace;
    };

} // ray::core