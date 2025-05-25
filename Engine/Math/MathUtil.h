// filepath: Engine/Math/MathUtil.h
#pragma once

namespace MathUtil
{
    // Clamp a value between min and max
    template <typename T>
    T Clamp(const T &value, const T &min, const T &max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }

    // Linear interpolation
    template <typename T>
    T Lerp(const T &a, const T &b, float t)
    {
        return a + (b - a) * t;
    }

    // Map a value from one range to another
    template <typename T>
    T Map(const T &value, const T &inMin, const T &inMax, const T &outMin, const T &outMax)
    {
        return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
    }

    // Sign function
    template <typename T>
    int Sign(const T &value)
    {
        return (T(0) < value) - (value < T(0));
    }
}
