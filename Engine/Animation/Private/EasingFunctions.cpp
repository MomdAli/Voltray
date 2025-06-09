#include "EasingFunctions.h"

namespace Voltray::Engine
{
    // Linear
    float EasingFunctions::Linear(float t)
    {
        return t;
    }

    // Sine
    float EasingFunctions::EaseInSine(float t)
    {
        return 1.0f - cos((t * M_PI) / 2.0f);
    }

    float EasingFunctions::EaseOutSine(float t)
    {
        return sin((t * M_PI) / 2.0f);
    }

    float EasingFunctions::EaseInOutSine(float t)
    {
        return -(cos(M_PI * t) - 1.0f) / 2.0f;
    }

    // Quad
    float EasingFunctions::EaseInQuad(float t)
    {
        return t * t;
    }

    float EasingFunctions::EaseOutQuad(float t)
    {
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    float EasingFunctions::EaseInOutQuad(float t)
    {
        return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
    }

    // Cubic
    float EasingFunctions::EaseInCubic(float t)
    {
        return t * t * t;
    }

    float EasingFunctions::EaseOutCubic(float t)
    {
        return 1.0f - pow(1.0f - t, 3.0f);
    }

    float EasingFunctions::EaseInOutCubic(float t)
    {
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    // Quart
    float EasingFunctions::EaseInQuart(float t)
    {
        return t * t * t * t;
    }

    float EasingFunctions::EaseOutQuart(float t)
    {
        return 1.0f - pow(1.0f - t, 4.0f);
    }

    float EasingFunctions::EaseInOutQuart(float t)
    {
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
    }

    // Quint
    float EasingFunctions::EaseInQuint(float t)
    {
        return t * t * t * t * t;
    }

    float EasingFunctions::EaseOutQuint(float t)
    {
        return 1.0f - pow(1.0f - t, 5.0f);
    }

    float EasingFunctions::EaseInOutQuint(float t)
    {
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
    }

    // Expo
    float EasingFunctions::EaseInExpo(float t)
    {
        return t == 0.0f ? 0.0f : pow(2.0f, 10.0f * (t - 1.0f));
    }

    float EasingFunctions::EaseOutExpo(float t)
    {
        return t == 1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * t);
    }

    float EasingFunctions::EaseInOutExpo(float t)
    {
        if (t == 0.0f)
            return 0.0f;
        if (t == 1.0f)
            return 1.0f;
        return t < 0.5f ? pow(2.0f, 20.0f * t - 10.0f) / 2.0f : (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    // Circ
    float EasingFunctions::EaseInCirc(float t)
    {
        return 1.0f - sqrt(1.0f - pow(t, 2.0f));
    }

    float EasingFunctions::EaseOutCirc(float t)
    {
        return sqrt(1.0f - pow(t - 1.0f, 2.0f));
    }

    float EasingFunctions::EaseInOutCirc(float t)
    {
        return t < 0.5f
                   ? (1.0f - sqrt(1.0f - pow(2.0f * t, 2.0f))) / 2.0f
                   : (sqrt(1.0f - pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    // Back
    float EasingFunctions::EaseInBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return c3 * t * t * t - c1 * t * t;
    }

    float EasingFunctions::EaseOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
    }

    float EasingFunctions::EaseInOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;

        return t < 0.5f
                   ? (pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
                   : (pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    // Elastic
    float EasingFunctions::EaseInElastic(float t)
    {
        const float c4 = (2.0f * M_PI) / 3.0f;

        if (t == 0.0f)
            return 0.0f;
        if (t == 1.0f)
            return 1.0f;

        return -pow(2.0f, 10.0f * (t - 1.0f)) * sin((t - 1.0f) * c4);
    }

    float EasingFunctions::EaseOutElastic(float t)
    {
        const float c4 = (2.0f * M_PI) / 3.0f;

        if (t == 0.0f)
            return 0.0f;
        if (t == 1.0f)
            return 1.0f;

        return pow(2.0f, -10.0f * t) * sin(t * c4) + 1.0f;
    }

    float EasingFunctions::EaseInOutElastic(float t)
    {
        const float c5 = (2.0f * M_PI) / 4.5f;

        if (t == 0.0f)
            return 0.0f;
        if (t == 1.0f)
            return 1.0f;

        return t < 0.5f
                   ? -(pow(2.0f, 20.0f * t - 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f
                   : (pow(2.0f, -20.0f * t + 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
    }

    // Bounce
    float EasingFunctions::EaseInBounce(float t)
    {
        return 1.0f - EaseOutBounce(1.0f - t);
    }

    float EasingFunctions::EaseOutBounce(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1.0f / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2.0f / d1)
        {
            return n1 * (t -= 1.5f / d1) * t + 0.75f;
        }
        else if (t < 2.5f / d1)
        {
            return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        }
        else
        {
            return n1 * (t -= 2.625f / d1) * t + 0.984375f;
        }
    }

    float EasingFunctions::EaseInOutBounce(float t)
    {
        return t < 0.5f
                   ? (1.0f - EaseOutBounce(1.0f - 2.0f * t)) / 2.0f
                   : (1.0f + EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
    }

    // Get easing function by type
    EasingFunction EasingFunctions::GetEasingFunction(EasingType type)
    {
        switch (type)
        {
        case EasingType::Linear:
            return Linear;

        case EasingType::EaseInSine:
            return EaseInSine;
        case EasingType::EaseOutSine:
            return EaseOutSine;
        case EasingType::EaseInOutSine:
            return EaseInOutSine;

        case EasingType::EaseInQuad:
            return EaseInQuad;
        case EasingType::EaseOutQuad:
            return EaseOutQuad;
        case EasingType::EaseInOutQuad:
            return EaseInOutQuad;

        case EasingType::EaseInCubic:
            return EaseInCubic;
        case EasingType::EaseOutCubic:
            return EaseOutCubic;
        case EasingType::EaseInOutCubic:
            return EaseInOutCubic;

        case EasingType::EaseInQuart:
            return EaseInQuart;
        case EasingType::EaseOutQuart:
            return EaseOutQuart;
        case EasingType::EaseInOutQuart:
            return EaseInOutQuart;

        case EasingType::EaseInQuint:
            return EaseInQuint;
        case EasingType::EaseOutQuint:
            return EaseOutQuint;
        case EasingType::EaseInOutQuint:
            return EaseInOutQuint;

        case EasingType::EaseInExpo:
            return EaseInExpo;
        case EasingType::EaseOutExpo:
            return EaseOutExpo;
        case EasingType::EaseInOutExpo:
            return EaseInOutExpo;

        case EasingType::EaseInCirc:
            return EaseInCirc;
        case EasingType::EaseOutCirc:
            return EaseOutCirc;
        case EasingType::EaseInOutCirc:
            return EaseInOutCirc;

        case EasingType::EaseInBack:
            return EaseInBack;
        case EasingType::EaseOutBack:
            return EaseOutBack;
        case EasingType::EaseInOutBack:
            return EaseInOutBack;

        case EasingType::EaseInElastic:
            return EaseInElastic;
        case EasingType::EaseOutElastic:
            return EaseOutElastic;
        case EasingType::EaseInOutElastic:
            return EaseInOutElastic;

        case EasingType::EaseInBounce:
            return EaseInBounce;
        case EasingType::EaseOutBounce:
            return EaseOutBounce;
        case EasingType::EaseInOutBounce:
            return EaseInOutBounce;

        default:
            return Linear;
        }
    }
}
