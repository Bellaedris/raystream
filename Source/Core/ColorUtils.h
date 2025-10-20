//
// Created by Bellaedris on 07/10/2025.
//

#pragma once
#include <glm/glm.hpp>

namespace ray::core
{
    using ColorRGB = glm::vec<3, unsigned char>;
    using Color = glm::vec3;

    class ColorUtils
    {
    public:
        /**
         * \brief Converts a floating point color to an RGB [0; 255] color
         * \param col A floating point color value
         * \return a representation of the color on a single byte per channel
         */
        static ColorRGB ColorToRGB(const Color& col)
        {
            return {
                static_cast<unsigned char>(col.x * 255.f)
                 , static_cast<unsigned char>(col.y * 255.f)
                 , static_cast<unsigned char>(col.z * 255.f)
            };
        }

        /**
         * \brief Convert a Color in floating point space to a color in Gamma space
         * \param col the color
         * \param gamma the factor. Defaults to 2.2 as it is a standard factor for gamma space in most devices
         * \return the color in gamma space
         */
        static Color ColorToGamma(const Color& col, float gamma)
        {
            return glm::pow(col, Color(1.f / gamma));
        }

        /**
         * \brief Linear interpolation between 2 floating point colors
         * \param a first color
         * \param b second color
         * \param t mixing factor
         * \return a linear interpolation between a and b
         */
        static Color ColorLerp(const Color& a, const Color& b, float t)
        {
            return a * t + b * (1.f - t);
        }
    };
}