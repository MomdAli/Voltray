# Engine Graphics Camera module CMakeLists.txt
add_library(VoltrayEngineGraphicsCamera STATIC
    private/BaseCamera.cpp
    private/CameraAnimator.cpp
    private/OrthographicCamera.cpp
    private/PerspectiveCamera.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineGraphicsCamera PUBLIC
    public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/include  # Add the custom header directory for Math/ includes
    ${CMAKE_SOURCE_DIR}/Math/public  # Add direct path to Math headers
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
