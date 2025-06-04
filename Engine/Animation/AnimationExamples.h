#pragma once

#include "Animation.h"
#include <iostream>
#include <vector>

namespace Animation
{
    /**
     * @brief Example usage and testing for the animation system
     */
    class AnimationExamples
    {
    public:
        /**
         * @brief Demonstrate basic easing functions
         */
        static void DemonstrateBasicEasing()
        {
            std::cout << "=== Basic Easing Examples ===" << std::endl;

            float start = 0.0f;
            float end = 100.0f;

            // Test different easing types
            std::vector<EasingType> easingTypes = {
                EasingType::Linear,
                EasingType::EaseInQuad,
                EasingType::EaseOutQuad,
                EasingType::EaseInOutQuad,
                EasingType::EaseInBounce,
                EasingType::EaseOutBounce,
                EasingType::EaseInElastic,
                EasingType::EaseOutElastic};

            for (auto easingType : easingTypes)
            {
                std::cout << "Easing Type: " << static_cast<int>(easingType) << std::endl;
                for (float t = 0.0f; t <= 1.0f; t += 0.25f)
                {
                    float result = AnimationUtil::Lerp(start, end, t, easingType);
                    std::cout << "  t=" << t << " -> " << result << std::endl;
                }
                std::cout << std::endl;
            }
        }

        /**
         * @brief Demonstrate interpolation with different data types
         */
        static void DemonstrateTypedInterpolation()
        {
            std::cout << "=== Typed Interpolation Examples ===" << std::endl;

            // Integer interpolation
            int intStart = 10, intEnd = 50;
            int intResult = AnimationUtil::Lerp(intStart, intEnd, 0.5f, EasingType::EaseInOutQuad);
            std::cout << "Int Lerp: " << intStart << " -> " << intEnd << " at 0.5 = " << intResult << std::endl;

            // Double interpolation
            double doubleStart = 1.5, doubleEnd = 3.7;
            double doubleResult = AnimationUtil::Lerp(doubleStart, doubleEnd, 0.7f, EasingType::EaseOutSine);
            std::cout << "Double Lerp: " << doubleStart << " -> " << doubleEnd << " at 0.7 = " << doubleResult << std::endl;

            std::cout << std::endl;
        }

        /**
         * @brief Demonstrate custom easing functions
         */
        static void DemonstrateCustomEasing()
        {
            std::cout << "=== Custom Easing Function Example ===" << std::endl;

            // Custom easing function - parabolic
            auto customEasing = [](float t) -> float
            {
                return t * t * (3.0f - 2.0f * t); // Smooth step function
            };

            float start = 0.0f;
            float end = 10.0f;

            for (float t = 0.0f; t <= 1.0f; t += 0.2f)
            {
                float result = AnimationUtil::Lerp(start, end, t, customEasing);
                std::cout << "Custom easing t=" << t << " -> " << result << std::endl;
            }

            std::cout << std::endl;
        }

        /**
         * @brief Demonstrate multi-point interpolation
         */
        static void DemonstrateMultiLerp()
        {
            std::cout << "=== Multi-Point Interpolation Example ===" << std::endl;

            float keyframes[] = {0.0f, 5.0f, 2.0f, 8.0f, 1.0f};
            int keyframeCount = 5;

            for (float t = 0.0f; t <= 1.0f; t += 0.1f)
            {
                float result = AnimationUtil::MultiLerp(keyframes, keyframeCount, t, EasingType::EaseInOutSine);
                std::cout << "MultiLerp t=" << t << " -> " << result << std::endl;
            }

            std::cout << std::endl;
        }

        /**
         * @brief Run all demonstration examples
         */
        static void RunAllExamples()
        {
            std::cout << "Animation System Examples" << std::endl;
            std::cout << "=========================" << std::endl
                      << std::endl;

            DemonstrateBasicEasing();
            DemonstrateTypedInterpolation();
            DemonstrateCustomEasing();
            DemonstrateMultiLerp();

            std::cout << "Examples completed!" << std::endl;
        }
    };
}
