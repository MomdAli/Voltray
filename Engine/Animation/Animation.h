#pragma once

/**
 * @file Animation.h
 * @brief Main header for the Animation system
 *
 * This header includes all animation-related functionality including:
 * - Easing types and functions
 * - Animation utilities for interpolation
 * - High-level animation interfaces
 */

#include "EasingTypes.h"
#include "EasingFunctions.h"
#include "AnimationUtil.h"

/**
 * @namespace Animation
 * @brief Contains all animation-related classes and functions
 *
 * The Animation namespace provides a comprehensive set of easing functions
 * and utilities for creating smooth animations and transitions. It includes:
 *
 * - 30+ different easing functions (sine, quad, cubic, bounce, elastic, etc.)
 * - Templated interpolation functions that work with any numeric type
 * - Easy-to-use enum-based easing selection
 * - Custom easing function support
 * - Multi-point interpolation capabilities
 */
