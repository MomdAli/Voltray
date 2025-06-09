# Animation module CMakeLists.txt
add_library(VoltrayEngineAnimation STATIC
    Private/EasingFunctions.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineAnimation PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEngineAnimation PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Engine::Animation ALIAS VoltrayEngineAnimation)

# If you want to build the test program
if(BUILD_ANIMATION_TESTS)
    add_executable(AnimationTest Private/AnimationTest.cpp)
    target_link_libraries(AnimationTest VoltrayEngineAnimation)

    # Set C++17 or later for the test
    target_compile_features(AnimationTest PRIVATE cxx_std_17)
endif()
