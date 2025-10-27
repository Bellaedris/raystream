//
// Created by bellaedris on 27/10/25.
//

#pragma once
#include <glm/glm.hpp>

namespace ray::core
{
    class VectorUtils
    {
    public:
        /**
         * Builds a new orthonormal basis using Frisvad's method https://backend.orbit.dtu.dk/ws/portalfiles/portal/126824972/onb_frisvad_jgt2012_v2.pdf
         * @param normal new up vector of the basis
         * @return a matrix to transform other vec3 to the new basis
         */
        static glm::mat3 FrisvadBasis(const glm::vec3& normal)
        {
            glm::vec3 b1, b2;
            if (normal.z < -0.9999999f) // Handle the singularity
            {
                b1 = {.0f, -1.f, .0f};
                b2 = {-1.f, .0f, .0f};
            }
            else
            {
                const float a = 1.f / (1.f + normal.z);
                const float b = -normal.x * normal.y * a ;
                b1 = {1.f - normal.x * normal.x * a , b , -normal.x};
                b2 = {b , 1.f - normal.y * normal.y * a, -normal.y};
            }

            return glm::mat3(b1, normal, b2);
        }
    };
}
