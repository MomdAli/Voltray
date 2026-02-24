#pragma once

#define M_PI 3.141592653589793238467932384626433f   // Pi constant
#define M_PI_2 1.570796326794896619231321691639751f // Pi / 2
#define DEG2RAD 0.01745329251994329576923690768489f // Pi / 180.0f
#define RAD2DEG 57.295779513082320876798154814105f  // 180.0f / Pi

namespace Voltray::Math
{

    class MathUtil
    {
    public:
        // Clamp a value between min and max
        template <typename T>
        static T Clamp(const T &value, const T &min, const T &max)
        {
            if (value < min)
                return min;
            if (value > max)
                return max;
            return value;
        }

        // Linear interpolation (basic version)
        template <typename T>
        static T Lerp(const T &a, const T &b, float t)
        {
            return a + (b - a) * t;
        }

        // Map a value from one range to another
        template <typename T>
        static T Map(const T &value, const T &inMin, const T &inMax, const T &outMin, const T &outMax)
        {
            return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
        }

        // Sign function
        template <typename T>
        static int Sign(const T &value)
        {
            return (T(0) < value) - (value < T(0));
        }
    };

}
