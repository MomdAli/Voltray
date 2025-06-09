# Math module CMakeLists.txt
add_library(VoltrayMath STATIC
    # Source files from Private directory
    Private/Mat4.cpp
    Private/Ray.cpp
    Private/Transform.cpp
    Private/Vec3.cpp
    Private/Vec4.cpp

    # Header files from Public directory
    Public/Mat4.h
    Public/MathUtil.h
    Public/Ray.h
    Public/Transform.h
    Public/Vec2.h
    Public/Vec3.h
    Public/Vec4.h
)

# Set include directories for this library
target_include_directories(VoltrayMath PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/include  # Add the custom header directory
)

# Create an alias that maps Math headers to the Public directory
target_include_directories(VoltrayMath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Alias for consistent naming
add_library(Voltray::Math ALIAS VoltrayMath)
