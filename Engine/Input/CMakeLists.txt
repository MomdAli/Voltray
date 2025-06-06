# Engine Input module CMakeLists.txt
add_library(VoltrayEngineInput STATIC
    Private/Input.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineInput PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEngineInput PUBLIC
    VoltrayMath
    VoltrayUtils
    glfw
)

# Alias for consistent naming
add_library(Voltray::Engine::Input ALIAS VoltrayEngineInput)
