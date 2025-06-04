# Math module CMakeLists.txt
add_library(VoltrayMath STATIC
    Mat4.cpp
    Mat4.h
    MathUtil.h
    Ray.cpp
    Ray.h
    Transform.cpp
    Transform.h
    Vec2.h
    vec3.cpp
    Vec3.h
    Vec4.cpp
    Vec4.h
)

# Set include directories for this library
target_include_directories(VoltrayMath PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Alias for consistent naming
add_library(Voltray::Math ALIAS VoltrayMath)
