#include "Engine/Animation/Animation.h"
#include "Engine/Animation/AnimationExamples.h"
#include "Math/MathUtil.h"
#include <iostream>

int main()
{
    std::cout << "Testing Animation System Integration" << std::endl;
    std::cout << "====================================" << std::endl
              << std::endl;

    // Test basic MathUtil::Lerp (should still work)
    float basicLerp = MathUtil::Lerp(0.0f, 10.0f, 0.5f);
    std::cout << "Basic MathUtil::Lerp(0, 10, 0.5) = " << basicLerp << std::endl;

    // Test MathUtil::Lerp with easing
    float easedLerp = MathUtil::Lerp(0.0f, 10.0f, 0.5f, Animation::EasingType::EaseOutBounce);
    std::cout << "Eased MathUtil::Lerp(0, 10, 0.5, EaseOutBounce) = " << easedLerp << std::endl;

    // Test direct Animation system usage
    float directAnim = Animation::AnimationUtil::Lerp(0.0f, 10.0f, 0.5f, Animation::EasingType::EaseInElastic);
    std::cout << "Direct Animation::Lerp(0, 10, 0.5, EaseInElastic) = " << directAnim << std::endl;

    // Test with different types
    int intLerp = Animation::AnimationUtil::Lerp(5, 15, 0.7f, Animation::EasingType::EaseInOutQuad);
    std::cout << "Integer Animation::Lerp(5, 15, 0.7, EaseInOutQuad) = " << intLerp << std::endl;

    double doubleLerp = Animation::AnimationUtil::Lerp(1.5, 4.8, 0.3f, Animation::EasingType::EaseOutSine);
    std::cout << "Double Animation::Lerp(1.5, 4.8, 0.3, EaseOutSine) = " << doubleLerp << std::endl;

    std::cout << std::endl;

    // Run all examples
    Animation::AnimationExamples::RunAllExamples();

    return 0;
}
