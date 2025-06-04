# Animation module CMakeLists.txt
add_library(VoltrayAnimation STATIC
    EasingFunctions.cpp
    EasingFunctions.h
    Animation.h
    EasingTypes.h
    AnimationUtil.h
    AnimationExamples.h
    AnimationTest.cpp
)

# Set include directories for this library
target_include_directories(VoltrayAnimation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Alias for consistent naming
add_library(Voltray::Animation ALIAS VoltrayAnimation)

# If you want to build the test program
if(BUILD_ANIMATION_TESTS)
    add_executable(AnimationTest Engine/Animation/AnimationTest.cpp ${ANIMATION_SOURCES})
    target_link_libraries(AnimationTest)

    # Set C++17 or later for the test
    target_compile_features(AnimationTest PRIVATE cxx_std_17)
endif()

# Make headers available to the main project
# target_include_directories(YourMainTarget PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
