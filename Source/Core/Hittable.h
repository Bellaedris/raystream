//
// Created by Bellaedris on 06/10/2025.
//

#pragma once

#include <optional>
#include "Ray.h"

namespace ray::core
{
    template<typename T>
    concept Hittable = requires(const T& type , const Ray& ray)
    {
        { type.Intersect(ray) } -> std::convertible_to<std::optional<RayHit>>;
    };
}