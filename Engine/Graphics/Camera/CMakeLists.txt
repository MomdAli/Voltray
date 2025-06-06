# Engine Graphics Camera module CMakeLists.txt
add_library(VoltrayEngineGraphicsCamera STATIC
    Private/BaseCamera.cpp
    Private/CameraAnimator.cpp
    Private/OrthographicCamera.cpp
    Private/PerspectiveCamera.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineGraphicsCamera PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/include  # Add the custom header directory for Math/ includes
    ${CMAKE_SOURCE_DIR}/Math/Public  # Add direct path to Math headers
    ${glfw_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEngineGraphicsCamera PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEngineInput
    VoltrayEngineConfig
    glfw
)

# Alias for consistent naming
add_library(Voltray::Engine::Graphics::Camera ALIAS VoltrayEngineGraphicsCamera)
