//
// Created by Bellaedris on 20/10/2025.
//

#include <random>
#include <glm/glm.hpp>

namespace ray::core
{
    class RNG
    {
    public:
        /**
         * \brief Allow to manually set a seed for reproductibility
         * \param seed
         */
        static void Seed(uint32_t seed)
        {
            GetEngine().seed(seed);
        }

        static float Float(float min = .0f, float max = 1.f)
        {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(GetEngine());
        }

        static int Int(int min, int max)
        {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(GetEngine());
        }

        static glm::vec3 Vector3(float min = .0f, float max = 1.f)
        {
            std::uniform_real_distribution<float> dist(min, max);
            return {dist(GetEngine()), dist(GetEngine()), dist(GetEngine())};
        }

    private:
        static std::mt19937& GetEngine()
        {
            thread_local std::mt19937 engine(std::random_device{}());
            return engine;
        }
    };
}