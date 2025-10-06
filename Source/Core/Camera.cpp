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
}