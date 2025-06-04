#pragma once

#include "EasingTypes.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.141592653589793238467932384626433f
#endif

namespace Animation
{
    /**
     * @brief Collection of easing functions implementing various animation curves
     */
    class EasingFunctions
    {
    public:
        // Linear
        static float Linear(float t);

        // Sine
        static float EaseInSine(float t);
        static float EaseOutSine(float t);
        static float EaseInOutSine(float t);

        // Quad
        static float EaseInQuad(float t);
        static float EaseOutQuad(float t);
        static float EaseInOutQuad(float t);

        // Cubic
        static float EaseInCubic(float t);
        static float EaseOutCubic(float t);
        static float EaseInOutCubic(float t);

        // Quart
        static float EaseInQuart(float t);
        static float EaseOutQuart(float t);
        static float EaseInOutQuart(float t);

        // Quint
        static float EaseInQuint(float t);
        static float EaseOutQuint(float t);
        static float EaseInOutQuint(float t);

        // Expo
        static float EaseInExpo(float t);
        static float EaseOutExpo(float t);
        static float EaseInOutExpo(float t);

        // Circ
        static float EaseInCirc(float t);
        static float EaseOutCirc(float t);
        static float EaseInOutCirc(float t);

        // Back
        static float EaseInBack(float t);
        static float EaseOutBack(float t);
        static float EaseInOutBack(float t);

        // Elastic
        static float EaseInElastic(float t);
        static float EaseOutElastic(float t);
        static float EaseInOutElastic(float t);

        // Bounce
        static float EaseInBounce(float t);
        static float EaseOutBounce(float t);
        static float EaseInOutBounce(float t);

        /**
         * @brief Get easing function by type
         * @param type The easing type
         * @return Function pointer to the easing function
         */
        static EasingFunction GetEasingFunction(EasingType type);
    };
}
