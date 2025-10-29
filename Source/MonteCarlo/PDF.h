//
// Created by bellaedris on 27/10/25.
//

#pragma once

#include <glm/glm.hpp>

namespace ray::mc
{
    template<typename T>
    concept PDF = requires(const T& type , const glm::vec3& dir)
    {
        { type.Value(dir) } -> std::convertible_to<float>;
        { type.Generate() } -> std::convertible_to<glm::vec3>;
    };
} // ray::mc