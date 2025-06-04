# Engine Graphics module CMakeLists.txt

# Add subdirectories first
add_subdirectory(Camera)

# Create the main Graphics library
add_library(VoltrayEngineGraphics STATIC
    IndexBuffer.cpp
    IndexBuffer.h
    Mesh.cpp
    Mesh.h
    Renderer.cpp
    Renderer.h
    Shader.cpp
    Shader.h
    VertexArray.cpp
    VertexArray.h
    VertexBuffer.cpp
    VertexBuffer.h
)

# Set include directories for this library
target_include_directories(VoltrayEngineGraphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEngineGraphics PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEngineGraphicsCamera
    glfw
    OpenGL::GL
)

# Alias for consistent naming
add_library(Voltray::Engine::Graphics ALIAS VoltrayEngineGraphics)
