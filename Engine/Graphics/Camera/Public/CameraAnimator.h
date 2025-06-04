#pragma once

#include "Vec3.h"
#include <functional>

using namespace Voltray::Math;

namespace Voltray::Engine
{

    // Forward declaration
    class BaseCamera;

    /**
     * @class CameraAnimator
     * @brief Handles smooth camera transitions and animations
     *
     * This utility class provides smooth interpolated movements for camera operations
     * like focusing on objects, transitioning between positions, and other animated
     * camera behaviors.
     */
    class CameraAnimator
    {
    public:
        /**
         * @brief Constructor
         * @param camera Reference to the camera to animate
         */
        CameraAnimator(BaseCamera &camera);

        /**
         * @brief Update the animation - call this every frame
         * @param deltaTime Time elapsed since last frame in seconds
         */
        void Update(float deltaTime);

        /**
         * @brief Smoothly focus the camera on a target position
         * @param targetPosition The position to focus on
         * @param duration How long the animation should take (in seconds)
         * @param onComplete Optional callback when animation completes
         */
        void FocusOnPosition(const Vec3 &targetPosition, float duration = 1.0f,
                             std::function<void()> onComplete = nullptr);

        /**
         * @brief Smoothly move camera to a new position and target
         * @param newPosition New camera position
         * @param newTarget New camera target
         * @param duration How long the animation should take (in seconds)
         * @param onComplete Optional callback when animation completes
         */
        void MoveTo(const Vec3 &newPosition, const Vec3 &newTarget, float duration = 1.0f,
                    std::function<void()> onComplete = nullptr);

        /**
         * @brief Check if any animation is currently active
         * @return True if animating, false otherwise
         */
        bool IsAnimating() const { return m_IsAnimating; }

        /**
         * @brief Stop any current animation
         */
        void StopAnimation();

        /**
         * @brief Set the easing function type
         */
        enum class EasingType
        {
            Linear,
            EaseInOut,
            EaseIn,
            EaseOut,
            EaseInOutQuad,
            EaseInOutCubic
        };

        /**
         * @brief Set the easing type for animations
         * @param type The easing type to use
         */
        void SetEasingType(EasingType type) { m_EasingType = type; }

    private:
        /**
         * @brief Apply easing function to a normalized time value (0-1)
         * @param t Normalized time (0-1)
         * @return Eased time value
         */
        float ApplyEasing(float t) const;

        /**
         * @brief Smoothly interpolate between two Vec3 values
         * @param start Starting value
         * @param end Ending value
         * @param t Interpolation factor (0-1)
         * @return Interpolated value
         */
        Vec3 Lerp(const Vec3 &start, const Vec3 &end, float t) const;

        BaseCamera &m_Camera;

        // Animation state
        bool m_IsAnimating = false;
        float m_AnimationTime = 0.0f;
        float m_AnimationDuration = 1.0f;
        EasingType m_EasingType = EasingType::EaseInOut;

        // Animation targets
        Vec3 m_StartPosition;
        Vec3 m_StartTarget;
        Vec3 m_EndPosition;
        Vec3 m_EndTarget; // Completion callback
        std::function<void()> m_OnComplete;
    };

}
