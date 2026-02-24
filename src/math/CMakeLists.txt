# Math module CMakeLists.txt
add_library(VoltrayMath STATIC
    # Source files from private directory
    private/Mat4.cpp
    private/Ray.cpp
    private/Transform.cpp
    private/Vec3.cpp
    private/Vec4.cpp

    # Header files from public directory
    public/Mat4.h
    public/MathUtil.h
    public/Ray.h
    public/Transform.h
    public/Vec2.h
    public/Vec3.h
    public/Vec4.h
)

# Set include directories for this library
target_include_directories(VoltrayMath PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/include  # Add the custom header directory
)

# Create an alias that maps Math headers to the public directory
target_include_directories(VoltrayMath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Alias for consistent naming
add_library(Voltray::Math ALIAS VoltrayMath)
