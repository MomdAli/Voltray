# Engine Input module CMakeLists.txt
add_library(VoltrayEngineInput STATIC
    Input.cpp
    Input.h
)

# Set include directories for this library
target_include_directories(VoltrayEngineInput PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
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
