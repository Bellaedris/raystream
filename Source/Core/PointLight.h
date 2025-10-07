//
// Created by Bellaedris on 07/10/2025.
//

#pragma once

#include <glm/glm.hpp>
#include "ColorUtils.h"

namespace ray::core
{
    struct PointLight
    {
        Color m_color;
        glm::vec3 m_position;

        PointLight(const Color& col, const glm::vec3& pos) : m_color(col), m_position(pos) {}
    };
}