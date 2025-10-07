//
// Created by Bellaedris on 07/10/2025.
//

#pragma once
#include "ColorUtils.h"

namespace ray::core
{
    struct Material
    {
        Color m_albedo;
        explicit Material(const ColorRGB& col) : m_albedo(col) {}
    };
}
