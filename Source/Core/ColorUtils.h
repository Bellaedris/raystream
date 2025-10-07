//
// Created by Bellaedris on 07/10/2025.
//

#pragma once

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
    };
}