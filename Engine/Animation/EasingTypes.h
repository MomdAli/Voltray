#pragma once

namespace Animation
{
    /**
     * @brief Enumeration of all available easing types
     */
    enum class EasingType
    {
        // Linear
        Linear,

        // Sine
        EaseInSine,
        EaseOutSine,
        EaseInOutSine,

        // Quad
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,

        // Cubic
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,

        // Quart
        EaseInQuart,
        EaseOutQuart,
        EaseInOutQuart,

        // Quint
        EaseInQuint,
        EaseOutQuint,
        EaseInOutQuint,

        // Expo
        EaseInExpo,
        EaseOutExpo,
        EaseInOutExpo,

        // Circ
        EaseInCirc,
        EaseOutCirc,
        EaseInOutCirc,

        // Back
        EaseInBack,
        EaseOutBack,
        EaseInOutBack,

        // Elastic
        EaseInElastic,
        EaseOutElastic,
        EaseInOutElastic,

        // Bounce
        EaseInBounce,
        EaseOutBounce,
        EaseInOutBounce
    };

    /**
     * @brief Easing function signature
     * @param t Time parameter (0.0 to 1.0)
     * @return Eased value
     */
    using EasingFunction = float (*)(float t);
}
