# Animation System

A comprehensive animation and easing system for the Voltray engine providing smooth interpolation and transitions.

Special thanks to [easings.net](https://easings.net/) for inspiration and reference on easing functions.

## Features

- **30+ Built-in Easing Functions**: Including sine, quad, cubic, quart, quint, expo, circ, back, elastic, and bounce variations
- **Type-Safe Templates**: Works with any numeric type (int, float, double, custom types)
- **Flexible API**: Support for both enum-based easing selection and custom easing functions
- **Multi-Point Interpolation**: Interpolate between multiple keyframe values
- **Integration with MathUtil**: Seamlessly extends the existing Lerp functionality

## Easing Types

### Basic
- `Linear` - No easing

### Sine
- `EaseInSine`, `EaseOutSine`, `EaseInOutSine`

### Quadratic
- `EaseInQuad`, `EaseOutQuad`, `EaseInOutQuad`

### Cubic
- `EaseInCubic`, `EaseOutCubic`, `EaseInOutCubic`

### Quartic
- `EaseInQuart`, `EaseOutQuart`, `EaseInOutQuart`

### Quintic
- `EaseInQuint`, `EaseOutQuint`, `EaseInOutQuint`

### Exponential
- `EaseInExpo`, `EaseOutExpo`, `EaseInOutExpo`

### Circular
- `EaseInCirc`, `EaseOutCirc`, `EaseInOutCirc`

### Back (Overshoot)
- `EaseInBack`, `EaseOutBack`, `EaseInOutBack`

### Elastic
- `EaseInElastic`, `EaseOutElastic`, `EaseInOutElastic`

### Bounce
- `EaseInBounce`, `EaseOutBounce`, `EaseInOutBounce`

## Usage Examples

### Basic Interpolation

```cpp
#include "Engine/Animation/Animation.h"

// Basic linear interpolation
float result = Animation::AnimationUtil::Lerp(0.0f, 100.0f, 0.5f);

// With easing
float eased = Animation::AnimationUtil::Lerp(0.0f, 100.0f, 0.5f, Animation::EasingType::EaseOutBounce);

// With custom easing function
auto customEasing = [](float t) { return t * t; };
float custom = Animation::AnimationUtil::Lerp(0.0f, 100.0f, 0.5f, customEasing);
```

### Using with MathUtil

```cpp
#include "MathUtil.h"

// Basic lerp (unchanged)
float basic = MathUtil::Lerp(10.0f, 20.0f, 0.3f);

// With animation easing
float animated = MathUtil::Lerp(10.0f, 20.0f, 0.3f, Animation::EasingType::EaseInOutQuad);

// With custom function
auto ease = Animation::EasingFunctions::EaseOutElastic;
float elastic = MathUtil::Lerp(10.0f, 20.0f, 0.3f, ease);
```

### Multi-Point Interpolation

```cpp
float keyframes[] = {0.0f, 5.0f, 2.0f, 8.0f, 1.0f};
float result = Animation::AnimationUtil::MultiLerp(keyframes, 5, 0.7f, Animation::EasingType::EaseInOutSine);
```

### Type-Safe Usage

```cpp
// Works with any numeric type
int intResult = Animation::AnimationUtil::Lerp(10, 50, 0.5f, Animation::EasingType::Linear);
double doubleResult = Animation::AnimationUtil::Lerp(1.5, 3.7, 0.3f, Animation::EasingType::EaseOutBounce);

// Custom types (must support arithmetic operations)
struct Vector3 { float x, y, z; /* ... */ };
Vector3 interpolated = Animation::AnimationUtil::Lerp(start, end, 0.5f, Animation::EasingType::EaseInQuad);
```

## Architecture

### Core Components

1. **EasingTypes.h** - Defines the `EasingType` enum and `EasingFunction` type
2. **EasingFunctions.h/.cpp** - Implementation of all easing functions
3. **AnimationUtil.h** - High-level templated interpolation utilities
4. **Animation.h** - Main include header

### Design Principles

- **Abstraction**: High-level API hides implementation complexity
- **Flexibility**: Support for both predefined and custom easing functions
- **Performance**: Template-based for compile-time optimization
- **Extensibility**: Easy to add new easing functions or interpolation methods

## Integration Notes

- The system is header-only except for the easing function implementations
- Integrates seamlessly with existing `MathUtil::Lerp` functions
- No external dependencies beyond standard C++ math library
- Compatible with any type that supports basic arithmetic operations

## Adding Custom Easing Functions

1. Add the function to `EasingFunctions` class
2. Add enum value to `EasingType` (if desired)
3. Update the `GetEasingFunction` switch statement

```cpp
// In EasingFunctions.h
static float MyCustomEasing(float t);

// In EasingFunctions.cpp
float EasingFunctions::MyCustomEasing(float t)
{
    return /* your easing calculation */;
}
```
