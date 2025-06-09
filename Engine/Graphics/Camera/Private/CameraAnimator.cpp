#include "CameraAnimator.h"
#include "BaseCamera.h"
#include <cmath>
#include <algorithm>

namespace Voltray::Engine
{

    CameraAnimator::CameraAnimator(BaseCamera &camera)
        : m_Camera(camera)
    {
    }

    void CameraAnimator::Update(float deltaTime)
    {
        if (!m_IsAnimating)
            return;

        m_AnimationTime += deltaTime;
        float t = m_AnimationTime / m_AnimationDuration;

        if (t >= 1.0f)
        {
            // Animation complete
            t = 1.0f;
            m_IsAnimating = false;

            // Set final values
            m_Camera.SetPosition(m_EndPosition);
            m_Camera.SetTarget(m_EndTarget);

            // Call completion callback if set
            if (m_OnComplete)
            {
                m_OnComplete();
                m_OnComplete = nullptr;
            }
        }
        else
        {
            // Apply easing and interpolate
            float easedT = ApplyEasing(t);

            Vec3 currentPosition = Lerp(m_StartPosition, m_EndPosition, easedT);
            Vec3 currentTarget = Lerp(m_StartTarget, m_EndTarget, easedT);

            // Update camera without triggering orbit calculations
            m_Camera.LookAt(currentPosition, currentTarget, m_Camera.GetUp());
        }
    }

    void CameraAnimator::FocusOnPosition(const Vec3 &targetPosition, float duration,
                                         std::function<void()> onComplete)
    {
        // Calculate new camera position maintaining current distance and direction
        Vec3 currentDirection = (m_Camera.GetPosition() - m_Camera.GetTarget()).Normalize();
        float currentDistance = (m_Camera.GetPosition() - m_Camera.GetTarget()).Length();

        Vec3 newPosition = targetPosition + currentDirection * currentDistance;

        MoveTo(newPosition, targetPosition, duration, onComplete);
    }

    void CameraAnimator::MoveTo(const Vec3 &newPosition, const Vec3 &newTarget, float duration,
                                std::function<void()> onComplete)
    {
        // Store current state as start values
        m_StartPosition = m_Camera.GetPosition();
        m_StartTarget = m_Camera.GetTarget();

        // Store target state
        m_EndPosition = newPosition;
        m_EndTarget = newTarget;

        // Setup animation
        m_AnimationTime = 0.0f;
        m_AnimationDuration = std::max(0.1f, duration); // Minimum duration to prevent division by zero
        m_IsAnimating = true;
        m_OnComplete = onComplete;
    }

    void CameraAnimator::StopAnimation()
    {
        m_IsAnimating = false;
        m_OnComplete = nullptr;
    }

    float CameraAnimator::ApplyEasing(float t) const
    {
        // Clamp t to [0, 1]
        t = std::clamp(t, 0.0f, 1.0f);

        switch (m_EasingType)
        {
        case EasingType::Linear:
            return t;

        case EasingType::EaseIn:
            return t * t;

        case EasingType::EaseOut:
            return 1.0f - (1.0f - t) * (1.0f - t);

        case EasingType::EaseInOut:
            return t < 0.5f ? 2.0f * t * t : 1.0f - 2.0f * (1.0f - t) * (1.0f - t);

        case EasingType::EaseInOutQuad:
            return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;

        case EasingType::EaseInOutCubic:
            return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;

        default:
            return t;
        }
    }

    Vec3 CameraAnimator::Lerp(const Vec3 &start, const Vec3 &end, float t) const
    {
        return start + (end - start) * t;
    }

}
