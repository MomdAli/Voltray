# Engine Graphics Camera module CMakeLists.txt
add_library(VoltrayEngineGraphicsCamera STATIC
    BaseCamera.cpp
    BaseCamera.h
    CameraAnimator.cpp
    CameraAnimator.h
    OrthographicCamera.cpp
    OrthographicCamera.h
    PerspectiveCamera.cpp
    PerspectiveCamera.h
)

# Set include directories for this library
target_include_directories(VoltrayEngineGraphicsCamera PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEngineGraphicsCamera PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Engine::Graphics::Camera ALIAS VoltrayEngineGraphicsCamera)
