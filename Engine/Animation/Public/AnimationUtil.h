#pragma once

#include "EasingTypes.h"
#include "EasingFunctions.h"

namespace Voltray::Engine
{
    /**
     * @brief High-level animation utility class providing templated interpolation functions
     */
    class AnimationUtil
    {
    public:
        /**
         * @brief Linear interpolation with easing
         * @tparam T Type to interpolate
         * @param start Starting value
         * @param end Ending value
         * @param t Time parameter (0.0 to 1.0)
         * @param easingType Type of easing to apply
         * @return Interpolated value with easing applied
         */
        template <typename T>
        static T Lerp(const T &start, const T &end, float t, EasingType easingType = EasingType::Linear)
        {
            float easedT = EasingFunctions::GetEasingFunction(easingType)(t);
            return start + (end - start) * easedT;
        }

        /**
         * @brief Linear interpolation with custom easing function
         * @tparam T Type to interpolate
         * @param start Starting value
         * @param end Ending value
         * @param t Time parameter (0.0 to 1.0)
         * @param easingFunction Custom easing function
         * @return Interpolated value with easing applied
         */
        template <typename T>
        static T Lerp(const T &start, const T &end, float t, EasingFunction easingFunction)
        {
            float easedT = easingFunction(t);
            return start + (end - start) * easedT;
        }

        /**
         * @brief Smooth step interpolation with easing
         * @tparam T Type to interpolate
         * @param start Starting value
         * @param end Ending value
         * @param t Time parameter (0.0 to 1.0)
         * @param easingType Type of easing to apply
         * @return Smoothly interpolated value
         */
        template <typename T>
        static T SmoothStep(const T &start, const T &end, float t, EasingType easingType = EasingType::EaseInOutQuad)
        {
            // Clamp t to [0, 1]
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            float easedT = EasingFunctions::GetEasingFunction(easingType)(t);
            return start + (end - start) * easedT;
        }

        /**
         * @brief Interpolate between multiple values with easing
         * @tparam T Type to interpolate
         * @param values Array of values to interpolate between
         * @param count Number of values in the array
         * @param t Time parameter (0.0 to 1.0)
         * @param easingType Type of easing to apply
         * @return Interpolated value
         */
        template <typename T>
        static T MultiLerp(const T *values, int count, float t, EasingType easingType = EasingType::Linear)
        {
            if (count < 2)
                return count > 0 ? values[0] : T{};

            float easedT = EasingFunctions::GetEasingFunction(easingType)(t);

            // Scale t to the range of segments
            float scaledT = easedT * (count - 1);
            int index = static_cast<int>(scaledT);
            float localT = scaledT - index;

            // Clamp index
            index = index < 0 ? 0 : (index >= count - 1 ? count - 2 : index);

            return values[index] + (values[index + 1] - values[index]) * localT;
        }

        /**
         * @brief Apply easing to a normalized time value
         * @param t Time parameter (0.0 to 1.0)
         * @param easingType Type of easing to apply
         * @return Eased time value
         */
        static float ApplyEasing(float t, EasingType easingType)
        {
            return EasingFunctions::GetEasingFunction(easingType)(t);
        }

        /**
         * @brief Apply custom easing function to a normalized time value
         * @param t Time parameter (0.0 to 1.0)
         * @param easingFunction Custom easing function
         * @return Eased time value
         */
        static float ApplyEasing(float t, EasingFunction easingFunction)
        {
            return easingFunction(t);
        }
    };
}
